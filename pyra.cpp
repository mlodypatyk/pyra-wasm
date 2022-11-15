#include <iostream>
#include <string>
#include <vector>
#include <utility>
#include <unordered_map>
#include <emscripten/bind.h>

static enum bodyMove {U, Uprim, R, Rprim, L, Lprim, B, Bprim, none}; // z fiutas

static bodyMove possibleMoves[8] = {U, Uprim, R, Rprim, L, Lprim, B, Bprim};

static std::map<std::string, bodyMove> inner_move = {
    {"U", U},
    {"U'", Uprim},
    {"R", R},
    {"R'", Rprim},
    {"L", L},
    {"L'", Lprim},
    {"B", B},
    {"B'", Bprim}
};

class Pyraminx{
    public:
    int edges[6]; // LF FR RB DF RD DL
    bool orientation[6]; 
    int centers[4]; // U R L B

    bool operator==(const Pyraminx& other)
    {
        for(int i=0;i<6;i++){
            if(edges[i] != other.edges[i] || orientation[i] != other.orientation[i]){
                return false;
            }
        }
        for(int i=0;i<4;i++){
            if(centers[i] != other.centers[i]){
                return false;
            }
        }
        return true;
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
            int extra_edge = edges[0];
            edges[0] = edges[1];
            edges[1] = edges[2];
            edges[2] = extra_edge;
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
            int extra_edge = edges[2];
            edges[2] = edges[1];
            edges[1] = edges[0];
            edges[0] = extra_edge;
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
            int extra_edge = edges[1];
            edges[1] = edges[3];
            edges[3] = edges[4];
            edges[4] = extra_edge;
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
            int extra_edge = edges[4];
            edges[4] = edges[3];
            edges[3] = edges[1];
            edges[1] = extra_edge;
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
            int extra_edge = edges[0];
            edges[0] = edges[5];
            edges[5] = edges[3];
            edges[3] = extra_edge;
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
            int extra_edge = edges[0];
            edges[0] = edges[3];
            edges[3] = edges[5];
            edges[5] = extra_edge;
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
            int extra_edge = edges[2];
            edges[2] = edges[4];
            edges[4] = edges[5];
            edges[5] = extra_edge;
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
            int extra_edge = edges[2];
            edges[2] = edges[5];
            edges[5] = edges[4];
            edges[4] = extra_edge;
            bool extra_orientation = orientation[2];
            orientation[2] = !orientation[5];
            orientation[5] = orientation[4];
            orientation[4] = !extra_orientation;
            break;
        }
    }
}
};




Pyraminx createSolved(){
    Pyraminx current;
    for(int i=0;i<6;i++){
        current.edges[i] = i;
        current.orientation[i] = true;
    }
    for(int i=0;i<4;i++){
        current.centers[i] = 0;
    }

    return current;
}

void printState(Pyraminx *cube){
    for(int i=0;i<6;i++){
        std::cout << cube->edges[i];
    }
    std::cout << "\n";
    for(int i=0;i<6;i++){
        std::cout << cube->orientation[i];
    }
    std::cout << "\n";
    for(int i=0;i<4;i++){
        std::cout << cube->centers[i];
    }
    std::cout << "\n";
}

std::string getLetterMove(bodyMove move){
    switch(move) {
        case U:
            return "U";break;
        case Uprim:
            return "U'";break;
        case R:
            return "R";break;
        case Rprim:
            return "R'";break;
        case L:
            return "L";break;
        case Lprim:
            return "L'";break;
        case B:
            return "B";break;
        case Bprim:
            return "B'";break;
        default:
            return "error";break;
    }
}

bodyMove getInnerMove(std::string move){
    return inner_move(move);
}

bodyMove getInverse(bodyMove move){
    switch(move) {
        case U:
            return Uprim;
            break;
        case Uprim:
            return U;
            break;
        case R:
            return Rprim;
            break;
        case Rprim:
            return R;
            break;
        case L:
            return Lprim;
            break;
        case Lprim:
            return L;
            break;
        case B:
            return Bprim;
            break;
        case Bprim:
            return B;
            break;
        default:
            return none;
            break;
    }
}

std::vector<bodyMove> createSolution(std::vector<bodyMove> start, std::vector<bodyMove> end){
    std::vector<bodyMove> result = start;
    for(int i = end.size()-1;i>=0;i--){
        result.push_back(getInverse(end.at(i)));
    }
    return result;
}

void check_if_solved(bool &fullMatch, std::vector<std::pair<std::vector<bodyMove>,Pyraminx>> &startStates, 
                                      std::vector<std::pair<std::vector<bodyMove>,Pyraminx>> &endStates, 
                                      std::vector<std::vector<bodyMove>> &solutions,
                                      std::chrono::duration<double> &elapsed) {
    // Check
    auto start = std::chrono::high_resolution_clock::now();

    std::unordered_map<int, std::vector<std::vector<bodyMove>>> startMap;
    for (std::pair<std::vector<bodyMove>, Pyraminx> state : startStates) {
        int hash = state.second.hash();
        if (startMap.find(hash) == startMap.end()) {
            startMap[hash] = std::vector<std::vector<bodyMove>>();
        } 
        startMap[hash].push_back(state.first);
    }

    for (std::pair<std::vector<bodyMove>, Pyraminx> state : endStates) {
        int hash = state.second.hash();
        if (startMap.find(hash) != startMap.end()) {
            for (std::vector<bodyMove> start : startMap[hash]) {
                solutions.push_back(createSolution(start, state.first));
            }
        }
    }
    elapsed = elapsed + std::chrono::high_resolution_clock::now() - start;
} 

std::vector<std::vector<bodyMove>> findSolution(Pyraminx start, Pyraminx end, int tolerance, bool fullMatch){
    std::vector<std::vector<bodyMove>> solutions;
    std::vector<std::pair<std::vector<bodyMove>,Pyraminx>> startStates;
    std::pair<std::vector<bodyMove>, Pyraminx> firstState;
    //std::vector<Pyraminx> startPreviousStates;
    firstState.first = std::vector<bodyMove>();
    firstState.second = start;
    startStates.push_back(firstState);
    //startPreviousStates.push_back(start);
    std::vector<std::pair<std::vector<bodyMove>,Pyraminx>> endStates;
    //std::vector<Pyraminx> endPreviousStates;
    std::pair<std::vector<bodyMove>, Pyraminx> lastState;
    lastState.first = std::vector<bodyMove>();
    lastState.second = end;
    endStates.push_back(lastState);
    //endPreviousStates.push_back(end);
    int moveCount = 0;
    int cutoff = 12;
    std::chrono::duration<double> elapsed = std::chrono::duration<double>::zero();
    while (tolerance + 1 > 0){
        // Ruch z przodu
        std::vector<std::pair<std::vector<bodyMove>,Pyraminx>> startStatesNew;
        for (int i = 0; i<startStates.size();i++){
            for (int j=0;j<8;j++){
                std::vector<bodyMove> previousMoves = startStates.at(i).first;
                bodyMove lastMove;
                if(previousMoves.size()==0){
                    lastMove = none;
                }else{
                    lastMove = previousMoves.at(previousMoves.size()-1);
                }
                if(lastMove != possibleMoves[j] && lastMove != getInverse(possibleMoves[j])){
                    Pyraminx newState = startStates.at(i).second;
                    newState.makeMove(possibleMoves[j]);
                    bool isIn = false;
                    std::vector<bodyMove> moves = previousMoves;
                    moves.push_back(possibleMoves[j]);
                    std::pair<std::vector<bodyMove>, Pyraminx> currState;
                    currState.first = moves;
                    currState.second = newState;
                    startStatesNew.push_back(currState);
                    
                }
            }
        }
        startStates = startStatesNew;
        moveCount += 1;
        
        check_if_solved(fullMatch, startStates, endStates, solutions, elapsed);

        if(moveCount == 11 ){
            break; // Gods number
        }
        
        if(!solutions.empty()){
            tolerance -=1;
        }
        if(tolerance < 0){
            break;
        }
        // Ruch z tylu
        std::vector<std::pair<std::vector<bodyMove>,Pyraminx>> endStatesNew;
        for (int i = 0; i<endStates.size();i++){
            for (int j=0;j<8;j++){
                std::vector<bodyMove> previousMoves = endStates.at(i).first;
                bodyMove lastMove;
                if(previousMoves.size()==0){
                    lastMove = none;
                }else{
                    lastMove = previousMoves.at(previousMoves.size()-1);
                }
                if(lastMove != possibleMoves[j] && lastMove != getInverse(possibleMoves[j])){
                    Pyraminx newState = endStates.at(i).second;
                    newState.makeMove(possibleMoves[j]);
                    std::vector<bodyMove> moves = previousMoves;
                    moves.push_back(possibleMoves[j]);
                    std::pair<std::vector<bodyMove>, Pyraminx> currState;
                    currState.first = moves;
                    currState.second = newState;
                    endStatesNew.push_back(currState);
                    
                }
            }
        }
        endStates = endStatesNew;
        moveCount += 1;
        // Check 2
        check_if_solved(fullMatch, startStates, endStates, solutions, elapsed);
        if(!solutions.empty()){
            tolerance -=1;
        }
    }
    std::cout << "Elapsed time: " << elapsed.count() << "s" << std::endl;
    return solutions;
}

int main() {
    Pyraminx solved = createSolved();
    solved.makeMove(U);
    solved.makeMove(B); 
    solved.makeMove(U);
    solved.makeMove(Lprim);
    solved.makeMove(Rprim);
    solved.makeMove(L);
    solved.makeMove(U);
    solved.makeMove(Rprim);
    solved.makeMove(Bprim);

    std::vector<std::vector<bodyMove>> solutions = findSolution(solved, createSolved(), 2, false);
    
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
