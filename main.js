class Method{
  constructor(methodJson){
    this.methodJson = methodJson
    //this.templates = []
    // TODO: check if method is correct (finds -> algs)
    // TODO: multiple templates
    for(const step of methodJson["steps"]){
      if(step["type"] == "find"){
        let edgesToIgnore = new Module.VectorInt()
        for(const edge of step["template"]["edgesIgnore"]){
          edgesToIgnore.push_back(edge)
        }
        let centersToIgnore = new Module.VectorInt()
        for(const center of step["template"]["centersIgnore"]){
          centersToIgnore.push_back(center)
        }
        let templateList = new Module.VectorPyraminx()
        let template = new Module.Pyraminx(edgesToIgnore, centersToIgnore)
        templateList.push_back(template)
        step["template"] = templateList
      }
    }
    //console.log(this.methodJson)

    this.algsByHash = {}
    let algs = [{"moves": [], "steps": []}]
    for(const step of methodJson["steps"]){
      if(step["type"] == "alg"){
        if(step["algs"]["source"] == "file"){
          var xmlHTTP = new XMLHttpRequest()
          xmlHTTP.open("GET", step["algs"]["filePath"], false)
          xmlHTTP.send(null)
          let newalgs = xmlHTTP.responseText.replaceAll('\r\n', '\n').split('\n').filter((alg) => alg[0] != '#')
          step["algs"]["algs"] = newalgs                
        }
        let newAlgs = []
        for(const algOuter of algs){
          for(const algInner of step["algs"]["algs"]){
            let algInnerSplit = algInner.split(' ')
            //console.log(algInnerSplit);
            if(algInnerSplit[0] != ''){
              let newAlg = Object.assign({}, algOuter) // copy
              newAlg["moves"] = newAlg["moves"].concat(algInnerSplit)
              newAlg["steps"] = newAlg["steps"].concat([[algInnerSplit, step["name"]]])
              newAlgs.push(newAlg)
            }else{
              newAlgs.push(algOuter)
            }
          }
        }
        algs = newAlgs
      }
    }
    for(const alg of algs){
      alg["moves"].reverse()
      let puzzle = new Module.Pyraminx()
      for(const move of alg["moves"]){
        let innerMove = Module.getInverse(Module.getInnerMove(move))
        puzzle.makeMove(innerMove)
      }
      alg["moves"].reverse()
      let hash = puzzle.hash()
      if(hash in this.algsByHash){
        this.algsByHash[hash].push(alg)
      }else{
        this.algsByHash[hash] = [alg]
      }
    }
    //console.log(this.algsByHash)
  }
  findSolution(scramble){
    //TODO: multiple finds maybe?
    let solutions = []
    // "find" steps
    for(const step of this.methodJson.steps){
      if(step.type == "find"){
        let stepSolutions = Module.findSolution(scramble, step.template, step.tolerance, true);
        for(let i = 0 ;i <stepSolutions.size();i++){
          let stepSolutionList = getSolutionList(stepSolutions.get(i))
          solutions.push({"moves": stepSolutionList, "steps": [[stepSolutionList, step.name]]})
        }
      }
    }
    //console.log(solutions)
    // "alg" steps
    let all_solutions = []
    for(let solution of solutions){
      // Move the solution by the rotations
      let actual_solution = []
      let rotations = []
      for(const move of solution["moves"]){
        if(move.includes('[')){
          rotations.push(move)
        }else{
          actual_solution.push(move)
        }
      }
      let rotated_scramble = getSolutionList(scramble)
      for(const rotation of rotations){
        let new_rotated_scramble = []
        for(const move of rotated_scramble){
          new_rotated_scramble.push(Module.getLetterMove(Module.rotateMove(Module.getInnerMove(move), Module.getInnerMove(rotation))))
        }
        rotated_scramble = new_rotated_scramble
      }
      let pseudoscr = rotated_scramble.concat(actual_solution).join(' ')
      let cube = new Module.Pyraminx(pseudoscr)
      let casehash = cube.hash()
      if(casehash in this.algsByHash){ //we dont need every cringe ml4e case to be solvable, so we dont add all algs
        for(const endCase of this.algsByHash[casehash]){
          //console.log(solution)
          //console.log(endCase)
          let newSolution = 
          {
            "moves": reduceMoves(solution.moves.concat(endCase.moves)),
            "steps": solution.steps.concat(endCase.steps),
          }
          all_solutions.push(newSolution)
        }
      }
    }
    all_solutions.sort(function compare(a, b) { return countRealMoves(a.moves) - countRealMoves(b.moves)})

    return all_solutions

  }

  
}

methods_enabled = []
function toggleMethod(n){
  methods_enabled[n] = !methods_enabled[n];
}

function updateTolerance(n){
  new_moves = parseInt(document.getElementById('input-'+n).value)
  if(new_moves > 0){
    methods[n].methodJson.steps[0].tolerance = new_moves
  }
  
}

var Module = {
    onRuntimeInitialized: function() {
      methods = [new Method(L4E), new Method(LBL), new Method(ML4ER), new Method(ML4EL), new Method(L5EL), new Method(L5ER), new Method(L5EF), new Method(TL4EB)]
      real_methods = methods
      document.getElementById('search').addEventListener("click", () => {
        real_methods = []
        for (index in methods_enabled){
          if (methods_enabled[index]){
            real_methods.push(methods[index])
          }
        }
        scrText = document.getElementById('input').value
        scrClean = scrText.split(' ').filter((move) => ["R", "R'", "L", "L'", "U", "U'", "B", "B'"].includes(move)).join(' ')
        if(scrClean.length == 0) return;
        scr = createScrambleFromString(scrClean)
        solutions = []        
        document.getElementById('solution').innerHTML = '0/' + methods.length + ' methods done';
        setTimeout(() => solveWithMethodN(0, solutions), 0)
      })
      settingsVisible = false;
      document.getElementById('settings').addEventListener("click", () => {
        settingsVisible = !settingsVisible;
        document.getElementById("settings_menu").style.display = settingsVisible ? 'block' : 'none'
      });
      menu = ''
      methods.forEach(element => {
        menu += `<div class="method"><input type="checkbox" class="check" id="checkbox-${methods_enabled.length}" onClick="toggleMethod(${methods_enabled.length})" checked>`
        menu += `<span>${element.methodJson.name}</span><span>tolerance</span>`
        step1 = element.methodJson.steps[0];
        if(step1.type == 'find'){
            menu += `<input type="number" class="method-input" id="input-${methods_enabled.length}" value="${step1.tolerance}" onChange="updateTolerance(${methods_enabled.length})">`
        }
        methods_enabled.push(true);
        menu += '</div>'
      });
      document.getElementById("settings_menu").innerHTML = menu;
    }
  }

  function solveWithMethodN(pos, solutions){
    method = real_methods[pos]
    preFilteredSolutions = method.findSolution(scr)
    thisMethodDone = []
    filteredSolutions = []
    for(const solution of preFilteredSolutions){
      pureSolution = unrotate(solution.moves)
      if(!thisMethodDone.includes(pureSolution)){
        filteredSolutions.push(solution)
        thisMethodDone.push(pureSolution)
      }
    }
    solutions.push(...filteredSolutions)
    pos += 1
    document.getElementById('solution').innerHTML = pos + '/' + real_methods.length + ' methods done';
    if(pos == real_methods.length){
      setTimeout(() => showSolutions(solutions), 0)
    }else{
      setTimeout(() => solveWithMethodN(pos, solutions), 0)
    }
      
  }

  function showSolutions(solutions) {
    solutions.sort((a, b) => {
      // Poprawione porównanie, zwracanie wartości numerycznych
      const diff = countRealMoves(a.moves) - countRealMoves(b.moves);
      if (diff !== 0) return diff;
      return a.steps.length - b.steps.length; // Mniejsza liczba kroków = wyżej
    });

    let solutions_sorted = {};
    let finalText = '';
    for (const solution of solutions) {
        const unrotated_moves = unrotate(solution.moves);
        if (unrotated_moves in solutions_sorted) {
            solutions_sorted[unrotated_moves].push(solution);
        } else { 
            solutions_sorted[unrotated_moves] = [solution];
        }
    }

    let keys = Object.keys(solutions_sorted);
    keys.sort((a, b) => a.split(' ').length - b.split(' ').length); // Poprawka sortowania

    let keys_printed = 0;
    for (const key of keys) {
        if (keys_printed < 20) { // Poprawka, aby cała pętla była w if
            finalText += '<br>';
            for (const solution of solutions_sorted[key]) {
                finalText += printSolution(solution);
                finalText += '<br>';
            }
        }
        keys_printed += 1;
    }

    document.getElementById('solution').innerHTML = finalText;
}


  function createScrambleFromString(scr){
    scr_vector = new Module.VectorMove()
    for(const move of scr.split(' ')){
      scr_vector.push_back(Module.getInnerMove(move))
    }
    return scr_vector
  }
  function reduceMoves(moves){
    while(true){
      change = false
      newMoveList = []
      //console.log(moves)
      for(i=0;i<moves.length-1;i++){
        if(moves[i][0] == moves[i+1][0] && moves[i][0] != '['){
          change = true
          if(moves[i].length == moves[i+1].length){ //ten sam ruch therefore inwersja
            newMove = moves[i][0] + ((moves[i].length == 1) ? "'" : "")
            newMoveList.push(newMove)
          } // w innym casie sie skracaja ze sobą
          i++ // skip
          if(i == moves.length-2){
            newMoveList.push(moves[moves.length-1])
          } //last trzeba dodac nwt przy skipie
        }else{
          newMoveList.push(moves[i])
          if(i == moves.length-2){
            newMoveList.push(moves[moves.length-1])
          }
        }
      }
      moves = newMoveList
      if(!change || moves.length == 1){
        break
      }
    }
    return moves
  }
  function countRealMoves(moves){
    count = 0
    for(const move of moves){
      if(move[0] != '['){
        count++
      }
    }
    return count
  }

  function printSolution(solution){
    //console.log(solution)
    endMoveCount = countRealMoves(solution.moves)
    normalMoveCount = 0
    finalStr = ''
    for(step of solution.steps){
      //console.log(step)
      finalStr += step[0].join(' ') + ' ' + '//' + step[1] + '<br>'
      normalMoveCount += countRealMoves(step[0])
    }
    finalStr += 'solution: ' + solution.moves.join(' ') + '<br>'
    finalStr += endMoveCount.toString() + ' moves (' + (normalMoveCount-endMoveCount).toString() + ' moves cancelled)</br>'
    //finalStr += 'unrotated ' + unrotate(solution.moves) + '<br>' 
    return '<button class="printed-solutions">' + finalStr + '</button>'
  }
  function getSolutionList(solution){
    result = []
    for(i = 0; i< solution.size(); i++){
      //console.log(solution.get(i));
      result.push(Module.getLetterMove(solution.get(i)))
    }
    return result
  }

  function unrotate(moves){
    rotationMatrix = {
      '[b]': {'U': 'R', 'R': 'L', 'L': 'U', 'B': 'B'},
      '[b\']': {'U': 'L', 'L': 'R', 'R': 'U', 'B': 'B'},
      '[u\']': {'R': 'L', 'L': 'B', 'B': 'R', 'U': 'U'},
      '[u\]': {'R': 'B', 'B': 'L', 'L': 'R', 'U': 'U'},
      '[r\']': {'L': 'U', 'U': 'B', 'B': 'L', 'R': 'R'},
      '[r]': {'L': 'B', 'B': 'U', 'U': 'L', 'R': 'R'},
      '[l\']': {'U': 'R', 'R': 'B', 'B': 'U', 'L': 'L'},
      '[l]': {'R': 'U', 'U': 'B', 'B': 'R', 'L': 'L'}
      }
    rotations = []
    bodyMoves = []
    for(const move of moves){
      if(move[0] == '['){
        rotations.push(move)
      }else{
        bodyMoves.push(move)
      }
    }
    rotations.reverse()
    for(const rotation of rotations){
      bodyMoves = bodyMoves.map((move) => rotationMatrix[rotation][move[0]] + '\''.repeat(move.length-1))
    }
    return bodyMoves.join(' ')
  }

  //Importing moves from textarea
  document.addEventListener('DOMContentLoaded', (event) => {
    document.getElementById("search").addEventListener("click", function() {
        var userInput = document.getElementById("input").value;
        var twistyPlayer = document.getElementById("scramble");
        twistyPlayer.setAttribute("alg", userInput);
    });
});


