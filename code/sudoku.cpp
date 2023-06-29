#include <iostream>
#include <vector>
#include <utility>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <fstream>
#include <map>
#include <string.h>
#include <random>
#include <algorithm>
#include <numeric>
#include <chrono>
using namespace std;

typedef vector<vector<char> > Board;

const int N = 9;

class SudokuPlayer
{
private:
    // 使用位运算来表示某个数是否出现过
    // rowUsed[i] 表示第i行使用过的数字，columnUsed[j]表示第j列使用过的数字，blockUsed[k]表示第k个3x3块使用过的数字
    int rowUsed[N];
    int columnUsed[N];
    int blockUsed[N];

public:
    vector<Board> result;           //存储解决方案的集合
    vector<pair<int, int> > spaces; // 存储数独棋盘上所有空格的位置

public:
    SudokuPlayer()
    {
        initState();
    }

    void initState()
    {
        memset(rowUsed, 0, sizeof(rowUsed));
        memset(columnUsed, 0, sizeof(columnUsed));
        memset(blockUsed, 0, sizeof(blockUsed));
        spaces.clear();
        result.clear();
    }

    void addResult(Board &board)
    {
        vector<vector<char> > obj(board);
        result.push_back(obj);
    }

    //将第i行、第j列、对应的3x3块中的digit翻转（即如果digit为使用状态，则变为未使用，反之亦然）
    void flip(int i, int j, int digit)
    {
        rowUsed[i] ^= (1 << digit);
        columnUsed[j] ^= (1 << digit);
        blockUsed[(i / 3) * 3 + j / 3] ^= (1 << digit);
    }

    vector<Board> solveSudoku(Board board)
    {
        initState();
        for (int i = 0; i < N; i++)
        {
            for (int j = 0; j < N; j++)
            {
                if (board[i][j] == '$') // 若为'$'，表示此格为空，需要填入数字
                {
                    spaces.push_back(pair<int, int>(i, j));
                }
                else // 否则，更新行、列、块的使用情况
                {
                    int digit = board[i][j] - '1';
                    flip(i, j, digit);
                }
            }
        }
        DFS(board, 0);
        return result;
    }

    void DFS(Board &board, int pos)
    {
        if (pos == spaces.size()) // 所有空位已填满，此时的棋盘状态为一个解决方案
        {
            addResult(board);
            return;
        }
        int i = spaces[pos].first, j = spaces[pos].second;
        for (int digit = 0; digit < N; digit++) // 尝试在空格填入所有可能的数字
        {
            // 判断在第i行、第j列、对应的3x3块中digit是否已经使用过，若使用过则不能填入，否则可以尝试填入
            if ((rowUsed[i] & (1 << digit)) || (columnUsed[j] & (1 << digit)) || (blockUsed[(i / 3) * 3 + j / 3] & (1 << digit)))
            {
                continue;
            }
            flip(i, j, digit); // 更新行、列、块的使用情况
            board[i][j] = '1' + digit; // 尝试填入数字
            DFS(board, pos + 1); // 继续搜索下一个空格的填入数字
            flip(i, j, digit); // 恢复行、列、块的使用情况，进行回溯
            board[i][j] = '$'; // 恢复棋盘状态，进行回溯
        }
    }

    void getResult()
    {
        for (size_t i = 0; i < result.size(); i++)
        {
            Board board = result[i];
            printBoard(board);
        }
    }

    bool checkBoard(Board &board)
    {
        initState();
        for (int i = 0; i < N; i++)
        {
            for (int j = 0; j < N; j++)
            {
                // 计算当前格子上的数字（把字符转换为整数）
                if (board[i][j] != '$' && board[i][j] != '.')
                {
                    int digit = board[i][j] - '1';

                    // 检查当前数字是否在其所在的行、列或者块中已经使用过
                    // 使用位操作来进行这个检查：如果对应的位已经被设置为1，那么结果不为0，即为true
                    // 这里使用位或（|）操作来组合所有的使用状态，然后使用位与（&）操作来检查特定的位
                    if ((rowUsed[i] | columnUsed[j] | blockUsed[(i / 3) * 3 + j / 3]) & (1 << digit))
                    {
                        return false;
                    }
                    flip(i, j, digit);
                }
            }
        }
        return true;
    }

    void printBoard(Board &board)
    {
        for (int i = 0; i < board.size(); i++)
        {
            for (int j = 0; j < board[i].size(); j++)
            {
                cout << board[i][j] << " ";
            }
            cout << "\n";
        }
    }

    vector<int> getRand9()
    {
        // 使用C++11的random_device来设置种子
        // std::random_device rd;
        // std::mt19937 g(rd());

        // 使用当前系统时间生成随机种子
        auto seed = std::chrono::high_resolution_clock::now().time_since_epoch().count();
        mt19937 g(seed);
        // 初始化一个从0到8的向量
        vector<int> result(9);
        iota(result.begin(), result.end(), 0); 

        // 使用shuffle函数随机排列向量
        
        shuffle(result.begin(), result.end(), g);
        return result;
    }
    Board generateBoard(int digCount)
    {
        vector<vector<char>> board(N, vector<char>(N, '$'));
        vector<int> row = getRand9();
        for (int i = 0; i < 3; i++)
        {
            board[3][i + 3] = row[i] + '1';
            board[4][i + 3] = row[i + 3] + '1';
            board[5][i + 3] = row[i + 6] + '1';
        }
        copySquare(board, 3, 3, true);
        copySquare(board, 3, 3, false);
        copySquare(board, 3, 0, false);
        copySquare(board, 3, 6, false);

        while (digCount)
        {
            int x = rand() % 9;
            int y = rand() % 9;
            if (board[x][y] == '$')
                continue;
            char tmp = board[x][y];
            board[x][y] = '$';

            solveSudoku(board);
            if (result.size() == 1)
            {
                digCount--;
            }
            else
            {
                board[x][y] = tmp;
            }
        }
        // printBoard(board);
        // cout << "spaces " << player.spaces.size() << "\n";
        if (!checkBoard(board))
        {
            cout << "wrong board" << endl;
        }

        return board;
    }

    

    void copySquare(Board &board, int src_x, int src_y, bool isRow)
    {
        // 随机决定复制的顺序
        int rand_tmp = rand() % 2 + 1;
        int order_first[3] = {1, 2, 0};
        int order_second[3] = {2, 0, 1};
        if (rand_tmp == 2)
        {
            order_first[0] = 2;
            order_first[1] = 0;
            order_first[2] = 1;
            order_second[0] = 1;
            order_second[1] = 2;
            order_second[2] = 0;
        }

        // 对棋盘进行复制和调整
        for (int i = 0; i < 3; i++)
        {
            int order[2][3] = { {order_first[0], order_first[1], order_first[2]}, {order_second[0], order_second[1], order_second[2]} };
            for (int j = 0; j < 2; j++)
            {
                for (int k = 0; k < 3; k++)
                {
                    if (isRow)
                    {
                        board[src_x + k][i + j * 6] = board[src_x + order[j][k]][src_y + i];
                    }
                    else
                    {
                        board[i + j * 6][src_y + k] = board[src_x + i][src_y + order[j][k]];
                    }
                }
            }
        }
    }
};
char data[9][9] = {
    {'5', '3', '.', '.', '7', '.', '.', '.', '.'},
    {'6', '.', '.', '1', '9', '5', '.', '.', '.'},
    {'.', '9', '8', '.', '.', '.', '.', '6', '.'},
    {'8', '.', '.', '.', '6', '.', '.', '.', '3'},
    {'4', '.', '.', '8', '.', '3', '.', '.', '1'},
    {'7', '.', '.', '.', '2', '.', '.', '.', '6'},
    {'.', '6', '.', '.', '.', '.', '2', '8', '.'},
    {'.', '.', '.', '4', '1', '9', '.', '.', '5'},
    {'.', '.', '.', '.', '8', '.', '.', '7', '9'}};

void test()
{
    SudokuPlayer player;
    vector<vector<char> > board(N, vector<char>(N, '.'));

    for (int i = 0; i < board.size(); i++)
    {
        for (int j = 0; j < board[i].size(); j++)
        {
            board[i][j] = data[i][j];
        }
    }
    bool check = player.checkBoard(board);
    if (check)
        cout << "checked" << endl;

    player.solveSudoku(board);
    player.getResult();

    cout << endl;
}

vector<Board> readFile(string filePath)
{
    ifstream infile;
    vector<Board> boards;
    infile.open(filePath);
    char data[100];
    Board tmp;
    vector<char> row;
    while (!infile.eof())
    {
        infile.getline(data, 100);
        if (data[0] == '-')
        {
            boards.push_back(Board(tmp));
            tmp.clear();
            continue;
        }
        for (int i = 0; i < strlen(data); i++)
        {
            if (('1' <= data[i] && data[i] <= '9') || data[i] == '$')
            {
                row.push_back(data[i]);
            }
        }
        tmp.push_back(vector<char>(row));
        row.clear();
    }
    infile.close();
    return boards;
}

void writeFile(vector<Board> boards, ofstream &f)
{
    for (int k = 0; k < boards.size(); k++)
    {
        for (int i = 0; i < boards[k].size(); i++)
        {
            for (int j = 0; j < boards[k][i].size(); j++)
            {
                f << boards[k][i][j] << " ";
            }
            f << "\n";
        }
        f << "------- " << k << " -------" << endl;
    }
}

struct Options {
    int completeBoardCount = 0;
    string inputFile = "";
    int gameNumber = 0;
    int gameLevel = 0;
    vector<int> range;
    bool uniqueSolution = false;
};
Options parse(int argc, char *argv[]) {
    Options opts;
    int opt = 0;
    while ((opt = getopt(argc, argv, "c:s:n:m:r:u")) != -1)
    {
        opt = static_cast<unsigned char>(opt);
        switch (opt)
        {
        case 'c':
            opts.completeBoardCount = atoi(optarg);
            if (opts.completeBoardCount < 1 || opts.completeBoardCount > 1000000)
            {
                printf("生成数独终盘数量范围在1~1000000之间\n");
                exit(0);
            }
            break;
        case 's':
            opts.inputFile = string(optarg);
            if (access(optarg, 0) == -1)
            {
                printf("file does not exist\n");
                exit(0);
            }
            break;
        case 'n':
            opts.gameNumber = atoi(optarg);
            if (opts.gameNumber < 1 || opts.gameNumber > 10000)
            {
                printf("生成数独游戏数量范围在1~10000之间\n");
                exit(0);
            }
            break;
        case 'm':
            opts.gameLevel = atoi(optarg);
            if (opts.gameLevel < 1 || opts.gameLevel > 3)
            {
                printf("生成游戏难度的范围在1~3之间\n");
                exit(0);
            }
            if(opts.gameNumber == 0){
                printf("参数m必须与参数n一起使用\n");
                exit(0);
            }
            break;
        case 'r':
            char *p;
            p = strtok(optarg, "~");
            while (p)
            {
                opts.range.push_back(atoi(p));
                p = strtok(NULL, "~");
            }
            if (opts.range.size() != 2)
            {
                printf("请输入一个范围参数\n");
                exit(0);
            }
            if ((opts.range[0] >= opts.range[1]) || opts.range[0] < 20 || opts.range[1] > 55)
            {
                printf("请输入合法范围20~55\n");
                exit(0);
            }
            if(opts.gameNumber == 0){
                printf("参数r必须与参数n一起使用\n");
                exit(0);
            }
            break;
        case 'u':
            opts.uniqueSolution = true;
            if(opts.gameNumber == 0){
                printf("参数u必须与参数n一起使用\n");
                exit(0);
            }
            break;
        default:
            printf("请输入合法参数\n");
            exit(0);
            break;
        }
    }
    return opts;
}




void generateGame(int gameNumber, int gameLevel, vector<int> digCount, ofstream &outfile, SudokuPlayer &player)
{
    for (int i = 0; i < gameNumber; i++)
    {
        int cnt = 0;
        if (digCount.size() == 1)
        {
            cnt = digCount[0];
        }
        else
        {
            cnt = rand() % (digCount[1] - digCount[0] + 1) + digCount[0];
        }
        Board b = player.generateBoard(cnt);
        vector<Board> bs;
        bs.push_back(b);
        writeFile(bs, outfile);
    }
    outfile.close();
}
void generateGameU(int gameNumber, int gameLevel, const vector<int>& digCount, ofstream& outfile, SudokuPlayer& player)
{
    for (int i = 0; i < gameNumber; i++)
    {
        int cnt = 0;
        if (digCount.size() == 1)
        {
            cnt = digCount[0];
        }
        else
        {
            cnt = rand() % (digCount[1] - digCount[0] + 1) + digCount[0];
        }

        Board b;
        vector<Board> bs;

        while (true)
        {
            b = player.generateBoard(cnt);
            bs.clear();
            bs.push_back(b);

            vector<Board> solutions = player.solveSudoku(b);
            if (solutions.size() == 1)
            {
                break; // 唯一解，退出循环
            }
            // 否则重新生成数独
        }

        writeFile(bs, outfile);
    }

    outfile.close();
}

int main(int argc, char *argv[]) {
    srand((unsigned)time(NULL));
    SudokuPlayer player;

    Options opts = parse(argc, argv);

    vector<Board> boards;
    ofstream outfile;

    if (!opts.inputFile.empty()) {
        outfile.open("sudoku.txt", ios::out | ios::trunc);
        boards = readFile(opts.inputFile);
        for (int i = 0; i < boards.size(); i++) {
            vector<Board> result = player.solveSudoku(boards[i]);
            writeFile(result, outfile);
        }
        outfile.close();
    }

    if (opts.completeBoardCount > 0) {
        outfile.open("game.txt", ios::out | ios::trunc);
        opts.range.push_back(0);
        generateGame(opts.completeBoardCount, 0, opts.range, outfile, player);
        opts.range.clear();
    }

    if (opts.gameNumber > 0) {
        if (opts.range.empty()) {
            // 根据不同级别采取挖空数量不同
            if (opts.gameLevel == 1) {
                opts.range.push_back(20);
                opts.range.push_back(30);
            } else if (opts.gameLevel == 2) {
                opts.range.push_back(30);
                opts.range.push_back(40);
            } else if (opts.gameLevel == 3) {
                opts.range.push_back(40);
                opts.range.push_back(55);
            } else {
                opts.range.push_back(20);
                opts.range.push_back(55);
            }
        }

        outfile.open("game.txt", ios::out | ios::trunc);
        if(opts.uniqueSolution) generateGameU(opts.gameNumber, opts.gameLevel, opts.range, outfile, player);
        else generateGame(opts.gameNumber, opts.gameLevel, opts.range, outfile, player);
        opts.range.clear();
    }

    return 0;
}