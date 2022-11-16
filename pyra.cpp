#include <iostream>
#include <string>
#include <vector>
#include <utility>
#include <unordered_map>
#include <chrono>
#include <emscripten/bind.h>

enum bodyMove {U, Uprim, R, Rprim, L, Lprim, B, Bprim, none}; // z fiutas

static bodyMove possibleMoves[8] = {U, Uprim, R, Rprim, L, Lprim, B, Bprim};

static std::unordered_map<std::string, bodyMove> inner_move = {
    {"U", U},
    {"U'", Uprim},
    {"R", R},
    {"R'", Rprim},
    {"L", L},
    {"L'", Lprim},
    {"B", B},
    {"B'", Bprim}
};

static std::unordered_map<bodyMove, std::string> letter_move = {
    {U, "U"},
    {Uprim, "U'"},
    {R, "R"},
    {Rprim, "R'"},
    {L, "L"},
    {Lprim, "L'"},
    {B, "B"},
    {Bprim, "B'"},
    {none, "none"}
};

static std::unordered_map<bodyMove, bodyMove> inverse_move = {
    {U, Uprim},
    {Uprim, U},
    {R, Rprim},
    {Rprim, R},
    {L, Lprim},
    {Lprim, L},
    {B, Bprim},
    {Bprim, B}
};

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

    void makeMove (bodyMove move){
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

std::string getLetterMove(bodyMove move){ // emcc sie nie lubił z tym 
    return letter_move[move];
}

bodyMove getInnerMove(std::string &move){
    return inner_move[move];
}

bodyMove getInverse(bodyMove &move){
    return inverse_move[move];
}

std::vector<bodyMove> createSolution(std::vector<bodyMove> start, std::vector<bodyMove> end){
    std::vector<bodyMove> result = start;
    for (auto e : end) {
        result.push_back(getInverse(e));
    }
    return result;
}

Pyraminx createSolved() {
    return Pyraminx();
}

void check_if_solved(bool &fullMatch, std::vector<std::pair<std::vector<bodyMove>,Pyraminx>> &startStates, 
                                      std::vector<std::pair<std::vector<bodyMove>,Pyraminx>> &endStates, 
                                      std::vector<std::vector<bodyMove>> &solutions,
                                      std::chrono::duration<double> &elapsed) {
    auto start = std::chrono::high_resolution_clock::now();

    std::unordered_map<int, std::vector<std::vector<bodyMove>>> startMap;
    for (auto state : startStates) {
        int hash = state.second.hash();
        if (startMap.find(hash) == startMap.end()) {
            startMap[hash] = std::vector<std::vector<bodyMove>>();
        } 
        startMap[hash].push_back(state.first);
    }

    for (auto state : endStates) {
        int hash = state.second.hash();
        if (startMap.find(hash) != startMap.end()) {
            for (std::vector<bodyMove> start : startMap[hash]) {
                solutions.push_back(createSolution(start, state.first));
            }
        }
    }
    elapsed = elapsed + std::chrono::high_resolution_clock::now() - start;
}

void move(std::vector<std::pair<std::vector<bodyMove>,Pyraminx>> &states) {
    std::vector<std::pair<std::vector<bodyMove>,Pyraminx>> temp;
    for (auto state : states) {
        for (auto possibleMove : possibleMoves) {
            bodyMove lastMove = none;
            if (state.first.size() > 0) {
                lastMove = state.first.at(state.first.size() - 1);
            }
            if (lastMove != possibleMove && lastMove != getInverse(possibleMove)) {
                std::vector<bodyMove> moves = state.first;
                moves.push_back(possibleMove);
                Pyraminx newState = state.second;
                newState.makeMove(possibleMove);
                temp.push_back(std::make_pair(moves, newState));
            }
        }
    }
    states = temp;
}

std::vector<std::vector<bodyMove>> findSolution(Pyraminx &start, Pyraminx end, int tolerance, bool fullMatch){
    std::vector<std::vector<bodyMove>> solutions;
    std::vector<std::pair<std::vector<bodyMove>,Pyraminx>> startStates = {std::make_pair(std::vector<bodyMove>(), start)};
    std::vector<std::pair<std::vector<bodyMove>,Pyraminx>> endStates = {std::make_pair(std::vector<bodyMove>(), end)};

    int moveCount = 0;
    std::chrono::duration<double> elapsed = std::chrono::duration<double>::zero();
    while (tolerance + 1 > 0){
        // Ruch z przodu
        move(startStates);
        check_if_solved(fullMatch, startStates, endStates, solutions, elapsed);

        ++moveCount;
        if(moveCount == 11 ) break; // Gods number
        if(!solutions.empty()) --tolerance;
        if(tolerance < 0) break;

        move(endStates);
        check_if_solved(fullMatch, startStates, endStates, solutions, elapsed);
        
        ++moveCount;
        if(!solutions.empty()) --tolerance;
    }
    std::cout << "Elapsed time: " << elapsed.count() << "s" << std::endl;
    return solutions;
}

int main() {
    Pyraminx solved("U B U L' R' L U R' B'");

    std::vector<std::vector<bodyMove>> solutions = findSolution(solved, Pyraminx(), 2, false);
    
    for (std::vector<bodyMove> solution : solutions){
        for (bodyMove move : solution){
            std::cout << getLetterMove(move) << " ";
        }
        std::cout << "\n";
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

    register_vector<bodyMove>("VectorBodyMove");
    register_vector<std::vector<bodyMove>>("VectorVectorBodyMove");

    class_<Pyraminx>("Pyraminx")
    .function("makeMove", &Pyraminx::makeMove);

    enum_<bodyMove>("bodyMove")
    .value("R", bodyMove::R)
    .value("Rprim", bodyMove::Rprim)
    .value("U", bodyMove::U)
    .value("Uprim", bodyMove::Uprim)
    .value("L", bodyMove::L)
    .value("Lprim", bodyMove::Lprim)
    .value("B", bodyMove::B)
    .value("Bprim", bodyMove::Bprim)
    .value("none", bodyMove::none);

    function("createSolved", &createSolved);
    function("findSolution", &findSolution);
    function("getLetterMove", &getLetterMove);
}
