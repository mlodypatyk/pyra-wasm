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