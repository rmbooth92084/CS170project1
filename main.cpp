/*
    Created by Raymond Booth 862152152
    github repo: https://github.com/rmbooth92084/CS170project1
*/
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
unsigned long int depth = 0;
class create_board
{
    private:
        vector<vector<int>> board;
        vector<vector<int>> goal_state;
        vector<int> user_input;
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
            user_input = input;
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
        vector<int> get_order(){
            return user_input;
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
            cout << "-----" << endl;
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
            for(int i = 0; i < goal_state.size(); i++){
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
            int cnt = 1;
            double result = 0;
            vector<int> correct_spot;
            for(int i = 0; i <board.size(); i++){
                for(int j = 0; j < board.size(); j++){
                    if(cnt >= 9)
                        cnt = 0;
                    if(board[i][j] != cnt){
                        correct_spot = get_correct_spot(board[i][j]);
                        result += pow(pow(abs(correct_spot[0] - i),2) + pow(abs(correct_spot[1] - j), 2), 0.5);
                        //result += abs(correct_spot[0] - i) + abs(correct_spot[1] - j);
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
    else if(alg_choice == 2)
        temp->cost = temp->total_edge_distance + temp->eucledian_heu;
    else
        temp->cost = temp->total_edge_distance;
    return temp;
}
//function for helping with creating the root node
Node *new_root_node(create_board board, int alg_choice)
{
    Node *temp = new Node;
    temp->board = board;
    temp->parent = nullptr;
    temp->total_edge_distance = 0;
    temp->mispalced_heu = temp->board.calculate_misplaced_heuristic();
    temp->eucledian_heu = temp->board.calculate_euclidean_heuristic();
    if(alg_choice == 1)
        temp->cost = temp->total_edge_distance + temp->mispalced_heu;
    else if(alg_choice == 2)
        temp->cost = temp->total_edge_distance + temp->eucledian_heu;
    else
        temp->cost = temp->total_edge_distance;
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
//does the swaping of the empty space and the target number
void swap(int y, int x,  vector<int> empty_spot, Node *node, int choice){
   //cout << "in make_child_nodes" << endl;
    create_board board = node->board;
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
//check all possible movement spaces from an open space
//and make nodes accordingly with the help of swap
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
    num_nodes_explored++;
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
//helper funtion that outputs the solution
void output_solution(Node *node){
    bool finished = false;
    vector<Node*> solution;
    while(!finished){
        //cout << "Output loop" << endl;
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
            //cout << "g(n): " << solution[i]->total_edge_distance << endl;
            //cout << "Cost of state: " << solution[i]->cost << endl;
            //cout << "eucledian_heu of state: " << solution[i]->eucledian_heu << endl;
        }
        depth = solution.size();
    }
}
//main funtion that finds the solution to a board accorting to the algorithm chosen
void find_solution(create_board board, int choice){
    Node *temp;
    vector<Node*> frontier;
    vector<Node*> explored;
    vector<Node*>::iterator it;
    Node *root = new_root_node(board, choice);

    int least_cost = INT_MAX;
    int pos = 0;

    double cost;
    frontier.push_back(root);
    //cout << "before frontier loop" << endl;
    while(1){
        least_cost = INT_MAX;
        if(frontier.empty()){
            cout << "No solution found" << endl;
            break;
        }
        //searches for the final state as well as only picking it if
        //it has the least cost in the frontier
        for(int j = 0; j < frontier.size(); j++){
            cost = frontier[j]->cost;
           //cout << "The cost is " << cost << endl;
            if(cost < least_cost){
                pos = j;
                least_cost = cost;
            }
            //moves postion to the final state leaf if it has the same cost or lower
            //compared to the current least cost
           //cout << "now it's  " << least_cost << endl;
            if(frontier[j]->board.test_finish() && cost <= least_cost){
                pos = j;
                least_cost = cost;
            }

        }
        
       // cout << "after first loop for deciding shortest edge" << endl;
        temp = frontier[pos];

        //cout << "Node to remove and explore: " << endl;
        //temp->board.output_board();

        it = frontier.begin() + pos;
        explored.push_back(temp);
        frontier.erase(it);
        //cout << "removed node that we will explore" << endl;
        if(temp->board.test_finish()){
            cout << "found finish" << endl;
            break;
        }
        //cout << "after check if the node removed is the final state" << endl;
        make_child_nodes(temp, choice);
        //cout << "after making child nodes" << endl;
        //pushes all the childen of the removed node into the frontier
        //only adds child to frontier if it's a unique state
        bool unique;
        for(int i = 0; i < temp->child.size(); i ++){
            unique = true;
            for(int k = 0; k < explored.size(); k++){
                //if the order of the board is the same as one in the explored set the not unique
                if(temp->child[i]->board.get_order() == explored[k]->board.get_order()){
                    unique = false;
                }
            }
            if(unique){
                frontier.push_back(temp->child[i]);
                //temp->child[i]->board.output_board();
                //cout << "cost" <<temp->child[i]->cost << endl;
                //cout << "Code being pushed into the frontier: " << endl;
               //temp->child[i]->board.output_board();
            }
        }
        //check max queue size
        if(frontier.size() > max_queue_size){
            max_queue_size = frontier.size();
            //cout << "current max queue " << max_queue_size << endl;
            //cout << "explored nodes " << num_nodes_explored << endl;  
        }
        

    }
    output_solution(temp);
}
//function for getting user input to solver their puzzle
void ui(){
    //default puzzle is the one for part 2 of what needs to be submitted to ilearn
    vector<int> main_one {1,0,3,4,2,6,7,5,8};
    vector<int> user_board;
    vector<int> temp_board;
    int user_input;
    int next_input;
    int temp;
    int alg_choice;
    cout << "Welcome to rboot005 8 puzzle solver." << endl;
    cout << "Type 1 to use a default puzzle or 2 to enter you own puzzle." << endl;

    cin >> user_input;

        if(user_input == 1){
            temp_board = main_one;
            cout << "Now pick an algorithm you would like to use to search for the solution" << endl;
            cout << "Input 0 for Uniform Cost Search" << endl;
            cout << "Input 1 for A* with Mispalced Tile heuristic" << endl;
            cout << "Input 2 for A* with Eucledian distance heuristic" << endl;
            cin >> alg_choice;
            create_board board(pow(temp_board.size(),0.5), main_one);
            find_solution(board, alg_choice);
        }
        else if(user_input == 2){
            cout << "Please enter you puzzle. Note the first 3 will be the top 3" << endl;
            cout << "in the order you put them in and the next 3 will be the same for" << endl;
            cout << "next row and so on. Input 0 to indicate the blank space." << endl;
            cout << "Example input: 123456780" << endl;
            cin >> next_input;
            while(next_input != 0){
                temp = next_input % 10;
                user_board.push_back(temp);
                next_input /= 10;
            }
            //user input it recived backwards so need to flip it
            for (int i = user_board.size() - 1; i >= 0 ; i--){
                temp_board.push_back(user_board[i]);
            }
            cout << "Now pick an algorithm you would like to use to search for the solution" << endl;
            cout << "Input 0 for Uniform Cost Search" << endl;
            cout << "Input 1 for A* with Mispalced Tile heuristic" << endl;
            cout << "Input 2 for A* with Eucledian distance heuristic" << endl;
            cin >> alg_choice;
            create_board board(pow(temp_board.size(),0.5), temp_board);
            find_solution(board, alg_choice);
        }
        else
            cout << "Invalid input" << endl;
    
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

    vector<int> main_one {1,0,3,4,2,6,7,5,8};
   /* int alg_choice = 1;

    create_board test(3, impossible);
    cout << "before output" << endl;
    find_solution(test, alg_choice);
    cout << "after output" << endl;
    */
    ui();
    cout << "To solve this problem the search algorithm expaned " << num_nodes_explored << " nodes." << endl;
    cout << "The max number of nodes in the queue at any one time was " << max_queue_size << " nodes." << endl;
    cout << "The depth of the goal state was " << depth << " nodes." << endl;
    
}
