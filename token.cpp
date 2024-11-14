#pragma once
#include <iostream>
#include <string>
#include <vector>

struct token {
	char* lexem;
	int lexemSize;
	char* type;
	int typeSize;

	token() {
		lexem = new char[0];
		type = new char[0];
		lexemSize = 0;
		typeSize = 0;
	}

	token(std::string& lex, std::string& tp) {
		lexemSize = lex.size();
		typeSize = tp.size();
		lexem = new char[lexemSize];
		type = new char[typeSize];
		for (int i = 0; i < lexemSize; ++i) {
			lexem[i] = lex[i];
		}
		for (int i = 0; i < typeSize; ++i) {
			type[i] = tp[i];
		}
	}
	~token() {
		delete[] lexem;
		delete[] type;
	}
	token(token& another) {
		this->lexemSize = another.lexemSize;
		this->typeSize = another.typeSize;
		this->lexem = new char[lexemSize];
		this->type = new char[typeSize];
		for (int i = 0; i < lexemSize; ++i) {
			this->lexem[i] = another.lexem[i];
		}
		for (int i = 0; i < typeSize; ++i) {
			this->type[i] = another.type[i];
		}
	}
	
	token operator = (const token& another) {
		this->lexemSize = another.lexemSize;
		this->typeSize = another.typeSize;
		this->lexem = new char[lexemSize];
		this->type = new char[typeSize];
		for (int i = 0; i < lexemSize; ++i) {
			this->lexem[i] = another.lexem[i];
		}
		for (int i = 0; i < typeSize; ++i) {
			this->type[i] = another.type[i];
		}
		return *this;
	}

	friend std::ostream& operator << (std::ostream& out, const token& tk) {
		out << "(";
		for (int i = 0; i < tk.lexemSize; ++i) {
			out << tk.lexem[i];
		}
		out << ", ";
		for (int i = 0; i < tk.typeSize; ++i) {
			out << tk.type[i];
		}
		out << ")";
		return out;
	}

	bool operator == (const token& b) {
		bool flag = true;
		if (lexemSize != b.lexemSize || b.typeSize != typeSize) {
			flag = false;
		}
		else {
			for (int i = 0; i < lexemSize; ++i) {
				if (lexem[i] != b.lexem[i]) {
					flag = false;
					break;
				}
			}
			for (int i = 0; i < typeSize; ++i) {
				if (type[i] != b.type[i]) {
					flag = false;
					break;
				}
			}
		}
		return flag;
	}
};