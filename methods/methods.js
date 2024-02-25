L4E = {
    "name": "L4E",
    "steps":[
        {
            "name": "V",
            "type": "find",
            "template": {
                "edgesIgnore": [0, 1, 2, 3],
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
            "name": "L4E",
            "type": "alg",
            "algs": {
                "source": "file",
                "filePath": "methods/l4e.txt"
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
                "source": "file",
                "filePath": "methods/lbl.txt"
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

ML4ER = {
    "name": "ML4ER",
    "steps":[
        {
            "name": "V_R",
            "type": "find",
            "template": {
                "edgesIgnore": [0, 1, 2, 4],
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
            "name": "ML4E",
            "type": "alg",
            "algs": {
                "source": "file",
                "filePath": "methods/ml4e_r.txt"
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

ML4EL= {
    "name": "ML4EL",
    "steps":[
        {
            "name": "V_L",
            "type": "find",
            "template": {
                "edgesIgnore": [0, 1, 2, 5],
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
            "name": "ML4E",
            "type": "alg",
            "algs": {
                "source": "file",
                "filePath": "methods/ml4e_l.txt"
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

  