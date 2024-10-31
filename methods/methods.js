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
    "name": "ML4E-R",
    "steps":[
        {
            "name": "V on Right",
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
            "name": "ML4E-R",
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
    "name": "ML4E-L",
    "steps":[
        {
            "name": "V on Left",
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
            "name": "ML4E-L",
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
    "name": "L5E-L",
    "steps": [
        {
            "name": "Bar on Left",
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
            "name": "L5E-L",
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
    "name": "L5E-R",
    "steps": [
        {
            "name": "Bar on Right",
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
            "name": "L5E-R",
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
    "name": "L5E-F",
    "steps": [
        {
            "name": "Bar on Front",
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
    "name": "TL4E-B",
    "steps": [
        {
            "name": "Twisted Back V",
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
            "name": "TL4E-B",
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

TL4ER = {
    "name": "TL4E-R",
    "steps": [
        {
            "name": "Twisted Right V",
            "type": "find",
            "template": {
                "edgesIgnore": [0, 1, 2, 3],
                "centersIgnore": [0, 1]
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
            "name": "TL4E-R",
            "type": "alg",
            "algs": {
                "source": "file",
                "filePath": "methods/tl4e_r.txt"
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

// Original PL4E with PV steps and l4e.txt algorithms
PL4E = {
    "name": "Pseudo-L4E",
    "steps": [
        {
            "name": "Pseudo-V",
            "type": "find",
            "template": {
                "edgesIgnore": [0, 1, 2, 3, 4],
                "centersIgnore": [0, 1]
            },
            "tolerance": 1
        },
        {
            "name": "Pseudo-V",
            "type": "find",
            "template": {
                "edgesIgnore": [0, 1, 2, 3, 5],
                "centersIgnore": [0, 2]
            },
            "tolerance": 1
        },
        {
            "name": "Pseudo-V",
            "type": "find",
            "template": {
                "edgesIgnore": [0, 1, 2, 3, 4, 5],
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
        },
        {
            "name": "Fix Pseudo",
            "type": "alg",
            "algs": {
                "source": "inline",
                "algs": ["R", "R'", "L", "L'", "B", "B'"]
            }
        }
    ]
};

PML4E = {
    "name": "Pseudo-ML4E",
    "steps": [
        {
            "name": "Pseudo-V",
            "type": "find",
            "template": {
                "edgesIgnore": [0, 1, 2, 3, 5],
                "centersIgnore": [0, 2]
            },
            "tolerance": 1
        },
        {
            "name": "Pseudo-V",
            "type": "find",
            "template": {
                "edgesIgnore": [0, 1, 2, 3, 4, 5],
                "centersIgnore": [0, 1]
            },
            "tolerance": 1
        },
        {
            "name": "Pseudo-V",
            "type": "find",
            "template": {
                "edgesIgnore": [0, 1, 2, 4, 5],
                "centersIgnore": [0, 3]
            },
            "tolerance": 1
        },
        {
            "name": "Pseudo-V",
            "type": "find",
            "template": {
                "edgesIgnore": [0, 1, 2, 3, 4, 5],
                "centersIgnore": [0, 2]
            },
            "tolerance": 1
        },
        {
            "name": "Pseudo-V",
            "type": "find",
            "template": {
                "edgesIgnore": [0, 1, 2, 3, 4],
                "centersIgnore": [0, 1]
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
            "name": "ML4E-L",
            "type": "alg",
            "algs": {
                "source": "file",
                "filePath": "methods/ml4e_l.txt"
            }
        },
        {
            "name": "ML4E-R",
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
        },
        {
            "name": "Fix Pseudo",
            "type": "alg",
            "algs": {
                "source": "inline",
                "algs": ["R", "R'", "L", "L'", "B", "B'"]
            }
        }
    ]
};