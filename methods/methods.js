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

L5EL = {
    "name": "L5EL",
    "steps": [
        {
            "name": "K_L",
            "type": "find",
            "template": {
                "edgesIgnore": [0, 1, 2, 3, 4],
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
            "name": "L5E",
            "type": "alg",
            "algs": {
                "source": "file",
                "filePath": "methods/l5e_l.txt"
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

L5ER = {
    "name": "L5ER",
    "steps": [
        {
            "name": "K_R",
            "type": "find",
            "template": {
                "edgesIgnore": [0, 1, 2, 3, 5],
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
            "name": "L5E",
            "type": "alg",
            "algs": {
                "source": "file",
                "filePath": "methods/l5e_r.txt"
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

L5EF = {
    "name": "L5EF",
    "steps": [
        {
            "name": "K_F",
            "type": "find",
            "template": {
                "edgesIgnore": [0, 1, 2, 4, 5],
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
            "name": "L5E",
            "type": "alg",
            "algs": {
                "source": "file",
                "filePath": "methods/l5e_f.txt"
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

TL4EB = {
    "name": "TL4EB",
    "steps": [
        {
            "name": "TV_B",
            "type": "find",
            "template": {
                "edgesIgnore": [0, 1, 2, 3],
                "centersIgnore": [0, 3]
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
            "name": "TL4E_B",
            "type": "alg",
            "algs": {
                "source": "file",
                "filePath": "methods/tl4e_b.txt"
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
