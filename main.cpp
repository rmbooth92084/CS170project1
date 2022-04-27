#include <iostream>
#include <vector>
#include <string>
#include <cmath>
#include <climits>
using namespace std;
/*
 *class for creating the board problem with also the goal_state
 */
unsigned long int max_queue_size = 0;
unsigned long int num_nodes_explored = 0;
class create_board
{
    private:
        vector<vector<int>> board;
        vector<vector<int>> goal_state;
    public:
        //constructor
        create_board(){
            vector<int> temp;
            for(int i = 0; i < 3; i++){
                temp.push_back(0);
            }
            for(int i = 0; i < 3; i++){
                board.push_back(temp);
                goal_state.push_back(temp);
            }
        }
        create_board(int size, vector<int> input){
            //cout << "making board" << endl;
            vector<int> temp;

            for(int i = 0; i < size; i++){
                temp.push_back(0);
            }
            //fills the board and goal_state with 0's to 
            //allocate memory
            for(int i = 0; i < size; i++){
                board.push_back(temp);
                goal_state.push_back(temp);
            }
            //cout << "before memery clean up" << endl;
            //free up memory
            temp.clear();
            temp.shrink_to_fit();
            set_board(input);
            //cout << "made board" << endl;
        }
        vector<vector<int>> get_board(){
            return board;
        }
        vector<vector<int>> get_goal(){
            return goal_state;
        }
        //creates the unsolved board as well as the final state board
        void set_board(vector<int> input){
            int k = 0;
            for(int i = 0; i < board.size(); i++){
                for(int j = 0; j < board.size(); j++){
                    board[i][j] = input[k];
                    k++;
                    
                    //this if leaves the last space open
                    if(k >= (get_size() * get_size())){
                        goal_state[i][j] = 0;
                        continue;
                    }
                    goal_state[i][j] = k;
                }
            }
        }
        //returns the size of the board
        int get_size(){
            return board.size();
        }
        //tests if the goal state and final state are the same
        bool test_finish(){
            for(int i = 0; i < get_size(); i++){
                for(int j = 0; j < get_size(); j++){
                    if(board[i][j] != goal_state[i][j])
                        return false;
                }   
            }
            return true;
        }
        //outputs the unsolved board
        //for debugging
        void output_board(){
            cout << "Unsolved board: " << endl;
            for(int i = 0; i < board.size(); i++){
                for(int j = 0; j < board.size(); j++){
                    cout << board[i][j] << " ";
                }
                cout << endl;
            }
        }
        void output_goal_state(){
            cout << "Goal state: " << endl;
            for(int i = 0; i < goal_state.size(); i++){
                for(int j = 0; j < goal_state.size(); j++){
                    cout << goal_state[i][j] << " ";
                }
                cout << endl;
            }
        }
        //finds the open spot in the board
        vector<int> find_open_space(){
            vector<int> result;
            for(int i = 0; i < get_size(); i++){
                for(int j = 0; j < get_size(); j++){
                    if(board[i][j] == 0){
                        result.push_back(i);
                        result.push_back(j);
                    }
                }
            }
            return result;
        }
        vector<int> get_num_order(){
            vector<int> result;
            for(int i = 0; i <board.size(); i++){
                for(int j = 0; j < board.size(); j++){
                    result.push_back(board[i][j]);
                }
            }
            return result;
        }
        vector<int> get_correct_spot(int target){
            vector<int> result;
            for(int i = 0; i <goal_state.size(); i++){
                for(int j = 0; j < goal_state.size(); j++){
                    if(goal_state[i][j] == target){
                        result.push_back(i);
                        result.push_back(j);
                        return result;
                    }   
                }
            }
            return result;
        }
        int get_specific_num(int y, int x) {
            return board[y][x];
        }
        //calculates the hurristic value of the board
        double calculate_euclidean_heuristic(){
            int cnt = 0;
            double result = 0;
            vector<int> correct_spot;
            for(int i = 0; i <board.size(); i++){
                for(int j = 0; j < board.size(); j++){
                    if(cnt >= 9)
                        continue;
                    if(board[i][j] != cnt){
                        correct_spot = get_correct_spot(cnt);
                        //result += pow(pow(abs(correct_spot[0] - i),2) + pow(abs(correct_spot[1] - j), 2), 0.5);
                        result += abs(correct_spot[0] - i) + abs(correct_spot[1] - j);
                    }
                    cnt++;
                }
            }
            return result;
        }
        int calculate_misplaced_heuristic(){
            int result = 0;
            for(int i = 0; i <board.size(); i++){
                for(int j = 0; j < board.size(); j++){
                    if(board[i][j] != goal_state[i][j])
                        result++;
                }
            }
            return result;
        }

};
//for making the tree with any amount of leaves per node
struct Node
{
    int total_edge_distance;//distance
    double cost;
    double eucledian_heu;
    int mispalced_heu;
    create_board board;
    Node * parent;
    vector<Node *> child;//pointer to each child
};
//Fuction for helping with creating child nodes
//alg_choice 0 = Uniform
//alg_choice 1 = A* Misplaced Tile heuristic
//alg_choice 2 = A* Eucledian distance
Node *new_node(vector<int> board, Node * parent, int alg_choice)
{
    create_board new_board(int(sqrt(board.size())), board);
    Node *temp = new Node;
    temp->board = new_board;
    temp->parent = parent;
    temp->mispalced_heu = temp->board.calculate_misplaced_heuristic();
    temp->eucledian_heu = temp->board.calculate_euclidean_heuristic();
    temp->total_edge_distance = parent->total_edge_distance + 1;
    if(alg_choice == 1)
        temp->cost = temp->total_edge_distance + temp->mispalced_heu;
    else if(alg_choice == 1)
        temp->cost = temp->total_edge_distance + temp->eucledian_heu;
    else
        temp->cost = temp->total_edge_distance;
    return temp;
}
//function for helping with creating the root node
Node *new_root_node(create_board board)
{
    Node *temp = new Node;
    temp->board = board;
    temp->parent = nullptr;
    temp->total_edge_distance = 0;
    temp->cost = 0;
    return temp;
}
//helper funtion to make_nodes by making a move in the game
//by switching the target with the open spot.
//Returns the positon of the target in postion 0 while the open spot
//postion at 1
vector<int> reorder_board(vector<int> order, int target){
    vector<int> result = {-1,-1};

    for(int i = 0; i < order.size(); i++){
        if(order[i] == target){
            result[0] = i;
        }
        else if(order[i] == 0){//check the postion of the open space
            result[1] = i;
        }
    }
    return result;
}
//helper funtion for make_child_nodes
void swap(int y, int x,  vector<int> empty_spot, Node *node, int choice){
   // cout << "in make_child_nodes" << endl;
    create_board board = node->board;
    //board.output_board();
    //vector<int> num_order = board.get_num_order();
    vector<int> num_switch;
    vector<int> num_order = board.get_num_order();
    int temp1, temp2;
    int empty_conver = empty_spot[1] + (empty_spot[0] * board.get_size());
    num_switch = reorder_board(num_order, board.get_specific_num(y, x));
    //move the piece into the empty spot
    temp1 = num_order[num_switch[0]];
    temp2 = num_order[empty_conver];
    //cout << "checking moving tile 0: "<< num_switch[0] << endl;

    num_order[num_switch[0]] = temp2;
    num_order[empty_conver] = temp1;

    (node->child).push_back(new_node(num_order, node, choice));
}
//helper funtion for making nodes
void make_child_nodes(Node *node, int choice){
    //cout << "in make_child_nodes" << endl;
    create_board board = node->board;
    //board.output_board();
    vector<int> empty_spot = board.find_open_space();
  //  vector<int> num_order = board.get_num_order();
 //   vector<int> num_switch;
    int x, y;
    //cout << "after making variables" << endl;
    //checks up of open spot
    
    y = empty_spot[0];
    x = empty_spot[1];
    //int empty_conver = x + (y * board.get_size());
    //cout << "checking empty_spot 0: "<< empty_spot[0] << endl;
    //cout << "checking empty_spot 1: "<< empty_spot[1] << endl;
    //num_nodes_explored++;
    //cout << "Number of nodes explored: "<< num_nodes_explored << endl;
    if(y - 1 >= 0){
        //cout << "above can move" << endl;
        swap(y - 1, x, empty_spot, node, choice);
    }
    //checks down of open spot
    
    if(y + 1 < board.get_size()){
        //cout << "below can move" << endl;
        swap(y + 1, x, empty_spot, node, choice);
    }
    //checks 
    if(x - 1 >= 0){
        //cout << "left can move" << endl;
        swap(y, x - 1, empty_spot, node, choice);
    }
    
    if(x + 1 < board.get_size()){
        //cout << "right can move" << endl;
        swap(y, x + 1, empty_spot, node, choice);
    }

}
void output_solution(Node *node){
    bool finished = false;
    vector<Node*> solution;
    while(!finished){
        cout << "Output loop" << endl;
        //node->board.output_board();
        solution.push_back(node);
        if(node->parent == nullptr)
            finished = true;
        node = node->parent;
    }
    if(solution.empty())
        cout << "Invalid puzzle" << endl;
    else{
        //outputs the board from the start state to the goal state
        for(int i = solution.size() - 1; i >= 0; i--){
            solution[i]->board.output_board();
        }
    }
}
void find_solution(create_board board, int choice){
    bool found = false;
    int least = INT_MAX;
    int least_pos = 0;
    Node *temp;
    vector<Node*> frontier;
    vector<Node*> explored;
    vector<Node*>::iterator it;
    Node *root = new_root_node(board);

    frontier.push_back(root);
    //cout << "before frontier loop" << endl;
    while(!found){
        
        //finds the one node with the least cost in the frontier
        for(int i = 0; i < frontier.size(); i++){
            if(frontier[i]->cost < least){
                least = frontier[i]->cost;
                least_pos = i;
            }
        }
       // cout << "after first loop for deciding shortest edge" << endl;
        temp = frontier[least_pos];
        it = frontier.begin() + least_pos;
        frontier.erase(it);
        explored.push_back(temp);
        //cout << "removed node that we will explore" << endl;
        if(temp->board.test_finish()){
            found = true;
            cout << "found finish" << endl;
            continue;
        }
        //cout << "after check if the node removed is the final state" << endl;
        make_child_nodes(temp, choice);
        //cout << "after making child nodes" << endl;
        //pushes all the childen of the removed node into the frontier
        for(int i = 0; i < temp->child.size(); i ++){
            frontier.push_back(temp->child[i]);
        }
        //searches for the final state as well as only picking it if
        //it has the least cost in the frontier
        int least_cost = INT_MAX;
        int pos;
        for(int j = 0; j < frontier.size(); j++){
            if(frontier[j]->cost < least_cost){
                pos = j;
                least_cost = frontier[j]->cost;
                /*
                if(max_queue_size < frontier.size()){
                    max_queue_size = frontier.size();
                }*/
            }
            if(j + 1  >= frontier.size() && frontier[j]->board.test_finish()){
                temp = frontier[j];
                found = true;
                temp = frontier[j];
                //it = frontier.begin() + j;
            }
        }
        

    }
    output_solution(temp);
}
int main()
{
    //test boards with 0 indicating the space
    vector<int> trivial {1,2,3,4,5,6,7,8,0};
    vector<int> very_easy {1,2,3,4,5,6,7,0,8};
    vector<int> easy {1,2,0,4,5,3,7,8,6};
    vector<int> do_able {0,1,2,4,5,3,7,8,6};
    vector<int> oh_boy {8,7,1,6,0,2,5,4,3};
    vector<int> impossible {1,2,3,4,5,6,8,7,0};
    int alg_choice = 1;
    create_board test(3, oh_boy);
    cout << "before output" << endl;
    //test.output_board();
    find_solution(test, alg_choice);
   // cout << test.get_size() << endl;
    cout << "after output" << endl;
}
