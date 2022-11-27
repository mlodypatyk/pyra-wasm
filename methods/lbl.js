LBL = {
    "name": "LBL",
    "steps":[
        {
            "name": "Layer",
            "type": "find",
            "template": {
                "edgesIgnore": [0, 1, 2],
                "centersIgnore": [0]
            },
            "tolerance": 1
        },
        {
            "name": "Pre-AUF",
            "type": "alg",
            "algs": {
                "source": "inline",
                "algs": ["", "U", "U'"]
            }
        },
        {
            "name": "LL",
            "type": "alg",
            "algs": {
                "source": "inline",
                "algs": [
                "", 
                "L U R U' R' L'",
                "R' U' L' U L R",
                "L U L' U L U L'",
                "R U' R' U' R U' R'",
                "R' L R L' U L' U' L"
                ]
            }
        },
        {
            "name": "AUF",
            "type": "alg",
            "algs": {
                "source": "inline",
                "algs": ["", "U", "U'"]
            }
        }
    ]
  }