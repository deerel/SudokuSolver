#include <iostream>
#include <sstream>
#include <fstream>

int main() {

	std::cout << "Put a file with your anwers in the same directory as this program." << std::endl;
	std::cout << "Rename the file answers.txt." << std::endl;
	std::cout << "Hit Enter to execute." << std::endl;
	std::cin.get();

	std::ifstream correctSolution;
	correctSolution.open("sudoku.csv");

	std::ifstream answers;
	answers.open("answers.txt");

	if (!correctSolution) {
		std::cerr << "Unable to open file sudoku.csv";
		std::cin.get();
		exit(1);
	}

	if (!answers) {
		std::cerr << "Unable to open file answers.txt";
		std::cin.get();
		exit(1);
	}
	std::string solutionLine, answerLine;

	getline(correctSolution, solutionLine);

	while (getline(answers, answerLine)) {
		if (!getline(correctSolution, solutionLine)) {
			std::cerr << "Error while reading file sudoku.csv";
			exit(1);
		}
		bool isCorrect = true;
		for (size_t i = 0; i < 81; ++i) {
			if (solutionLine[i + 82] != answerLine[i]) {
				std::cout << "Failed\n Answer:   " << answerLine << "\nSolution: " << solutionLine << std::endl;
				std::cout << "Aborting check." << std::endl;
				std::cin.get();
				return 0;
			}
		}
	}

	std::cout << "All correct!" << std::endl;
	std::cin.get();
	return 0;
}