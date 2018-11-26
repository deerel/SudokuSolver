//***************************************************************************//
//               Sudoku Solver, by Rickard Lundberg 2018                     //
//                                                                           //
// The applications solves sudokus. A file containing sudoku quizes must be  //
// provided. The format of each quiz must be all numbers in a single line,   //
// no commas or extra delimiter characters must be added. The first line in  //
// the quiz file will be ignored.                                            //
// For quizzes see https://www.kaggle.com/bryanpark/sudoku                   //
// All solutions will be presented as a single line in a text file named     //
// output.txt after a successfull execution.                                 //
//***************************************************************************//

#include <iostream>
#include <sstream>
#include <fstream>
#include <chrono>
#include <vector>

namespace sudokusolver {

struct field;
bool parse(const unsigned pos, const field& quiz, field& solution);
bool isPossible(const unsigned number, unsigned pos, const field& quiz, field& solution);

struct field
{
	unsigned pos[81]{ 0 };

	/**
		Writes the array pos as a single line to a file.

		@param output_file The output stream (file) to print to.
	*/
	void writeToFile(std::ofstream& output_file) const {
		char s[82];
		for (size_t i = 0; i < 81; ++i) {
			s[i] = pos[i] + 48;
		}
		s[81] = '\n';
		output_file.write(s, 82);
	}

	std::string toString() const {
		char s[83];
		for (size_t i = 0; i < 81; ++i) {
			s[i] = pos[i] + 48;
		}
		s[81] = '\n';
		s[82] = '\0';
		return std::string(s);
	}
};

/**
	A recursive function that tries to solve a sudoku.

	@param index The index of the current position.
	@param quiz The initial sudoku board.
	@param solution The current state of the solution sudoku board.
	@return True if it is possible, false otherwise.
*/
bool parse(const unsigned index, const field& quiz, field& solution)
{
	unsigned number = 1;
	bool foundNumber = false;
	bool nextSucceeded = false;

	// Base case, the last position in the sudoku board
	if (index == 80) {
		if (quiz.pos[index] != 0) {
			solution.pos[index] = quiz.pos[index];
			return true;
		}
		while (!foundNumber) {
			if (number > 9) {
				solution.pos[index] = 0;
				return false;
			}
			foundNumber = isPossible(number, index, quiz, solution);
			++number;
		}
		// A number was found, subtract with 1 due to previous increment.
		solution.pos[index] = number - 1;
		return true;
	}
	// Special case, a number is supplied by the initial quiz.
	else if (quiz.pos[index] != 0) {
		solution.pos[index] = quiz.pos[index];
		return parse(index + 1, quiz, solution);
	}
	// Recursive part, place a possible number at the current position and
	// call parse() on the nest position.
	else {
		while (!nextSucceeded) {
			foundNumber = isPossible(number, index, quiz, solution);
			if (foundNumber) {
				solution.pos[index] = number;
				nextSucceeded = parse(index + 1, quiz, solution);
				if (nextSucceeded) {
					return true;
				}
			}
			++number;
			if (number > 9) {
				solution.pos[index] = 0;
				return false;
			}
		}
	}

	return true;
}

/**
	Checks is a number can be places at a selected position

	@param number The number to check.
	@param index The index of the position the number is places in.
	@param quiz The initial sudoku board.
	@param solution The current state of the solution sudoku board.
	@return True if it is possible, false otherwise.
*/
bool isPossible(const unsigned number, const unsigned index, const field& quiz, field& solution)
{

	// Horizontal
	const unsigned horStart = (index / 9) * 9;
	for (unsigned i = horStart; i < horStart + 9; ++i)
	{
		if (solution.pos[i] == number || quiz.pos[i] == number) {
			return false;
		}
	}
	// Vertical
	const unsigned vertStart = index % 9;
	for (unsigned i = vertStart; i < vertStart + 81; i += 9)
	{
		if (solution.pos[i] == number || quiz.pos[i] == number) {
			return false;
		}
	}
	// Square
	unsigned squareStart = 0;
	// Find the section start index
	if (index > 26) squareStart = 27;
	if (index > 53) squareStart = 54;
	squareStart += ((index / 3) % 3) * 3;
	for (unsigned i = 0; i < 3; ++i)
	{
		for (unsigned j = 0; j < 3; ++j)
		{
			const unsigned sIndex = squareStart + i * 9 + j;
			if (solution.pos[sIndex] == number || quiz.pos[sIndex] == number) {
				return false;
			}
		}
	}

	return true;
}
}

int main()
{
	using namespace std::chrono;
	using namespace sudokusolver;

	high_resolution_clock::time_point t1 = high_resolution_clock::now();
	std::ifstream input_file;
	input_file.open("sudoku.csv");

	std::ofstream output_file;
	output_file.open("output.txt");

	if (!input_file) {
		std::cerr << "Unable to open file";
		exit(1);
	}

	if (!output_file) {
		std::cerr << "Unable to open file";
		exit(1);
	}

	std::string textLine;
	field sudokuInput;

	// Skip first line
	getline(input_file, textLine);

	// Solve sudoku
	//while (getline(input_file, textLine)) {
	//	for (size_t i = 0; i < 81; ++i) {
	//		sudokuInput.pos[i] = textLine[i]-48;
	//	}
	//	field sudokuOutput;
	//	parse(0, sudokuInput, sudokuOutput);
	//	sudokuOutput.writeToFile(output_file);
	//}
	std::vector<std::string> vecSolution(1000000);
	std::vector<field> vecQuiz(1000000);

	for (int i = 0; i < 1000000; ++i) {
		getline(input_file, textLine);
		for (int j = 0; j < 81; ++j) {
			vecQuiz.at(i).pos[j] = textLine[j]-48;
		}
	}

	#pragma omp parallel for 
	for (int i = 0; i < 1000000; ++i) {
		field sudokuOutput;
		parse(0, vecQuiz.at(i), sudokuOutput);
		vecSolution.at(i) = sudokuOutput.toString();
		int x = 1;
	}

	for (int i = 0; i < 1000000; ++i) {
		output_file.write(vecSolution.at(i).c_str(), 82);
	}

	output_file.flush();
	high_resolution_clock::time_point t2 = high_resolution_clock::now();
	output_file.close();
	input_file.close();
	std::cout << "Done!\n";
	duration<double> time_span = duration_cast<duration<double>>(t2 - t1);

	std::cout << "It took me " << time_span.count() << " seconds.";
	std::cout << std::endl;
	std::cin.get();
	return 0;
}