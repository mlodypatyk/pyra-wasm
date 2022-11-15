var Module = {
    onRuntimeInitialized: function() {
      scr = Module.createSolved();
      console.log(scr)
      /* B R' U L R B U' B' l r' u'*/
      scr.makeMove(Module.bodyMove.B)
      scr.makeMove(Module.bodyMove.Rprim)
      scr.makeMove(Module.bodyMove.U)
      scr.makeMove(Module.bodyMove.L)
      scr.makeMove(Module.bodyMove.R)
      scr.makeMove(Module.bodyMove.B)
      scr.makeMove(Module.bodyMove.Uprim)
      scr.makeMove(Module.bodyMove.Bprim)
      
      solutions = Module.findSolution(scr, Module.createSolved(), 0 , false);
      for(j=0;j<solutions.size();j++){
        console.log(getSolutionString(solutions.get(j)))
      }
    }
  };


  function getSolutionString(solution){
    result = []
    for(i = 0; i< solution.size(); i++){
      result.push(Module.getLetterMove(solution.get(i)))
    }
    return result.join(' ')
  }
