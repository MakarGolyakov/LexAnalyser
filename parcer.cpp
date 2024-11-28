#pragma once
#include <string>
#include <iostream>
#include "authomata.cpp"
#include <fstream>
#include <exception>
#include <sstream>
#include "Node.cpp"

class parcer {
private:
	std::ofstream fout{ "E:/Рабочий стол/Все лабы/лаба ямп/ввод вывод/SyntOutput.txt" };
	std::string lexem;
	authomata a;
	int level = 0;
	void output(std::string lex, Node& n) {
		for (int i = 0; i < level; ++i) {
			fout << '\t';
		}
		fout << lex << std::endl;
		/*
		if (lex == "Function") {
			n.changeLexem("Function");
			return;
		}
		n.addNode(lex);
		*/
	}
	void changeLexem(std::string lex) {
		lexem = lex;
		return;
	}
	bool isID() {
		int letters = 0;
		int numbers = 0;
		int type = 0;
		for (int a : lexem) {
			if ((a >= 65 && a <= 90) || (a >= 97 && a <= 122)) {
				if (type == 1) {
					return false;
				}
				++letters;
			}
			if (a >= 48 && a <= 57) {
				if (type == 0 && letters == 0) {
					return false;
				}
				type = 1;
				++numbers;
			}
		}
		return true;
	}

	bool isIntNum() {
		for (int a : lexem) {
			if (a < 48 || a > 57) {
				return false;
			}
		}
		return true;
	}

	bool isDoubleNum() {
		for (int a : lexem) {
			bool separator = false;
			int numLen = 0;
			if ((a >= 48 && a <= 57) || a == 46) {
				if (a == 44) {
					if (numLen == 0) {
						return false;
					}
					if (separator) {
						return false;
					}
					separator = true;
				}
				++numLen;
			}
			else {
				return false;
			}
		}
		return true;
	}

	void Function(Node& n) {
		++level;
		if (lexem == "int" || lexem == "double") {
			output(std::string("Begin"), n);
			n.addNode("Begin");
			Begin(n.getNode(0));
		}
		else {
			std::string ex = "wrong lexem \nFound: " + lexem + " position: " + std::to_string(a.getLexemPosition());
			throw std::runtime_error(ex);
		}
		if (lexem == "int" || lexem == "double") {
			//std::cout << "Descriptions" << std::endl;
			output(std::string("Descriptions"), n);
			n.addNode("Descroptions");
			Descriptions(n.getNode(1));
		}
		else {
			std::string ex = "wrong lexem \nFound: " + lexem + " position: " + std::to_string(a.getLexemPosition());
			throw std::runtime_error(ex);
		}
		if (isID()) {
			//std::cout << "Operators" << std::endl;
			output(std::string("Operators"), n);
			n.addNode("Operators");
			Operators(n.getNode(2));
		}
		else {
			std::string ex = "wrong lexem \nFound: " + lexem + " position: " + std::to_string(a.getLexemPosition());
			throw std::runtime_error(ex);
		}
		if (lexem == "return") {
			//std::cout << "End" << std::endl;
			output(std::string("End"), n);
			n.addNode("End");
			End(n.getNode(3));
		}
		else {
			std::string ex = "wrong lexem \nFound: " + lexem + " position: " + std::to_string(a.getLexemPosition());
			throw std::runtime_error(ex);
		}
		--level;
	}

	void Begin(Node& n) {
		++level;
		if (lexem == "int" || lexem == "double") {
			//std::cout << "Type" << std::endl;
			output(std::string("Type"), n);
			n.addNode("Type");
			Type(n.getNode(0));
			if (isID()) {
				//std::cout << "FunctionName" << std::endl;
				output(std::string("FunctionName"), n);
				n.addNode("FunctionName");
				FunctionName(n.getNode(1));
				if (lexem == "{") {
					output(std::string("{"), n);
					n.addNode("{");
					lexem = a.nextLexem();
				}
				else {
					std::string ex = "wrong lexem \nFound: " + lexem + " position: " + std::to_string(a.getLexemPosition());
					throw std::runtime_error(ex);
				}
			}
			else {
				; std::string ex = "wrong lexem \nFound: " + lexem + " position: " + std::to_string(a.getLexemPosition());
				throw std::runtime_error(ex);
			}
		}
		else {
			std::string ex = "wrong lexem \nFound: " + lexem + " position: " + std::to_string(a.getLexemPosition());
			throw std::runtime_error(ex);
		}
		--level;
	}

	void End(Node& n) {
		++level;
		if (lexem == "return") {
			//std::cout << "return" << std::endl;
			output(std::string("return"), n);
			n.addNode("return");
			lexem = a.nextLexem();
			if (isID()) {
				//std::cout << "Id" << std::endl;
				output(std::string("Id"), n);
				n.addNode("Id");
				Id(n.getNode(1));
				if (lexem == ";") {
					//std::cout << ";" << std::endl;
					output(std::string(";"), n);
					n.addNode(";");
					lexem = a.nextLexem();
					if (lexem == "}") {
						//std::cout << "}" << std::endl;
						output(std::string("}"), n);
						n.addNode("}");
					}
					else {
						std::string ex = "wrong lexem \nFound: " + lexem + " position: " + std::to_string(a.getLexemPosition());
						throw std::runtime_error(ex);
					}
				}
				else {
					std::string ex = "wrong lexem \nFound: " + lexem + " position: " + std::to_string(a.getLexemPosition());
					throw std::runtime_error(ex);
				}
			}
			else {
				std::string ex = "wrong lexem \nFound: " + lexem + " position: " + std::to_string(a.getLexemPosition());
				throw std::runtime_error(ex);
			}
		}
		else {
			std::string ex = "wrong lexem \nFound: " + lexem + " position: " + std::to_string(a.getLexemPosition());
			throw std::runtime_error(ex);
		}
		--level;
	}

	void FunctionName(Node& n) {
		++level;
		if (isID()) {
			output(std::string("Id"), n);
			n.addNode("Id");
			Id(n.getNode(0));
		}
		--level;
	}

	void Descriptions(Node& n) {
		++level;
		if (lexem == "int" || lexem == "double") {
			output(std::string("Descr"), n);
			n.addNode("Descr");
			//std::cout << "Descr" << std::endl;
			Descr(n.getNode(0));
			if (lexem == ";") {
				lexem = a.nextLexem();
				//n.addNode(";");
				//std::cout << "Descriptions'" << std::endl;
				output(std::string("Descriptions'"), n);
				n.addNode("Descroptions'");
				Descriptions1(n.getNode(1));
			}
			else {
				std::string ex = "wrong lexem \nFound: " + lexem + " position: " + std::to_string(a.getLexemPosition());
				throw std::runtime_error(ex);
			}
		}
		else {
			std::string ex = "wrong lexem \nFound: " + lexem + " position: " + std::to_string(a.getLexemPosition());
			throw std::runtime_error(ex);
		}
		--level;
	}

	void Descriptions1(Node& n) {
		++level;
		if (lexem == "int" || lexem == "double") {
			//std::cout << "Descriptions" << std::endl;
			output(std::string("Descriptions"), n);
			n.addNode("Descroptions");
			Descriptions(n.getNode(0));
		}
		else if (isID()) {
			output(std::string("eps"), n);
			n.addNode("eps");
			//std::cout << "eps" << std::endl;
		}
		else {
			std::string ex = "wrong lexem \nFound: " + lexem + " position: " + std::to_string(a.getLexemPosition());
			throw std::runtime_error(ex);
		}
		--level;
	}

	void Operators(Node& n) {
		++level;
		if (isID()) {
			//std::cout << "Op" << std::endl;
			output(std::string("Op"), n);
			n.addNode("Op");
			Op(n.getNode(0));
			if (lexem == ";") {
				output(std::string(";"), n);
				//n.addNode(";");
				//std::cout << "Operators'" << std::endl;
				lexem = a.nextLexem();
				output(std::string("Operators'"), n);
				n.addNode("Operators'");
				Operators1(n.getNode(1));
			}
			else {
				std::string ex = "wrong lexem \nFound: " + lexem + " position: " + std::to_string(a.getLexemPosition());
				throw std::runtime_error(ex);
			}
		}
		else {
			std::string ex = "wrong lexem \nFound: " + lexem + " position: " + std::to_string(a.getLexemPosition());
			throw std::runtime_error(ex);
		}
		--level;
	}

	void Operators1(Node& n) {
		++level;
		if (lexem == "return") {
			output(std::string("eps"), n);
			n.addNode("eps");
			//std::cout << "eps" << std::endl;
		}
		else if (isID()) {
			output(std::string("Operators"), n);
			//std::cout << "Operators" << std::endl;
			n.addNode("Operators");
			Operators(n.getNode(0));
		}
		else {
			std::string ex = "wrong lexem \nFound: " + lexem + " position: " + std::to_string(a.getLexemPosition());
			throw std::runtime_error(ex);
		}
		--level;
	}

	void Descr(Node& n) {
		++level;
		if (lexem == "int" || lexem == "double") {
			output(std::string("Type"), n);
			n.addNode("Type");
			//std::cout << "Type" << std::endl;
			Type(n.getNode(0));
			if (isID()) {
				output(std::string("VarList"), n);
				n.addNode("VarList");
				//std::cout << "VarList" << std::endl;
				VarList(n.getNode(1));
				if (lexem == ";") {
					output(std::string(";"), n);
					n.addNode(";");
					//std::cout << ";" << std::endl;
				}
				else {
					std::string ex = "wrong lexem \nFound: " + lexem + " position: " + std::to_string(a.getLexemPosition());
					throw std::runtime_error(ex);
				}
			}
			else {
				std::string ex = "wrong lexem \nFound: " + lexem + " position: " + std::to_string(a.getLexemPosition());
				throw std::runtime_error(ex);
			}
		}
		else {
			std::string ex = "wrong lexem \nFound: " + lexem + " position: " + std::to_string(a.getLexemPosition());
			throw std::runtime_error(ex);
		}
		--level;
	}

	void VarList(Node& n) {
		++level;
		if (isID()) {
			output(std::string("Id"), n);
			n.addNode("Id");
			//std::cout << "Id" << std::endl;
			Id(n.getNode(0));
			if (lexem == ",") {
				output(std::string("VarList'"), n);
				n.addNode("VarList'");
				//std::cout << "VarList'" << std::endl;
				VarList1(n.getNode(1));
			}
			else if (lexem == ";") {
				output(std::string("VarList'"), n);
				n.addNode("VarList'");
				VarList1(n.getNode(1));
			}
			else {
				std::string ex = "wrong lexem \nFound: " + lexem + " position: " + std::to_string(a.getLexemPosition());
				throw std::runtime_error(ex);
			}
		}
		else {
			std::string ex = "wrong lexem \nFound: " + lexem + " position: " + std::to_string(a.getLexemPosition());
			throw std::runtime_error(ex);
		}
		--level;
	}

	void VarList1(Node& n) {
		++level;
		if (lexem == ",") {
			output(std::string(","), n);
			n.addNode(",");
			//std::cout << "," << std::endl;
			lexem = a.nextLexem();
			output(std::string("VarList"), n);
			n.addNode("VarList");
			//std::cout << "VarList" << std::endl;
			VarList(n.getNode(1));
		}
		else {
			output(std::string("eps"), n);
			n.addNode("eps");
			//std::cout << "eps" << std::endl;
		}
		--level;
	}

	void Type(Node& n) {
		++level;
		if (lexem == "int") {
			//std::cout << "int" << std::endl;
			output(std::string("int"), n);
			n.addNode("int");
			lexem = a.nextLexem();
		}
		if (lexem == "double") {
			//std::cout << "double" << std::endl;
			output(std::string("double"), n);
			n.addNode("double");
			lexem = a.nextLexem();
		}
		--level;
	}

	void Op(Node& n) {
		++level;
		if (isID()) {
			//std::cout << "Id" << std::endl;
			output(std::string("Id"), n);
			n.addNode("Id");
			Id(n.getNode(0));
			if (lexem == "=") {
				//std::cout << "=" << std::endl;
				output(std::string("="), n);
				n.addNode("=");
				lexem = a.nextLexem();
			}
			else {
				std::string ex = "wrong lexem \nFound: " + lexem + " position: " + std::to_string(a.getLexemPosition());
				throw std::runtime_error(ex);
			}
			if (lexem == "(" || isID() || lexem == "itod" || lexem == "dtoi" || isIntNum() || isDoubleNum()) {
				//std::cout << "Expr" << std::endl;
				output(std::string("Expr"), n);
				n.addNode("Expr");
				Expr(n.getNode(2));
				if (lexem == ";") {
					//std::cout << ";" << std::endl;
					output(std::string(";"), n);
					n.addNode(";");
				}
				else {
					std::string ex = "wrong lexem \nFound: " + lexem + " position: " + std::to_string(a.getLexemPosition());
					throw std::runtime_error(ex);
				}
			}
			else {
				std::string ex = "wrong lexem \nFound: " + lexem + " position: " + std::to_string(a.getLexemPosition());
				throw std::runtime_error(ex);
			}
		}
		else {
			std::string ex = "wrong lexem \nFound: " + lexem + " position: " + std::to_string(a.getLexemPosition());
			throw std::runtime_error(ex);
		}
		--level;
	}

	void SimpleExpr(Node& n) {
		++level;
		if (lexem == "(") {
			//std::cout << "(" << std::endl;
			output(std::string("("), n);
			n.addNode("(");
			lexem = a.nextLexem();
			if (lexem == "(" || isID() || lexem == "itod" || lexem == "dtoi" || isIntNum() || isDoubleNum()) {
				//std::cout << "Expr" << std::endl;
				output(std::string("Expr"), n);
				n.addNode("Expr");
				Expr(n.getNode(1));
				if (lexem == ")") {
					//std::cout << ")" << std::endl;
					output(std::string(")"), n);
					n.addNode(")");
					lexem = a.nextLexem();
				}
				else {
					//std::string ex = "wrong lexem \nFound: " + lexem;
					//throw std::runtime_error(ex);
					std::string ex = "wrong lexem \nFound: " + lexem + " position: " + std::to_string(a.getLexemPosition());
					throw std::runtime_error(ex);
				}
			}
			else {
				std::string ex = "wrong lexem \nFound: " + lexem + " position: " + std::to_string(a.getLexemPosition());
				throw std::runtime_error(ex);
			}
		}

		else if (lexem == "itod") {
			//std::cout << "itod" << std::endl;
			output(std::string("itod"), n);
			n.addNode("itod");
			lexem = a.nextLexem();
			if (lexem == "(") {
				//std::cout << "(" << std::endl;
				output(std::string("("), n);
				n.addNode("(");
				lexem = a.nextLexem();
				if (lexem == "(" || isID() || lexem == "itod" || lexem == "dtoi" || isIntNum() || isDoubleNum()) {
					//std::cout << "Expr" << std::endl;
					output(std::string("Expr"), n);
					n.addNode("Expr");
					Expr(n.getNode(2));
					if (lexem == ")") {
						//std::cout << ")" << std::endl;
						output(std::string(")"), n);
						n.addNode(")");
						lexem = a.nextLexem();
					}
					else {
						std::string ex = "wrong lexem \nFound: " + lexem + " position: " + std::to_string(a.getLexemPosition());
						throw std::runtime_error(ex);
					}
				}
				else {
					std::string ex = "wrong lexem \nFound: " + lexem + " position: " + std::to_string(a.getLexemPosition());
					throw std::runtime_error(ex);
				}
			}
			else {
				std::string ex = "wrong lexem \nFound: " + lexem + " position: " + std::to_string(a.getLexemPosition());
				throw std::runtime_error(ex);
			}
		}

		else if (lexem == "dtoi") {
			//std::cout << "dtoi" << std::endl;
			output(std::string("dtoi"), n);
			n.addNode("dtoi");
			lexem = a.nextLexem();
			if (lexem == "(") {
				//std::cout << "(" << std::endl;
				output(std::string("("), n);
				n.addNode("(");
				lexem = a.nextLexem();
				if (lexem == "(" || isID() || lexem == "itod" || lexem == "dtoi" || isIntNum() || isDoubleNum()) {
					//std::cout << "Expr" << std::endl;
					output(std::string("Expr"), n);
					n.addNode("Expr");
					Expr(n.getNode(2));
					if (lexem == ")") {
						//std::cout << ")" << std::endl;
						output(std::string(")"), n);
						n.addNode(")");
						lexem = a.nextLexem();
					}
					else {
						std::string ex = "wrong lexem \nFound: " + lexem + " position: " + std::to_string(a.getLexemPosition());
						throw std::runtime_error(ex);
					}
				}
				else {
					std::string ex = "wrong lexem \nFound: " + lexem + " position: " + std::to_string(a.getLexemPosition());
					throw std::runtime_error(ex);
				}
			}
			else {
				std::string ex = "wrong lexem \nFound: " + lexem + " position: " + std::to_string(a.getLexemPosition());
				throw std::runtime_error(ex);
			}
		}

		else if (isDoubleNum() || isIntNum()) {
			//std::cout << "Const" << std::endl;
			output(std::string("Const"), n);
			n.addNode("Const");
			Const(n.getNode(0));
		}

		else if (isID()) {
			//std::cout << "Id" << std::endl;
			output(std::string("Id"), n);
			n.addNode("Id");
			Id(n.getNode(0));
		}
		else {
			std::string ex = "wrong lexem \nFound: " + lexem + " position: " + std::to_string(a.getLexemPosition());
			throw std::runtime_error(ex);
		}
		--level;
	}

	void Expr(Node& n) {
		++level;
		if (lexem == "(" || lexem == "itod" || lexem == "dtoi" || isID() || isIntNum() || isDoubleNum()) {
			output(std::string("SimpleExpr"), n);
			n.addNode("SimpleExpr");
			//std::cout << "SimpleExpr" << std::endl;
			SimpleExpr(n.getNode(0));
			if (lexem == "+" || lexem == "-") {
				//std::cout << lexem << std::endl;
				output(lexem, n);
				n.addNode(lexem);
				//lexem = a.nextLexem();
				output(std::string("Expr'"), n);
				n.addNode("Expr'");
				//std::cout << "Expr'" << std::endl;
				Expr1(n.getNode(2));
			}
		}
		else {
			std::string ex = "wrong lexem \nFound: " + lexem + " position: " + std::to_string(a.getLexemPosition());
			throw std::runtime_error(ex);
		}
		--level;
	}

	void Expr1(Node& n) {
		++level;
		if (lexem == "+" || lexem == "-") {
			//std::cout << "Expr" << std::endl;
			lexem = a.nextLexem();
			output(std::string("Expr"), n);
			n.addNode("Expr");
			Expr(n.getNode(0));
		}
		else if (isID()) {
			output(std::string("eps"), n);
			n.addNode("eps");
		}
		else {
			std::string ex = "wrong lexem \nFound: " + lexem + " position: " + std::to_string(a.getLexemPosition());
			throw std::runtime_error(ex);
		}
		--level;
	}

	void Id(Node& n) {
		++level;
		output(lexem, n);
		n.addNode(lexem);
		//std::cout << lexem << std::endl;
		lexem = a.nextLexem();
		--level;
	}

	void Const(Node& n) {
		++level;
		if (isDoubleNum() || isIntNum()) {
			//std::cout << lexem << std::endl;
			output(lexem, n);
			n.addNode(lexem);
			lexem = a.nextLexem();
		}
		--level;
	}

public:
	parcer(std::string lex, authomata& aa) {
		lexem = lex;
		a = aa;
	}

	void parceLexem(Node& n) {
		output(std::string("Function"), n);
		n.changeLexem("Function");
		Function(n);
	}
};