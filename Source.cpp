#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <algorithm>
#include <chrono>
#include <time.h>

struct greedElement {
	int index;
	float value;
};

std::vector<std::vector<int>> generateElementData(int n) {
	std::vector<std::vector<int>> elementData(2, std::vector<int>(0));
	for (int i = 0; i < n; i++) {
		elementData[0].push_back((rand() % 30) + 1);
		elementData[1].push_back((rand() % 30) + 1);
	}
	return elementData;
}

bool selectNextCase(std::vector<bool>& caseData) {
	int i = caseData.size() - 1;
	while (true) {
		if (caseData[i] == false) {
			caseData[i] = true;
			return true;
		}
		else {
			if (i == 0) {
				return false;
			}
			caseData[i] = false;
		}
		i--;
	}
}

std::vector<bool> bruteforce(std::vector<std::vector<int>> elementData, int s) {
	std::vector<bool> caseData(elementData[0].size());
	std::vector<bool> resultData(elementData[0].size());
	int max = 0;

	while (selectNextCase(caseData)) {
		int caseSize = 0, caseValue = 0;
		for (int i = 0; i < caseData.size(); i++) {
			if (caseData[i]) {
				caseSize += elementData[0][i];
				caseValue += elementData[1][i];
			}
		}
		if (caseSize < s && caseValue > max) {
			resultData = caseData;
			max = caseValue;
		}
	}
	return resultData;
}

bool sortGreed(struct greedElement v1, struct greedElement v2) {
	return (v1.value) > (v2.value);
}

std::vector<bool> greedy(std::vector<std::vector<int>> elementData, int s) {
	std::vector<struct greedElement> quotientData(elementData[0].size());
	std::vector<bool> resultData(elementData[0].size());

	for (int i = 0; i < elementData[0].size(); i++) {
		quotientData[i].index = i;
		quotientData[i].value = elementData[1][i] / elementData[0][i];
	}
	sort(quotientData.begin(), quotientData.end(), sortGreed);

	for (int i = 0; i < quotientData.size(); i++) {
		if (s != 0) {
			if (s - elementData[0][quotientData[i].index] >= 0) {
				s -= elementData[0][quotientData[i].index];
				resultData[quotientData[i].index] = true;
			}
			else {
				continue;
			}
		}
		else {
			break;
		}
	}

	return resultData;
}

std::vector<bool> dynamic(std::vector<std::vector<int>> elementData, int s, int n) {
	std::vector<std::vector<int>> resultMatrix(n + 1, std::vector<int>(s + 1));
	std::vector<bool> resultData(elementData[0].size());

	for (int i = 1; i < resultMatrix.size(); i++) {
		for (int j = 1; j < s + 1; j++) {
			if (elementData[0][i - 1] > j) {
				resultMatrix[i][j] = resultMatrix[i - 1][j];
			}
			else {
				resultMatrix[i][j] = std::max(resultMatrix[i - 1][j], resultMatrix[i - 1][j - elementData[0][i - 1]] + elementData[1][i - 1]);
			}
		}
	}
	int i = resultMatrix.size() - 1, j = s;
	while (i != 0) {
		if (resultMatrix[i][j] != resultMatrix[i - 1][j]) {
			resultData[i - 1] = true;
			j -= elementData[0][i - 1];
			i -= 1;
		}
		else {
			i -= 1;
		}
	}
	
	return resultData;
}

int main() {
	srand(time(NULL));
	std::ofstream plik;
	plik.open("wyniki.txt");
	while (true) {
		int menuInput;
		std::cout << "1 - WPROWADZ DANE Z KLAWIATURY.\n";
		std::cout << "2 - WYKONAJ TESTY DLA LOSOWYCH DANYCH.\n";
		std::cin >> menuInput;
		if (menuInput == 1) {
			int s, n;
			std::vector<std::vector<int>> elementData(2, std::vector<int>(0));
			std::vector<bool> result;
			int input1, input2;
			std::cout << "PODAJ ROZMIAR PLECAKA I ILOSC ELEMENTOW.\n";
			std::cin >> s >> n;
			std::cout << "WPROWADZ ELEMENTY (ROZMIAR WARTOSC):\n";
			for (int i = 0; i < n; i++) {
				std::cin >> input1 >> input2;
				elementData[0].push_back(input1);
				elementData[1].push_back(input2);
			}
			std::cout << "WYNIK BRUTEFORCE:";
			result = bruteforce(elementData, s);
			for (int i = 0; i < result.size(); i++) {
				if (result[i] == 1) {
					std::cout << i + 1 << " ";
				}
			}
			std::cout << "\n";
			std::cout << "WYNIK ZACHLANNY:";
			result = greedy(elementData, s);
			for (int i = 0; i < result.size(); i++) {
				if (result[i] == 1) {
					std::cout << i + 1 << " ";
				}
			}
			std::cout << "\n";
			std::cout << "WYNIK DYNAMICZNY:";
			result = dynamic(elementData, s, n);
			for (int i = 0; i < result.size(); i++) {
				if (result[i] == 1) {
					std::cout << i + 1 << " ";
				}
			}
			break;
		}
		if (menuInput == 2) {
			std::cout << bool(plik.is_open());
			for (int s = 100; s <= 200; s += 10) {
				for (int n = 10; n <= 18; n += 1) {
					std::vector<std::vector<int>> caseVector;
					caseVector = generateElementData(n);

					auto start = std::chrono::high_resolution_clock::now();
					bruteforce(caseVector, s);
					auto end = std::chrono::high_resolution_clock::now();

					auto result = std::chrono::duration_cast<std::chrono::nanoseconds>(end - start);
					plik << std::to_string(s) << ", " << std::to_string(n) << ", " << std::to_string(result.count()) << ", B\n";

					start = std::chrono::high_resolution_clock::now();
					greedy(caseVector, s);
					end = std::chrono::high_resolution_clock::now();

					result = std::chrono::duration_cast<std::chrono::nanoseconds>(end - start);
					plik << std::to_string(s) << ", " << std::to_string(n) << ", " << std::to_string(result.count()) << ", G\n";

					start = std::chrono::high_resolution_clock::now();
					dynamic(caseVector, s, n);
					end = std::chrono::high_resolution_clock::now();

					result = std::chrono::duration_cast<std::chrono::nanoseconds>(end - start);
					plik << std::to_string(s) << ", " << std::to_string(n) << ", " << std::to_string(result.count()) << ", D\n";
				}
			}
			break;
		}
	}
	plik.close();
}
