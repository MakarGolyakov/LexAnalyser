#include <vector>
#include <string>


class authomata {
private:
	std::vector<char> alphabet;
	std::vector<std::vector<int>> transitFunction;
	int currentState;
	std::vector<std::string> lexemTypes = {
		"keyword",      //0
		"separator",    //1
		"operator",     //2
		"functionName", //3
		"const",        //4
		"function",     //5
		"var"           //6
	};
public:
	int getCurrentState() {
		return currentState;
	}

	std::vector<std::vector<int>> getTransFunc() {
		return transitFunction;
	}

	std::vector<std::string> getLexemTypes() {
		return lexemTypes;
	}

	authomata() {
		alphabet = { };
		transitFunction = { };
	}

	authomata(std::vector<char> alphabet, std::vector<std::vector<int>> transitFunction) {
		this->currentState = 0;
		this->alphabet = alphabet;
		this->transitFunction = transitFunction;
	}

	authomata(authomata& a) {
		this->currentState = a.currentState;
		this->alphabet = a.alphabet;
		this->transitFunction = a.transitFunction;
	}

	authomata operator = (authomata& a) {
		this->currentState = a.currentState;
		this->alphabet = a.alphabet;
		this->transitFunction = a.transitFunction;
	}

	int acceptChar(char a) {
		int res = -1;
		for (int i = 0; i < transitFunction[currentState].size(); ++i) {
			if (transitFunction[currentState][i] != -1) {
				if (alphabet[i] == a) {
					currentState = transitFunction[currentState][i];
					res = currentState;
					break;
				}
			}
		}
		return res;
	}

	std::string checkLexem() {
		//"keyword",      //0
		//"separator",    //1
		//"operator",     //2
		//"functionName", //3
		//"const",        //4
		//"function"      //5
		//"var"           //6
		if (currentState == 19 || currentState == 29 || currentState == 30) {
			return lexemTypes[0];
		}
		else if (currentState == 6) {
			return lexemTypes[1];
		}
		else if (currentState == 7) {
			return lexemTypes[2];
		}
		else if (currentState == 16) {
			return lexemTypes[3];
		}
		else if (currentState == 5 || currentState == 22) {
			return lexemTypes[4];
		}
		else if (currentState == 24 || currentState == 25) {
			return lexemTypes[5];
		}
		else {
			return lexemTypes[6];
		}
	}

	void setCurrentState(int state) {
		currentState = state;
		return;
	}
};