#pragma once
#include <string>
#include <vector>

class Node {
private:
	std::string lexem;
	std::vector<Node> children;

public:
	std::string type = "";
	int count = 0;
	std::string tr = "";
	Node(std::string lex) {
		lexem = lex;
	}

	Node() = default;
	
	void addNode(std::string lex) {
		Node nw(lex);
		children.push_back(nw);
		return;
	}

	Node& getNode(int iter) {
		return children[iter];
	}

	void changeLexem(std::string lex) {
		lexem = lex;
		return;
	}
};