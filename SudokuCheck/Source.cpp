#include <iostream>
#include <sstream>
#include <fstream>
#include <vector>

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

	int count = 0;
	std::vector<std::string> vecSolution(1000000), vecAnswers(1000000);

	for (int i = 0; i < 1000000; ++i) {
		getline(answers, answerLine);
		getline(correctSolution, solutionLine);
		vecAnswers.at(i) = answerLine;
		vecSolution.at(i) = solutionLine.substr(82, 82);
	}

	bool isCorrect = true;
	#pragma omp parallel for 
	for (int i = 0; i < 1000000; ++i) {
		#pragma omp critical
		count++;
		if (vecAnswers.at(i) != vecSolution.at(i)) {
			std::cout << "Error at row " << i << "\n";
			std::cout << "Got: " << vecAnswers.at(i) << "\n";
			std::cout << "Exp: " << vecSolution.at(i) << "\n";
			isCorrect = false;
		}
	}

	std::cout << "Made it to number " << count << std::endl;

	if (isCorrect) {
		std::cout << "All correct!" << std::endl;
	}
	else {
		std::cout << "Not ok!" << std::endl;
	}
		

	//while (getline(answers, answerLine)) {
	//	std::cout << ++count << std::endl;
	//	if (!getline(correctSolution, solutionLine)) {
	//		std::cerr << "Error while reading file sudoku.csv";
	//		exit(1);
	//	}

	//	bool isCorrect = true;
	//	if (solutionLine.substr(82, 81) != answerLine) {
	//		std::cout << "Failed\n Answer:   " << answerLine << "\nSolution: " << solutionLine.substr(82, 81) << std::endl;
	//		std::cout << "Aborting check." << std::endl;
	//		std::cin.get();
	//		return 0;
	//	}
	//	//for (size_t i = 0; i < 81; ++i) {
	//	//	if (solutionLine[i + 82] != answerLine[i]) {
	//	//		std::cout << "Failed\n Answer:   " << answerLine << "\nSolution: " << solutionLine << std::endl;
	//	//		std::cout << "Aborting check." << std::endl;
	//	//		std::cin.get();
	//	//		return 0;
	//	//	}
	//	//}
	//}

	//std::cout << "All correct!" << std::endl;
	std::cin.get();
	return 0;
}