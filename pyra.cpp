#include <iostream>
#include <string>
#include <vector>
#include <utility>

struct Pyraminx{
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
};

enum bodyMove {U, Uprim, R, Rprim, L, Lprim, B, Bprim, none};

bodyMove possibleMoves[8] = {U, Uprim, R, Rprim, L, Lprim, B, Bprim};

void makeMove (Pyraminx *cube, bodyMove move){
    switch(move){
        case U:
        {
            if(cube->centers[0] != -1){
                cube->centers[0] = (cube->centers[0] + 1)%3;
            }
            int extra_edge = cube->edges[0];
            cube->edges[0] = cube->edges[1];
            cube->edges[1] = cube->edges[2];
            cube->edges[2] = extra_edge;
            bool extra_orientation = cube->orientation[0];
            cube->orientation[0] = cube->orientation[1];
            cube->orientation[1] = cube->orientation[2];
            cube->orientation[2] = extra_orientation;
            break;
        }
        case Uprim:
        {
            if(cube->centers[0] != -1){
                cube->centers[0] = (cube->centers[0] - 1 + 3)%3;
            }
            int extra_edge = cube->edges[2];
            cube->edges[2] = cube->edges[1];
            cube->edges[1] = cube->edges[0];
            cube->edges[0] = extra_edge;
            bool extra_orientation = cube->orientation[2];
            cube->orientation[2] = cube->orientation[1];
            cube->orientation[1] = cube->orientation[0];
            cube->orientation[0] = extra_orientation;
            break;
        }
        case R:
        {
            if(cube->centers[1] != -1){
                cube->centers[1] = (cube->centers[1] + 1)%3;
            }
            int extra_edge = cube->edges[1];
            cube->edges[1] = cube->edges[3];
            cube->edges[3] = cube->edges[4];
            cube->edges[4] = extra_edge;
            bool extra_orientation = cube->orientation[1];
            cube->orientation[1] = cube->orientation[3];
            cube->orientation[3] = cube->orientation[4];
            cube->orientation[4] = extra_orientation;
            break;
        }
        case Rprim:
        {
            if(cube->centers[1] != -1){
                cube->centers[1] = (cube->centers[1] - 1 + 3)%3;
            }
            int extra_edge = cube->edges[4];
            cube->edges[4] = cube->edges[3];
            cube->edges[3] = cube->edges[1];
            cube->edges[1] = extra_edge;
            bool extra_orientation = cube->orientation[4];
            cube->orientation[4] = cube->orientation[3];
            cube->orientation[3] = cube->orientation[1];
            cube->orientation[1] = extra_orientation;
            break;
        }
        case L:
        {
            if(cube->centers[2] != -1){
                cube->centers[2] = (cube->centers[2] + 1)%3;
            }
            int extra_edge = cube->edges[0];
            cube->edges[0] = cube->edges[5];
            cube->edges[5] = cube->edges[3];
            cube->edges[3] = extra_edge;
            bool extra_orientation = cube->orientation[0];
            cube->orientation[0] = cube->orientation[5];
            cube->orientation[5] = !cube->orientation[3];
            cube->orientation[3] = !extra_orientation;
            break;
        }
        case Lprim:
        {
            if(cube->centers[2] != -1){
                cube->centers[2] = (cube->centers[2] - 1 + 3)%3;
            }
            int extra_edge = cube->edges[0];
            cube->edges[0] = cube->edges[3];
            cube->edges[3] = cube->edges[5];
            cube->edges[5] = extra_edge;
            bool extra_orientation = cube->orientation[0];
            cube->orientation[0] = !cube->orientation[3];
            cube->orientation[3] = !cube->orientation[5];
            cube->orientation[5] = extra_orientation;
            break;
        }
        case B:
        {
            if(cube->centers[3] != -1){
                cube->centers[3] = (cube->centers[3] + 1)%3;
            }
            int extra_edge = cube->edges[2];
            cube->edges[2] = cube->edges[4];
            cube->edges[4] = cube->edges[5];
            cube->edges[5] = extra_edge;
            bool extra_orientation = cube->orientation[2];
            cube->orientation[2] = !cube->orientation[4];
            cube->orientation[4] = cube->orientation[5];
            cube->orientation[5] = !extra_orientation;
            break;
        }
        case Bprim:
        {
            if(cube->centers[3] != -1){
                cube->centers[3] = (cube->centers[3] - 1 + 3)%3;
            }
            int extra_edge = cube->edges[2];
            cube->edges[2] = cube->edges[5];
            cube->edges[5] = cube->edges[4];
            cube->edges[4] = extra_edge;
            bool extra_orientation = cube->orientation[2];
            cube->orientation[2] = !cube->orientation[5];
            cube->orientation[5] = cube->orientation[4];
            cube->orientation[4] = !extra_orientation;
            break;
        }
    }
}
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
    }
}

bodyMove getInnerMove(std::string move){
    if(move=="U"){return U;};
    if(move=="U'"){return Uprim;};
    if(move=="R"){return R;};
    if(move=="R'"){return Rprim;};
    if(move=="L"){return L;};
    if(move=="L'"){return Lprim;};
    if(move=="B"){return B;};
    if(move=="B'"){return Bprim;};
}

bodyMove getInverse(bodyMove move){
    switch(move) {
        case U:
            return Uprim;break;
        case Uprim:
            return U;break;
        case R:
            return Rprim;break;
        case Rprim:
            return R;break;
        case L:
            return Lprim;break;
        case Lprim:
            return L;break;
        case B:
            return Bprim;break;
        case Bprim:
            return B;break;
    }
}

std::vector<bodyMove> createSolution(std::vector<bodyMove> start, std::vector<bodyMove> end){
    std::vector<bodyMove> result = start;
    for(int i = end.size()-1;i>=0;i--){
        result.push_back(getInverse(end.at(i)));
    }
    return result;
}

std::vector<std::vector<bodyMove>> findSolution(Pyraminx start, Pyraminx end, int tolerance, bool fullMatch){
    std::vector<std::vector<bodyMove>> solutions;
    std::vector<std::pair<std::vector<bodyMove>,Pyraminx>> startStates;
    std::pair<std::vector<bodyMove>, Pyraminx> firstState;
    std::vector<Pyraminx> startPreviousStates;
    firstState.first = std::vector<bodyMove>();
    firstState.second = start;
    startStates.push_back(firstState);
    startPreviousStates.push_back(start);
    std::vector<std::pair<std::vector<bodyMove>,Pyraminx>> endStates;
    std::vector<Pyraminx> endPreviousStates;
    std::pair<std::vector<bodyMove>, Pyraminx> lastState;
    lastState.first = std::vector<bodyMove>();
    lastState.second = end;
    endStates.push_back(lastState);
    endPreviousStates.push_back(end);
    int moveCount = 0;
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
                    makeMove(&newState, possibleMoves[j]);
                    bool isIn = false;
                    for (int k=0;k<startPreviousStates.size();k++){
                        if(newState==startPreviousStates.at(k)){
                            isIn = true;
                            break;
                        }
                    }
                    if(!isIn){
                        std::vector<bodyMove> moves = previousMoves;
                        moves.push_back(possibleMoves[j]);
                        startPreviousStates.push_back(newState);
                        std::pair<std::vector<bodyMove>, Pyraminx> currState;
                        currState.first = moves;
                        currState.second = newState;
                        startStatesNew.push_back(currState);
                    }
                }
            }
        }
        startStates = startStatesNew;
        moveCount += 1;
        // Check
        for(int i = 0; i<startStates.size();i++){
            for(int j =0; j<endStates.size();j++){
                if((fullMatch && startStates.at(i).second == endStates.at(j).second)
                || (!fullMatch && startStates.at(i).second.match(endStates.at(j).second))){
                    solutions.push_back(createSolution(startStates.at(i).first, endStates.at(j).first));
                }
            }
        }
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
                    makeMove(&newState, possibleMoves[j]);
                    bool isIn = false;
                    for (int k=0;k<endPreviousStates.size();k++){
                        if(newState==endPreviousStates.at(k)){
                            isIn = true;
                            break;
                        }
                    }
                    if(!isIn){
                        std::vector<bodyMove> moves = previousMoves;
                        moves.push_back(possibleMoves[j]);
                        endPreviousStates.push_back(newState);
                        std::pair<std::vector<bodyMove>, Pyraminx> currState;
                        currState.first = moves;
                        currState.second = newState;
                        endStatesNew.push_back(currState);
                    }
                }
            }
        }
        endStates = endStatesNew;
        moveCount += 1;
        // Check 2
        for(int i = 0; i<startStates.size();i++){
            for(int j =0; j<endStates.size();j++){
                if((fullMatch && startStates.at(i).second == endStates.at(j).second)
                || (!fullMatch && startStates.at(i).second.match(endStates.at(j).second))){
                    solutions.push_back(createSolution(startStates.at(i).first, endStates.at(j).first));
                }
            }
        }
        if(!solutions.empty()){
            tolerance -=1;
        }
    }
    return solutions;
}

int main() {
    Pyraminx state1 = createSolved();
    state1.edges[0] = 5;
    state1.edges[1] = 2;
    state1.edges[2] = 1; 
    state1.edges[3] = 4;
    state1.edges[4] = 0;
    state1.edges[5] = 3;
    state1.centers[0] = 2;
    state1.centers[1] = 1;
    state1.centers[2] = 1;
    Pyraminx state2 = createSolved();
    




    Pyraminx solved = createSolved();
    makeMove(&solved, U);
    makeMove(&solved, B); 
    makeMove(&solved, U);
    makeMove(&solved, Lprim);
    makeMove(&solved, Rprim);
    makeMove(&solved, L);
    makeMove(&solved, U);
    makeMove(&solved, Rprim);
    makeMove(&solved, Bprim);

    std::vector<std::vector<bodyMove>> solutions = findSolution(solved, createSolved(), 0, false);
 

    for(int i =0;i<solutions.size();i++){
        std::vector<bodyMove> solution = solutions.at(i);
        for(int j=0;j<solution.size();j++){
            std::cout << getLetterMove(solution.at(j));
        }
        std::cout << "\n";
    }
}

