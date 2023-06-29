#include <gtest/gtest.h>
#include <fstream>
#include <vector>
#include <sstream>
#include "sudoku_functions.h"

// 测试 generateGame 函数
TEST(SudokuTest, GenerateGame)
{
    SudokuPlayer player;
    std::ofstream outfile("test_game.txt");
    std::vector<int> digCount = {30, 40};
    generateGame(1, 2, digCount, outfile, player);
    outfile.close();

    // 检查生成的游戏数量是否符合预期
    std::ifstream infile("test_game.txt");
    std::string line;
    int gameCount = 0;
    bool isInGameSection = false;
    while (std::getline(infile, line))
    {
        if (line == "------- 0 -------")
        {
            // 游戏分隔符
            isInGameSection = true;
            gameCount++;
        }
        else if (isInGameSection && !line.empty())
        {
            // 非空行表示游戏内容
            std::istringstream iss(line);
            std::string cellValue;
            while (iss >> cellValue)
            {
                // 检查数独单元格的值是否合法
                int value = std::stoi(cellValue);
                ASSERT_GE(value, 0);
                ASSERT_LE(value, 9);
            }
        }
    }
    infile.close();

    ASSERT_EQ(gameCount, 1);
}
TEST(SudokuPlayerTest, SolveSudoku)
{
    // 创建一个 SudokuPlayer 对象
    SudokuPlayer player;

    // 创建一个需要解决的数独棋盘
    Board board = {
        {'5', '3', '$', '$', '7', '$', '$', '$', '$'},
        {'6', '$', '$', '1', '9', '5', '$', '$', '$'},
        {'$', '9', '8', '$', '$', '$', '$', '6', '$'},
        {'8', '$', '$', '$', '6', '$', '$', '$', '3'},
        {'4', '$', '$', '8', '$', '3', '$', '$', '1'},
        {'7', '$', '$', '$', '2', '$', '$', '$', '6'},
        {'$', '6', '$', '$', '$', '$', '2', '8', '$'},
        {'$', '$', '$', '4', '1', '9', '$', '$', '5'},
        {'$', '$', '$', '$', '8', '$', '$', '7', '9'}
    };

    // 调用 SudokuPlayer 的 solveSudoku 函数解决数独
    std::vector<Board> solutions = player.solveSudoku(board);

    // 断言解决方案的数量为 1，即解决方案是唯一的
    EXPECT_EQ(solutions.size(), 1);

    // TODO: 可以添加更多的断言来验证解决方案的正确性
}
TEST(SudokuPlayerTest, AddResult)
{
    // 创建一个 SudokuPlayer 对象
    SudokuPlayer player;

    // 创建一个棋盘对象
    Board board = {
        {'1', '2', '3', '4', '5', '6', '7', '8', '9'},
        {'4', '5', '6', '7', '8', '9', '1', '2', '3'},
        {'7', '8', '9', '1', '2', '3', '4', '5', '6'},
        {'2', '3', '4', '5', '6', '7', '8', '9', '1'},
        {'5', '6', '7', '8', '9', '1', '2', '3', '4'},
        {'8', '9', '1', '2', '3', '4', '5', '6', '7'},
        {'3', '4', '5', '6', '7', '8', '9', '1', '2'},
        {'6', '7', '8', '9', '1', '2', '3', '4', '5'},
        {'9', '1', '2', '3', '4', '5', '6', '7', '8'}
    };

    // 调用 SudokuPlayer 的 addResult 函数
    player.addResult(board);

    // 获取结果集合
    vector<Board> result = player.getResult();

    // 验证结果集合是否包含刚刚添加的棋盘对象
    EXPECT_EQ(result.size(), 1);
    EXPECT_EQ(result[0], board);
}
TEST(CheckBoardTest, ValidBoard) {
    // 创建一个数独棋盘
    SudokuPlayer player;
    Board board = {
        {'5', '3', '.', '.', '7', '.', '.', '.', '.'},
        {'6', '.', '.', '1', '9', '5', '.', '.', '.'},
        {'.', '9', '8', '.', '.', '.', '.', '6', '.'},
        {'8', '.', '.', '.', '6', '.', '.', '.', '3'},
        {'4', '.', '.', '8', '.', '3', '.', '.', '1'},
        {'7', '.', '.', '.', '2', '.', '.', '.', '6'},
        {'.', '6', '.', '.', '.', '.', '2', '8', '.'},
        {'.', '.', '.', '4', '1', '9', '.', '.', '5'},
        {'.', '.', '.', '.', '8', '.', '.', '7', '9'}
    };

    // 调用 checkBoard 函数进行测试
    bool result = player.checkBoard(board);

    // 使用断言检查返回结果是否为 true，即数独棋盘有效
    ASSERT_TRUE(result);
}

TEST(CheckBoardTest, InvalidBoard) {
    // 创建一个数独棋盘（无效）
    SudokuPlayer player;
    Board board = {
        {'5', '3', '.', '.', '7', '.', '.', '.', '.'},
        {'6', '.', '.', '1', '9', '5', '.', '.', '.'},
        {'.', '9', '8', '.', '.', '.', '.', '6', '.'},
        {'8', '.', '.', '.', '6', '.', '.', '.', '3'},
        {'4', '.', '.', '8', '.', '3', '.', '.', '1'},
        {'7', '.', '.', '.', '2', '.', '.', '.', '6'},
        {'.', '6', '.', '.', '.', '.', '2', '8', '.'},
        {'.', '.', '.', '4', '1', '9', '.', '.', '5'},
        {'.', '.', '.', '.', '8', '.', '8', '7', '9'} // 重复的数字 '8'
    };

    // 调用 checkBoard 函数进行测试
    bool result = player.checkBoard(board);

    // 使用断言检查返回结果是否为 false，即数独棋盘无效
    ASSERT_FALSE(result);
}
// 声明测试用例
TEST(GetRand9Test, RandomOrder) {
    SudokuPlayer player;
    // 调用 getRand9 函数进行测试
    std::vector<int> result = player.getRand9();

    // 使用断言检查返回结果是否为一个从0到8的随机排列向量
    std::vector<int> expected = {0, 1, 2, 3, 4, 5, 6, 7, 8};
    ASSERT_NE(result, expected); // 检查结果不等于预期的有序向量
}
// 执行所有测试用例
int countFilledCells(const Board& board) {
    int count = 0;
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            if (board[i][j] != '$' && board[i][j] != '.') {
                count++;
            }
        }
    }
    return count;
}
TEST(GenerateBoardTest, ValidBoard) {
    SudokuPlayer player;
    // 调用 generateBoard 函数进行测试
    Board result = player.generateBoard(40);

    // 使用断言检查生成的数独棋盘是否满足条件
    ASSERT_TRUE(player.checkBoard(result)); // 检查生成的数独棋盘是否合法
    ASSERT_EQ(countFilledCells(result), 81-40); // 检查生成的数独棋盘中已填数字的数量是否正确
}
TEST(GenerateBoardTest, InvalidBoard) {
    SudokuPlayer player;
    // 调用 generateBoard 函数进行测试
    Board result = player.generateBoard(20);

    // 使用断言检查生成的数独棋盘是否满足条件
    ASSERT_TRUE(player.checkBoard(result)); // 检查生成的数独棋盘是否合法
    ASSERT_NE(countFilledCells(result), 20); // 检查生成的数独棋盘中已填数字的数量是否正确
}

TEST(CopySquareTest, ValidCopy)
{
    SudokuPlayer player;
    Board board(N, vector<char>(N, '$')); // 创建一个空的数独棋盘

    vector<int> row = player.getRand9();
    for (int i = 0; i < 3; i++)
    {
        board[3][i + 3] = row[i] + '1';
        board[4][i + 3] = row[i + 3] + '1';
        board[5][i + 3] = row[i + 6] + '1';
    }
    // 调用 copySquare 函数进行测试
    player.copySquare(board, 3, 3, true); // 复制行方向的方块

    // 使用断言检查复制结果是否正确
    // 根据具体的复制逻辑和期望结果进行断言检查
    // ...

    // 示例断言检查，检查复制后的棋盘中某个位置的值是否符合预期
    ASSERT_NE(board[3][3], '$'); // 检查复制后的棋盘中特定位置的值是否正确
}
TEST(CopySquareTest, InvalidCopy)
{
    SudokuPlayer player;
    Board board(N, vector<char>(N, '$')); // 创建一个空的数独棋盘

    vector<int> row = player.getRand9();
    for (int i = 0; i < 3; i++)
    {
        board[3][i + 3] = row[i] + '1';
        board[4][i + 3] = row[i + 3] + '1';
        board[5][i + 3] = row[i + 6] + '1';
    }
    // 调用 copySquare 函数进行测试
    player.copySquare(board, 3, 3, true); // 复制行方向的方块

    // 使用断言检查复制结果是否正确
    // 根据具体的复制逻辑和期望结果进行断言检查
    // ...

    // 示例断言检查，检查复制后的棋盘中某个位置的值是否符合预期
    ASSERT_EQ(board[0][3], '$'); // 检查复制后的棋盘中特定位置的值是否正确
}
int main(int argc, char* argv[])
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
