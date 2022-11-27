
var Module = {
    onRuntimeInitialized: function() {
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

          //TODO: algs from source different than inline
          this.algsByHash = {}
          let algs = [{"moves": [], "steps": []}]
          for(const step of methodJson["steps"]){
            if(step["type"] == "alg"){
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
            for(const endCase of this.algsByHash[casehash]){
              //console.log(solution)
              //console.log(endCase)
              let newSolution = 
              {
                "moves": solution.moves.concat(endCase.moves),
                "steps": solution.steps.concat(endCase.steps),
              }
              all_solutions.push(newSolution)
            }
          }
          return all_solutions

        }

        
      }
      lbl = new Method(LBL);
      /*B' R' U' B' U' B R L' l' u'*/
      scr = new Module.VectorMove()
      scr.push_back(Module.move.Bprim)
      scr.push_back(Module.move.Rprim)
      scr.push_back(Module.move.Uprim)
      scr.push_back(Module.move.Bprim)
      scr.push_back(Module.move.Uprim)
      scr.push_back(Module.move.B)
      scr.push_back(Module.move.R)
      scr.push_back(Module.move.Lprim)
      solutions = lbl.findSolution(scr)
      console.log('SCRAMBLE: ' + getSolutionList(scr).join(' '))
      console.log('')
      for(const solution of solutions){
        printSolution(solution)
        console.log('')
      }
    }
  };

  function printSolution(solution){
    //console.log(solution)
    console.log(solution.moves.join(' '));
    for(step of solution.steps){
      //console.log(step)
      console.log(step[0].join(' ') + ' ' + '//' + step[1])
    }
  }
  function getSolutionList(solution){
    result = []
    for(i = 0; i< solution.size(); i++){
      //console.log(solution.get(i));
      result.push(Module.getLetterMove(solution.get(i)))
    }
    return result
  }
