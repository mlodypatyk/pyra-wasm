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
    console.log(this.methodJson)

    this.algsByHash = {}
    let algs = [{"moves": [], "steps": []}]
    for(const step of methodJson["steps"]){
      if(step["type"] == "alg"){
        if(step["algs"]["source"] == "file"){
          var xmlHTTP = new XMLHttpRequest()
          xmlHTTP.open("GET", step["algs"]["filePath"], false)
          xmlHTTP.send(null)
          let newalgs = xmlHTTP.responseText.replaceAll('\r\n', '\n').split('\n')
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
    console.log(this.algsByHash)
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
    all_solutions.sort(function compare(a, b) { return countRealMoves(a.moves) - countRealMoves(b.moves)})

    return all_solutions

  }

  
}

var Module = {
    onRuntimeInitialized: function() {
      l4e_m = new Method(L4E);
      lbl_m = new Method(LBL)
      document.getElementById('search').addEventListener("click", async () => {
        scrText = document.getElementById('input').value
        scrClean = scrText.split(' ').filter((move) => ["R", "R'", "L", "L'", "U", "U'", "B", "B'"].includes(move)).join(' ')
        scr = createScrambleFromString(scrClean)
        solutions = l4e_m.findSolution(scr)
        prevSolutions = []
        printed = 0;
        finalText = ''
        for(const solution of solutions){
          if(!prevSolutions.includes(solution.moves.join(' ')) && printed < 20){
            finalText += printSolution(solution)
            finalText += '<br>'
            printed++
            prevSolutions.push(solution.moves.join(' '))
          }
        }
        document.getElementById('solution').innerHTML = finalText})
    }
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
    finalStr += endMoveCount.toString() + ' moves (' + (normalMoveCount-endMoveCount).toString() + ' moves cancelled)<br>'
    return finalStr
  }
  function getSolutionList(solution){
    result = []
    for(i = 0; i< solution.size(); i++){
      //console.log(solution.get(i));
      result.push(Module.getLetterMove(solution.get(i)))
    }
    return result
  }
