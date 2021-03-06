#include "../include/board.h"
#include "../include/man.h"
#include "../include//king.h"
#include <iomanip>

//#define VERBOSE


// defalult constructor
Board::Board() {
    initiate_pieces_array();
}

// constructor taking vectors of locations of pieces, mainly for loading from file
Board::Board(vector <int> vector_black, vector <int> vector_white,
             vector<int> vector_black_king, vector<int> vector_white_king) {
    initiate_pieces_from_vector(vector_black, vector_white, vector_black_king, vector_white_king);
}

// function used to check whether given space is empty
int Board::check_space_empty(int location) {
    if (location < 0 || location > 31) {
        cout << "Wrong space number.";
        return -1;
    }
    else {
        if (squares_of_pieces[location] == NULL) {
            return 1;
        }
        else {
            return 0;
        }
    }
}

// function used by the constructor to iniate the object from vectors
int Board::initiate_pieces_from_vector(vector <int> vector_black, vector <int> vector_white,
                                       vector<int> vector_black_king, vector<int> vector_white_king) {
    Man *man_temp_pointer;
    Piece *piece_temp_pointer;

    // initiate pointers in the squares_of_pieces array to NULL
    int i;
    for (i=0; i<32; i++) {
        squares_of_pieces[i] = NULL;
    }

    //initiate black
    for (int &man : vector_black) {
        if (squares_of_pieces[man] == NULL) {
            squares_of_pieces[man] = new Man(black, man);
#ifdef VERBOSE
            cout << "Initiated black man at: " << man << endl;
#endif
        }
        else {
            cout << man << " at squares_of_pieces array is not empty." << endl;
        }
    }

    for (int &man : vector_white) {
        if (squares_of_pieces[man] == NULL) {
            squares_of_pieces[man] = new Man(white, man);
#ifdef VERBOSE
            cout << "Initiated white man at: " << man << endl;
#endif
        }
        else {
            cout << man << " at squares_of_pieces array is not empty." << endl;
        }
    }
}

// function used by the constructor to initialize default game
int Board::initiate_pieces_array() {
    Man *man_temp_pointer;
    Piece *piece_temp_pointer;
    //initiate black
    int i;
    for (i=0; i<12; i++) {
//        man_temp_pointer =
        squares_of_pieces[i] = new Man(black, i);
    }
    for (;i<20; i++) {
        squares_of_pieces[i] = NULL;
    }
    for (; i<32; i++) {
        squares_of_pieces[i] = new Man(white, i);
    }
}

//function used to print the board
void Board::print_board() {
    cout << endl;
    cout << "               0     1     2     3     4     5     6     7    \n";

    int i, j=0;
    for (i=0; i<32; i++) {
        if ((i%4)==0) {
            cout << "            +-----------------------------------------------+\n";
            cout << "          " << i/4 << " |";
        }

        if ((j%2) ==0) {
            cout << "     |";
        }

        if (squares_of_pieces[i] == NULL) {
            cout << "" <<  setw(2)  << i << "   |" ;//    << 1 << endl;

//            cout << format("%1% %2% %|40t|%3%\n") i << "   |"; //"%5d|
        }
        else {
            char piece = squares_of_pieces[i]->return_descriptive_char();
            cout << setw(2) << i << setw(2)  << piece << " |" ;//    << 1 << endl;
//            cout << piece << "    |"; //"%5c|", );
        }


        if ((j%2) == 1) {
            cout << "     |";
        }
        if ((i%4) == 3) {
            j++;
            putchar('\n');
        }
    }
    cout << "            +-----------------------------------------------+\n\n\n";
}

// older version of printing function
void Board::print_squares() {
    int i, j=0;
    for (i=0; i<32; i++) {
        if ((j%2) ==1) {
            cout << "   ";
        }
//        printf("%5d", i);
        cout << setw(5) << i;
        if ((j%2) == 0) {
            cout << "   ";
        }
        if ((i%4) == 3) {
            j++;
            putchar('\n');
        }
    }
}

// one of the functions responsible for making a move
int Board::try_move(int from, int to) {
    int empty_to = check_space_empty(to);
    int empty_from = check_space_empty(from);

    if (empty_to == 0) {
        cout << "For piece at " << from << " can't move to " << to
             << ". The space is not empty. Choose a different square." << endl;
        return -1;
    }
    else if (empty_from == 1) {
        cout << "There is not piece at " << from << " so something that does not exist can't move... :)" << endl;
        return -1;
    }
    else {
        vector<int> moves_proposal;
        vector<int> moves_valid;
        moves_proposal = (squares_of_pieces[from]->generate_possible_moves());

        for (int &move : moves_proposal) {
//            std::cout << "valid move maybe: " << move << endl;
            empty_to = check_space_empty(move);
            if (empty_to == 1) {
                moves_valid.push_back(move);
//                std::cout << "valid move : " << move << endl;
            }
        }
        std::cout << '\n';
        for (int &move_valid : moves_valid) {
            if (move_valid == to) {
                if (squares_of_pieces[to] == NULL) {
                    squares_of_pieces[to] = squares_of_pieces[from];
                    squares_of_pieces[from]->move(from, to);
                    squares_of_pieces[from] = NULL;

                } else {
                    cout << "Something went wrong with a pointer";
                }

                cout << "Piece from " << from << " successfuly moved to " << to << "." << endl; //  %d.", from, to);
                return 1;
            }
        }

        cout << "For piece at " << from << " can't move to " << to << ", it's not a valid move.";
    }
    return 0;
}

// one of the functions responsible for checking whether capture is valid
int Board::try_capture(int from, int to) {
    int empty_a, empty_b;
    int right_target = 0;
    int non_empty_a = 0;
    vector<std::tuple<int, int>> capture_proposal;
    vector<std::tuple<int, int>> captures_valid;

    if (squares_of_pieces[from] != NULL) {
        capture_proposal = squares_of_pieces[from]->generate_possible_captures();
    }
    else {
        cout << "These is somthing wrong with the pointer. There is no pointer at this place of the squares_of_pieces array." << endl;
        return -1;
    }

//    vector<int> captures_valid;

    for (auto&& tuple: capture_proposal) {

        int a, b;
        std::tie(a, b) = tuple;
//        std::cout << a << " " << b << " " << std::endl;

        empty_a = check_space_empty(a);
        empty_b = check_space_empty(b);

        if (b==to) {
            right_target = 1;
        }

        if (empty_a == 0 && empty_b == 1) {
            Color color_from, color_through;
            color_through = squares_of_pieces[a]->get_color();
            color_from = squares_of_pieces[from]->get_color();

            string str_color_trough;
            string str_color_from;

            if (color_through== black) {
                str_color_trough = "black";
            }
            else {
                str_color_trough = "white";
            }

            if (color_from == black) {
                str_color_from = "black";
            }
            else {
                str_color_from = "white";
            }

            if (color_through != color_from) {
                //            captures_valid.push_back(b);
                captures_valid.push_back(std::make_tuple(a, b));

                //            tuples_move_capture.push_back(std::make_tuple(location_new1, location_new2));
//                cout << "Valid capture from (" << str_color_from << ") : " << from << " to: (" << str_color_trough << ") " << b << endl;

            }
            else {
                cout << "Trying to remove one's own piece... :)"<< endl;
            }

        }
        else {
//            cout << "captures from " << from << "not a valid capture " << " to: " << b << "   a_empty: " << empty_a << "   b_empty" << empty_b << endl;
//            cout << "You capture by jumping one square over enemy's piece, meaning you move two squares." << endl;
        }
    }

    for (auto&& tuple: captures_valid) {
        int a, b;
        std::tie(a, b) = tuple;

        if (b == to) {
            if (squares_of_pieces[b] == NULL && squares_of_pieces[a] != NULL) {
                squares_of_pieces[to] = squares_of_pieces[from];
                squares_of_pieces[from]->move(from, to);
                squares_of_pieces[from] = NULL;

                //deleting
                delete squares_of_pieces[a];
                squares_of_pieces[a] = NULL;

            } else {
                cout << "Something went wrong with a pointer";
            }

            cout << "Piece from " << from << " successfuly moved to " << to << ". ";
            cout << "And captured at square " << a << " which was removed from the board.";//  %d.", from, to);
            return 1;
        }
    }

    if (right_target==0) {
        cout << "You cannot move capture from " << from << " to " << to << " Wrong target." << endl;
    }

    return 0;
}

// function incorporating almost all move functions
int Board::whole_move_procedure(int from, int to, Color turn) {

    if (squares_of_pieces[from] != NULL) {
        if (squares_of_pieces[from]->get_color() != turn) {
            string str_color_from;
            string str_color_turn;
            if (turn == black) {
                str_color_turn = "black";
                str_color_from = "white";
            }
            else {
                str_color_turn = "white";
                str_color_from = "black";
            }
            cout << "It's " << str_color_turn << " move and " << str_color_from << " was trying to move." << endl;
            return -1;
        }

    }

    int moved = try_move(from, to);

    if (moved == 1) {
        man_to_king_promotion(to);
        return 1;
    }
    else if (moved == -1) {
        // something went wrong
        return -1;
    }

    // check capture
    int captured = try_capture(from, to);
    if (captured==1) {
        man_to_king_promotion(to);
    }
    return captured;
}

// function checks whether a move proposed by player is found within possible moves
int Board::check_move_in_valid_moves(int to, int* valid_moves, int size) {
    int i;
    int destination_in_valid_moves = 0;
    for (i=0; i<size; i++) {
        if (valid_moves[i] == to) {
            destination_in_valid_moves = 1;
            return destination_in_valid_moves;
        }
    }

    return destination_in_valid_moves;
}

// function promoting a man to king
int Board::promotion(int location, Color color, string str_color) {
    delete squares_of_pieces[location];
    squares_of_pieces[location] = NULL;
    squares_of_pieces[location] = new King(color, location);
    cout << "Promoted (" << str_color << ") at location: " << location << endl;
}

// outer function promoting man to king
int Board::man_to_king_promotion(int location) {
    int empty = check_space_empty(location);
    if (empty==0) {
        Color color = squares_of_pieces[location]->get_color();
        string str_color;
        if (color == black) {
            str_color = "black";
        }
        else {
            str_color = "white";
        }

        int row = squares_of_pieces[location]->calculate_row(location);

        if (color == black && row == 7) {
            promotion(location, color, str_color);
        }
        else if (color == white && row == 0) {
            promotion(location, color, str_color);
        }
        else {
#ifdef VERBOSE
            cout << "Not a row to promote : " << location << endl;
#endif
        }
    }
    else {
        cout << "Can't promote at: " << location << ". You can't promote something that isn't there." << endl;
    }

}

// function checking whether player has any pieces left, used to check whether the game has ended
int Board::check_player_has_no_pieces() {
    int i;
    int pieces_black = 0, pieces_white = 0;

    for (i=0; i<32; i++) {
        if (squares_of_pieces[i] != NULL) {
            Color c = squares_of_pieces[i]->get_color();
            if (c==black)
                pieces_black++;
            else
                pieces_white++;
        }
    }
    if (pieces_black == 0) {
        game_ended = 1;
        winner = white;
        cout << "White won." << endl;
        return 1;

    }
    else if (pieces_white == 0) {
        game_ended = 1;
        winner = black;
        cout << "Black won." << endl;
        return -1;
    }
    cout << "Game's on." << endl;
    return 0;
}

// returns locations off all the pieces in a tuple of two vectors
std::tuple<vector<int>, vector<int>> Board::return_locations_pieces_board() {
    vector<int> black_pieces;
    vector<int> white_pieces;

    Color temp_color;

    int i;
    for (i=0; i<32; i++) {
        if (squares_of_pieces[i] != NULL) {
            temp_color = squares_of_pieces[i]->get_color();

            if (temp_color == black) {
                black_pieces.push_back(i);
            }
            else if (temp_color == white) {
                white_pieces.push_back(i);
            }
        }
    }
    return make_tuple(black_pieces, white_pieces);
}