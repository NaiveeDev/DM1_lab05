#include <iostream>
#include <vector>
#include <bitset>
#include <Windows.h>
#include <string>
#include <algorithm>


using std::vector;
using std::cout; using std::endl;

std::vector<int> evaluateFunction(const std::vector<std::vector<int>>& truthTable) {
    std::vector<int> F;
    for (const auto& row : truthTable) {
        int x = row[0];
        int y = row[1];
        int z = row[2];
        int result = row[3]; 
        F.push_back(result);
    }
    return F;
}

void printTruthTable(const std::vector<std::vector<int>>& truthTable, const std::vector<int>& F) {
    std::cout << "x  y  z  F" << std::endl;
    for (size_t i = 0; i < truthTable.size(); ++i) {
        for (size_t j = 0; j < truthTable[i].size(); ++j) {
            std::cout << truthTable[i][j] << "  ";
        }
        std::cout << F[i] << std::endl;
    }
}

std::string findDualFunction(const std::vector<std::vector<int>>& truthTable, const std::vector<int>& F) {
    std::string dualFunction = "";
    for (size_t i = 0; i < truthTable.size(); ++i) {
        if (F[i] == 0) {
            std::bitset<3> binary(i); 
            dualFunction += "(";
            for (size_t j = 0; j < binary.size(); ++j) {
                if (binary[j] == 0) {
                    dualFunction += "x" + std::to_string(j + 1) + " + ";
                }
                else {
                    dualFunction += "!x" + std::to_string(j + 1) + " + ";
                }
            }
            dualFunction = dualFunction.substr(0, dualFunction.length() - 3);
            dualFunction += ") * ";
        }
    }
    dualFunction = dualFunction.substr(0, dualFunction.length() - 3);
    return dualFunction;
}

std::string generateZhegalkinPolynomial(const std::vector<int>& F) {
    std::string polynomial = "";
    for (size_t i = 0; i < F.size(); ++i) {
        if (F[i] == 1) {
            std::bitset<3> binary(i); 
            polynomial += "(";
            for (size_t j = 0; j < binary.size(); ++j) {
                if (binary[j] == 0) {
                    polynomial += "x" + std::to_string(j + 1) + " * ";
                }
                else {
                    polynomial += "!x" + std::to_string(j + 1) + " * ";
                }
            }
            polynomial = polynomial.substr(0, polynomial.length() - 3); 
            polynomial += ") + ";
        }
    }
    polynomial = polynomial.substr(0, polynomial.length() - 3);
    return polynomial;
}

std::string decimalToBinary(int decimal, int numVars) {
    std::string binary;
    for (int i = numVars - 1; i >= 0; --i) {
        binary += (decimal & (1 << i)) ? '1' : '0';
    }
    return binary;
}

std::string buildDNF(const std::vector<int>& F, int numVars) {
    std::string dnf = "";
    for (size_t i = 0; i < F.size(); ++i) {
        if (F[i] == 1) {
            std::string binary = decimalToBinary(i, numVars);
            dnf += "(";
            for (size_t j = 0; j < binary.size(); ++j) {
                if (binary[j] == '0') {
                    dnf += "!x" + std::to_string(j + 1) + " v ";
                }
                else {
                    dnf += "x" + std::to_string(j + 1) + " v ";
                }
            }
            dnf = dnf.substr(0, dnf.length() - 3);
            dnf += ") ^ ";
        }
    }
    if (!dnf.empty()) {
        dnf = dnf.substr(0, dnf.length() - 3);
    }
    return dnf;
}

std::string buildKNF(const std::vector<int>& F, int numVars) {
    std::string knf = "";
    for (size_t i = 0; i < F.size(); ++i) {
        if (F[i] == 0) {
            std::string binary = decimalToBinary(i, numVars);
            knf += "(";
            for (size_t j = 0; j < binary.size(); ++j) {
                if (binary[j] == '0') {
                    knf += "!x" + std::to_string(j + 1) + " ^ ";
                }
                else {
                    knf += "x" + std::to_string(j + 1) + " ^ ";
                }
            }
            knf = knf.substr(0, knf.length() - 3);
            knf += ") v ";
        }
    }
    if (!knf.empty()) {
        knf = knf.substr(0, knf.length() - 3);
    }
    return knf;
}



bool isConstantZero(const std::vector<int>& F) {
    return std::all_of(F.begin(), F.end(), [](int val) { return val == 0; });
}

bool isConstantOne(const std::vector<int>& F) {
    return std::all_of(F.begin(), F.end(), [](int val) { return val == 1; });
}



bool isMonotonic(const std::vector<int>& F, int numVars) {
    for (int i = 0; i < (1 << numVars); ++i) {
        for (int j = i + 1; j < (1 << numVars); ++j) {
            if (F[i] > F[j]) {
                std::bitset<3> binaryI(i);
                std::bitset<3> binaryJ(j);
                bool nonDecreasing = true;
                for (int k = 0; k < numVars; ++k) {
                    if (binaryI[k] > binaryJ[k]) {
                        nonDecreasing = false;
                        break;
                    }
                }
                if (!nonDecreasing) {
                    return false;
                }
            }
        }
    }
    return true;
}

bool isLinear(const std::vector<int>& F, int numVars) {
    for (int i = 0; i < (1 << numVars); ++i) {
        for (int j = i + 1; j < (1 << numVars); ++j) {
            if (F[i] != F[j]) {
                std::bitset<3> binaryI(i);
                std::bitset<3> binaryJ(j);
                std::bitset<3> xorResult = binaryI ^ binaryJ;
                int countOnes = xorResult.count();
                if (countOnes != 1) {
                    return false;
                }
            }
        }
    }
    return true;
}

int main() {
    

    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);
    setlocale(LC_ALL, "ru");

    const std::vector<int> F = { 0, 0, 1, 0, 0, 1, 0, 0 };
    std::vector<std::vector<int>> truthTable = {
        {0, 0, 0, 0},
        {0, 0, 1, 0},
        {0, 1, 0, 1},
        {0, 1, 1, 0},
        {1, 0, 0, 0},
        {1, 0, 1, 1},
        {1, 1, 0, 0},
        {1, 1, 1, 0}
    };

    int numVars = 3;
    cout << "Таблиця істинності:";
    printTruthTable(truthTable, F);

    std::string dualFunction = findDualFunction(truthTable, F);
    std::cout << "\nФункція двоїсту: " << dualFunction << std::endl;



    std::string dnf = buildDNF(F, numVars);
    std::cout << "\nДНФ: " << dnf << std::endl;

    std::string knf = buildKNF(F, numVars);
    std::cout << "\nКНФ: " << knf << std::endl;


    std::string zhegalkinPolynomial = generateZhegalkinPolynomial(F);
    std::cout << "\nПоліном Жегалкіна: " << zhegalkinPolynomial << std::endl;

    bool isZeroConstant = isConstantZero(F);
    bool isOneConstant = isConstantOne(F);
    bool isMonotonicFunction = isMonotonic(F, numVars);
    bool isLinearFunction = isLinear(F, numVars);

    std::cout << "Is Zero Constant: " << (isZeroConstant ? "Так" : "Ні") << std::endl;
    std::cout << "Is One Constant: " << (isOneConstant ? "Так" : "Ні") << std::endl;
    std::cout << "Is Monotonic: " << (isMonotonicFunction ? "Так" : "Ні") << std::endl;
    std::cout << "Is Linear: " << (isLinearFunction ? "Так" : "Ні") << std::endl;

    return 0;
}
