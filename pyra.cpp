#include <iostream>
#include <string>
#include <vector>
#include <utility>
#include <unordered_map>
#include <chrono>
#include <emscripten/bind.h>

enum move {U, Uprim, R, Rprim, L, Lprim, B, Bprim, /*rotacje*/ Ur, Urprim, Rr, Rrprim, Lr, Lrprim, Br, Brprim, none};

static move possibleMoves[8] = {U, Uprim, R, Rprim, L, Lprim, B, Bprim};

std::unordered_map<move, std::unordered_map<char, char>> rotations = {
{Ur,     {{'R', 'L'}, {'L', 'B'}, {'B', 'R'}, {'U', 'U'}}},
{Urprim, {{'R', 'B'}, {'B', 'L'}, {'L', 'R'}, {'U', 'U'}}},
{Rr,     {{'L', 'U'}, {'U', 'B'}, {'B', 'L'}, {'R', 'R'}}},
{Rrprim, {{'L', 'B'}, {'B', 'U'}, {'U', 'L'}, {'R', 'R'}}},
{Lr,     {{'U', 'R'}, {'R', 'B'}, {'B', 'U'}, {'L', 'L'}}},
{Lrprim, {{'R', 'U'}, {'U', 'B'}, {'B', 'R'}, {'L', 'L'}}},
{Br,     {{'U', 'L'}, {'L', 'R'}, {'R', 'U'}, {'B', 'B'}}},
{Brprim, {{'U', 'R'}, {'R', 'L'}, {'L', 'U'}, {'B', 'B'}}}
};

static std::unordered_map<std::string, move> inner_move = {
    {"U", U},
    {"U'", Uprim},
    {"R", R},
    {"R'", Rprim},
    {"L", L},
    {"L'", Lprim},
    {"B", B},
    {"B'", Bprim},
    {"[u]", Ur},
    {"[u']", Urprim},
    {"[r]", Rr},
    {"[r']", Rrprim},
    {"[l]", Lr},
    {"[l]", Lrprim},
    {"[b]", Br},
    {"[b']", Brprim}
};

static std::unordered_map<move, std::string> letter_move = {
    {U, "U"},
    {Uprim, "U'"},
    {R, "R"},
    {Rprim, "R'"},
    {L, "L"},
    {Lprim, "L'"},
    {B, "B"},
    {Bprim, "B'"},
    {Ur, "[u]"},
    {Urprim, "[u']"},
    {Rr, "[r]"},
    {Rrprim, "[r']"},
    {Lr, "[l]"},
    {Lrprim, "[l']"},
    {Br, "[b]"},
    {Brprim, "[b']"},
    {none, "none"}
};

static std::unordered_map<move, move> inverse_move = {
    {U, Uprim},
    {Uprim, U},
    {R, Rprim},
    {Rprim, R},
    {L, Lprim},
    {Lprim, L},
    {B, Bprim},
    {Bprim, B},
    {Ur, Urprim},
    {Urprim, Ur},
    {Rr, Rrprim},
    {Rrprim, Rr},
    {Lr, Lrprim},
    {Lrprim, Lr},
    {Br, Brprim},
    {Brprim, Br}
};

move rotateMove(move bodyMove, move rotation){
    if(rotation == none){
        return bodyMove;
    }
    std::string move = letter_move[bodyMove];
    move[0] = rotations[rotation][move[0]];
    return inner_move[move];

}

std::vector<move> rotateMoves(std::vector<move> bodyMoves, move rotation){
    std::vector<move> newMoves;
    for(auto move : bodyMoves){
        newMoves.push_back(rotateMove(move, rotation));
    }
    return newMoves;
}

void shift3(int &a, int &b, int &c) {
    int tmp = a;
    a = b;
    b = c;
    c = tmp;
}

class Pyraminx{
    public:
    int edges[6] = {0, 1, 2, 3, 4, 5}; // LF FR RB DF RD DL
    bool orientation[6] = {true, true, true, true, true, true}; // true = up, false = down
    int centers[4] = {0, 0, 0, 0}; // U R L B

    Pyraminx() {}
    Pyraminx(std::string initial_state) {
        size_t pos = 0;
        while ((pos = initial_state.find(" ")) != std::string::npos) {
            this->makeMove(inner_move[initial_state.substr(0, pos)]);
            initial_state.erase(0, pos + 1);
        }
        if (initial_state != "") {
            this->makeMove(inner_move[initial_state]);
        }
    }
    Pyraminx(std::vector<move> scr_moves){
        for(auto e: scr_moves){
            this->makeMove(e);
        }
    }
    Pyraminx(std::vector<int> edgesToIgnore, std::vector<int> centersToIgnore){
        for(auto i : edgesToIgnore){
            this->edges[i] = -1;
        }
        for(auto i: centersToIgnore){
            this->centers[i] = -1;
        }
    }

    bool operator==(const Pyraminx& other)
    {
        return edges == other.edges && orientation == other.orientation && centers == other.centers;
    }

    bool match(const Pyraminx& other){
        for(int i=0;i<6;i++){
            if((edges[i] != other.edges[i] || orientation[i] != other.orientation[i]) && (edges[i] != -1 && other.edges[i] != -1)){
                return false;
            }
        }
        for(int i=0;i<4;i++){
            if(centers[i] != other.centers[i] && (centers[i] != -1 && other.centers[i] != -1)){
                return false;
            }
        }
        return true;
    }

    int hash() {
        int hash = 13;
        for(int i=0;i<6;i++){
            hash = hash * 31 + edges[i];
            hash = hash * 2 + orientation[i];
        }
        for(int i=0;i<4;i++){
            hash = hash * 17 + centers[i];
        }
        return hash;
    }

    void makeMove (move move){
    switch(move){
        case U:
        {
            if(centers[0] != -1){
                centers[0] = (centers[0] + 1)%3;
            }
            shift3(edges[0], edges[1], edges[2]);
            bool extra_orientation = orientation[0];
            orientation[0] = orientation[1];
            orientation[1] = orientation[2];
            orientation[2] = extra_orientation;
            break;
        }
        case Uprim:
        {
            if(centers[0] != -1){
                centers[0] = (centers[0] - 1 + 3)%3;
            }
            shift3(edges[2], edges[1], edges[0]);
            bool extra_orientation = orientation[2];
            orientation[2] = orientation[1];
            orientation[1] = orientation[0];
            orientation[0] = extra_orientation;
            break;
        }
        case R:
        {
            if(centers[1] != -1){
                centers[1] = (centers[1] + 1)%3;
            }
            shift3(edges[1], edges[3], edges[4]);
            bool extra_orientation = orientation[1];
            orientation[1] = orientation[3];
            orientation[3] = orientation[4];
            orientation[4] = extra_orientation;
            break;
        }
        case Rprim:
        {
            if(centers[1] != -1){
                centers[1] = (centers[1] - 1 + 3)%3;
            }
            shift3(edges[4], edges[3], edges[1]);
            bool extra_orientation = orientation[4];
            orientation[4] = orientation[3];
            orientation[3] = orientation[1];
            orientation[1] = extra_orientation;
            break;
        }
        case L:
        {
            if(centers[2] != -1){
                centers[2] = (centers[2] + 1)%3;
            }
            shift3(edges[0], edges[5], edges[3]);
            bool extra_orientation = orientation[0];
            orientation[0] = orientation[5];
            orientation[5] = !orientation[3];
            orientation[3] = !extra_orientation;
            break;
        }
        case Lprim:
        {
            if(centers[2] != -1){
                centers[2] = (centers[2] - 1 + 3)%3;
            }
            shift3(edges[0], edges[3], edges[5]);
            bool extra_orientation = orientation[0];
            orientation[0] = !orientation[3];
            orientation[3] = !orientation[5];
            orientation[5] = extra_orientation;
            break;
        }
        case B:
        {
            if(centers[3] != -1){
                centers[3] = (centers[3] + 1)%3;
            }
            shift3(edges[2], edges[4], edges[5]);
            bool extra_orientation = orientation[2];
            orientation[2] = !orientation[4];
            orientation[4] = orientation[5];
            orientation[5] = !extra_orientation;
            break;
        }
        case Bprim:
        {
            if(centers[3] != -1){
                centers[3] = (centers[3] - 1 + 3)%3;
            }
            shift3(edges[2], edges[5], edges[4]);
            bool extra_orientation = orientation[2];
            orientation[2] = !orientation[5];
            orientation[5] = orientation[4];
            orientation[4] = !extra_orientation;
            break;
        }
        default:
            break;
    }
}
};

std::ostream &operator<<(std::ostream &os, const Pyraminx &pyra) {
    for (auto edge : pyra.edges) {
        os << edge;
    } os << "\n";
    for (auto orient : pyra.orientation) {
        os << orient;
    } os << "\n";
    for (auto center : pyra.centers) {
        os << center;
    } os << "\n";
    return os;
}

std::string getLetterMove(move move){ // emcc sie nie lubił z tym 
    return letter_move[move];
}

move getInnerMove(const std::string move){
    return inner_move[move];
}

move getInverse(move move){
    return inverse_move[move];
}

std::vector<move> createSolution(std::vector<move> start, std::vector<move> end){
    std::vector<move> result = start;
    for (int i = end.size()-1; i>=0;i--) {
        // kto to kurwa jest marek mostowiak
        result.push_back(getInverse(end.at(i)));
    }
    return result;
}

Pyraminx createSolved() {
    return Pyraminx();
}

void check_if_solved(bool &fullMatch, std::vector<std::pair<std::vector<move>,Pyraminx>> &startStates, 
                                      std::vector<std::pair<std::vector<move>,Pyraminx>> &endStates, 
                                      std::vector<std::vector<move>> &solutions,
                                      std::chrono::duration<double> &elapsed) {
    auto start = std::chrono::high_resolution_clock::now();
    if(fullMatch){
        for(auto state1: startStates){
            for(auto state2: endStates){
                if(state1.second.match(state2.second)){
                    solutions.push_back(createSolution(state1.first, state2.first));
                }
            }
        }
    }else{
        std::unordered_map<int, std::vector<std::vector<move>>> startMap;
        for (auto state : startStates) {
            int hash = state.second.hash();
            if (startMap.find(hash) == startMap.end()) {
                startMap[hash] = std::vector<std::vector<move>>();
            } 
            startMap[hash].push_back(state.first);
        }

        for (auto state : endStates) {
            int hash = state.second.hash();
            if (startMap.find(hash) != startMap.end()) {
                for (std::vector<move> start : startMap[hash]) {
                    solutions.push_back(createSolution(start, state.first));
                }
            }
        }
    }
    elapsed = elapsed + std::chrono::high_resolution_clock::now() - start;
}

void moveStates(std::vector<std::pair<std::vector<move>,Pyraminx>> &states) {
    std::vector<std::pair<std::vector<move>,Pyraminx>> temp;
    for (auto state : states) {
        for (auto possibleMove : possibleMoves) {
            move lastMove = none;
            if (state.first.size() > 0) {
                lastMove = state.first.at(state.first.size() - 1);
            }
            if (lastMove != possibleMove && lastMove != getInverse(possibleMove)) {
                std::vector<move> moves = state.first;
                moves.push_back(possibleMove);
                Pyraminx newState = state.second;
                newState.makeMove(possibleMove);
                temp.push_back(std::make_pair(moves, newState));
            }
        }
    }
    states = temp;
}

std::vector<std::vector<move>> findSolution(std::vector<move> scramble, std::vector<Pyraminx> end, int tolerance, bool fullMatch){
    std::vector<std::vector<move>> solutions;
    std::vector<std::pair<std::vector<move>,Pyraminx>> startStates;
    std::vector<std::pair<std::vector<move>,Pyraminx>> endStates;
    for(auto state : end){
        endStates.push_back(std::make_pair(std::vector<move>(), state));
    }

    Pyraminx start = Pyraminx(scramble);

    if(fullMatch){
        move firstRotation[] = {none, Rr, Rrprim, Lr};
        move secondRotation[] = {none, Br, Brprim};
        for(auto rot1 : firstRotation){
            for(auto rot2: secondRotation){
                std::vector<move> preMoves;
                    if(rot1 != none){
                    preMoves.push_back(rot1);
                }
                if(rot2 != none){
                    preMoves.push_back(rot2);
                }
                Pyraminx current = Pyraminx(rotateMoves(rotateMoves(scramble, rot1), rot2));
                startStates.push_back(std::make_pair(preMoves, current));
            }
        }
    }else{
        startStates.push_back(std::make_pair(std::vector<move>(), start));
    }

    int moveCount = 0;
    std::chrono::duration<double> elapsed = std::chrono::duration<double>::zero();
    while (tolerance + 1 > 0){
        // Ruch z przodu
        moveStates(startStates);
        check_if_solved(fullMatch, startStates, endStates, solutions, elapsed);

        ++moveCount;
        if(moveCount == 11 ) break; // Gods number
        if(!solutions.empty()) --tolerance;
        if(tolerance < 0) break;

        moveStates(endStates);
        check_if_solved(fullMatch, startStates, endStates, solutions, elapsed);
        
        ++moveCount;
        if(!solutions.empty()) --tolerance;
    }
    std::cout << "Elapsed time: " << elapsed.count() << "s" << std::endl;
    return solutions;
}

int main() {
    // U B U L' R' L U R' B'
    //std::vector<move> scramble = {U, B, U, Lprim, Rprim, L, U, Rprim, Bprim};
    //B' L B' U' L' B L R'
    //std::vector<move> scramble = {Bprim, L, Bprim, Uprim, Lprim, B, L, Rprim};
    /* B R' U L R B U' B' l r' u'*/
    if(false){
        std::vector<move> scramble = {B, Rprim, U, L, R, B, Uprim, Bprim};


        std::vector<Pyraminx> solved = {Pyraminx()};

        Pyraminx V = Pyraminx(std::vector<int>{0,1,2,3}, std::vector<int>{0});

        std::vector<Pyraminx> vTemplate = {V};

        //std::vector<std::vector<move>> solutions = findSolution(scramble, solved, 0, false);
        std::vector<std::vector<move>> solutions = findSolution(scramble, vTemplate, 2, true);
        
        for (std::vector<move> solution : solutions){
            for (move move : solution){
                std::cout << getLetterMove(move) << " ";
            }
            std::cout << "\n";
        }
    }
}
using namespace emscripten;

EMSCRIPTEN_BINDINGS(my_module) {
    value_array<std::array<int, 4>>("array_int_4")
        .element(emscripten::index<0>())
        .element(emscripten::index<1>())
        .element(emscripten::index<2>())
        .element(emscripten::index<3>());
    
    value_array<std::array<int, 6>>("array_int_6")
        .element(emscripten::index<0>())
        .element(emscripten::index<1>())
        .element(emscripten::index<2>())
        .element(emscripten::index<3>())
        .element(emscripten::index<4>())
        .element(emscripten::index<5>());

    value_array<std::array<bool, 6>>("array_bool_6")
        .element(emscripten::index<0>())
        .element(emscripten::index<1>())
        .element(emscripten::index<2>())
        .element(emscripten::index<3>())
        .element(emscripten::index<4>())
        .element(emscripten::index<5>());

    register_vector<move>("VectorMove");
    register_vector<std::vector<move>>("VectorVectorMove");
    register_vector<Pyraminx>("VectorPyraminx");
    register_vector<int>("VectorInt");


    class_<Pyraminx>("Pyraminx")
    .constructor<>()
    .constructor<std::string>()
    .constructor<std::vector<int>, std::vector<int>>()
    .function("makeMove", &Pyraminx::makeMove)
    .function("hash", &Pyraminx::hash)
    .function("equals", &Pyraminx::match);

    enum_<move>("move")
    .value("R", move::R)
    .value("Rprim", move::Rprim)
    .value("U", move::U)
    .value("Uprim", move::Uprim)
    .value("L", move::L)
    .value("Lprim", move::Lprim)
    .value("B", move::B)
    .value("Bprim", move::Bprim)
    .value("Rr", move::Rr)
    .value("Rrprim", move::Rrprim)
    .value("Ur", move::Ur)
    .value("Urprim", move::Urprim)
    .value("Lr", move::Lr)
    .value("Lrprim", move::Lrprim)
    .value("Br", move::Br)
    .value("Brprim", move::Brprim)
    .value("none", move::none);

    function("createSolved", &createSolved);
    function("findSolution", &findSolution);
    function("getLetterMove", &getLetterMove);
    function("getInverse", &getInverse);
    function("getInnerMove", &getInnerMove);
    function("rotateMove", &rotateMove);

}
