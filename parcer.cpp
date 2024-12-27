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
	std::string finctionType;
	authomata a;
	int level = 0;
	std::vector<std::pair<std::string, std::string>> vars;
	bool varExsits(std::string& vr) {
		for (int i = 0; i < vars.size(); ++i) {
			if (vr == vars[i].first) {
				return true;
			}
		}
		return false;
	}
	std::pair<std::string, std::string> varInfo(std::string& vr) {
		for (int i = 0; i < vars.size(); ++i) {
			if (vars[i].first == vr) {
				return vars[i];
			}
		}
		return std::make_pair("", "");
	}
	void output(std::string lex, Node& n) {
		for (int i = 0; i < level; ++i) {
			fout << '\t';
		}
		fout << lex << std::endl;
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
			n.tr += n.getNode(0).tr;
		}
		else {
			std::string ex = "wrong lexem \nFound: " + lexem + " position: " + std::to_string(a.getLexemPosition());
			throw std::runtime_error(ex);
		}

		if (lexem == "int" || lexem == "double") {
			output(std::string("Descriptions"), n);
			n.addNode("Descroptions");
			Descriptions(n.getNode(1));
			n.tr += ' ' + n.getNode(1).tr;
		}
		else {
			std::string ex = "wrong lexem \nFound: " + lexem + " position: " + std::to_string(a.getLexemPosition());
			throw std::runtime_error(ex);
		}

		if (isID()) {
			output(std::string("Operators"), n);
			n.addNode("Operators");
			Operators(n.getNode(2));
			n.tr += ' ' + n.getNode(2).tr;
		}
		else {
			std::string ex = "wrong lexem \nFound: " + lexem + " position: " + std::to_string(a.getLexemPosition());
			throw std::runtime_error(ex);
		}
		if (lexem == "return") {
			output(std::string("End"), n);
			n.addNode("End");
			End(n.getNode(3));
			n.tr += ' ' + n.getNode(3).tr;
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
			output(std::string("Type"), n);
			n.addNode("Type");
			Type(n.getNode(0));
			n.tr = n.getNode(0).tr;
			n.count = n.getNode(0).count;
			n.type = n.getNode(0).type;
			finctionType = n.type;
			if (isID()) {
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
			n.tr += ' ' + n.getNode(1).tr + " DECL";
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
			output(std::string("return"), n);
			n.addNode("return");
			lexem = a.nextLexem();
			if (isID()) {
				if (!varExsits(lexem)) {
					std::string ex = "undefined " + lexem;
					throw std::runtime_error(ex);
				}
				output(std::string("Id"), n);
				n.addNode("Id");
				Id(n.getNode(1));
				n.tr = n.getNode(1).tr + " return";
				n.type = n.getNode(1).type;
				if (n.type != finctionType) {
					std::string ex = "wrong type returned";
					throw std::runtime_error(ex);
				}
				if (lexem == ";") {
					output(std::string(";"), n);
					n.addNode(";");
					lexem = a.nextLexem();
					if (lexem == "}") {
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
			n.tr = n.getNode(0).tr;
		}
		--level;
	}

	void Descriptions(Node& n) {
		++level;
		if (lexem == "int" || lexem == "double") {
			output(std::string("Descr"), n);
			n.addNode("Descr");
			Descr(n.getNode(0));
			n.tr = n.getNode(0).tr;
			if (lexem == ";") {
				lexem = a.nextLexem();
				output(std::string("Descriptions'"), n);
				n.addNode("Descroptions'");
				Descriptions1(n.getNode(1));
				n.tr += ' ' + n.getNode(1).tr;
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
			output(std::string("Descriptions"), n);
			n.addNode("Descroptions");
			Descriptions(n.getNode(0));
			n.tr = n.getNode(0).tr;
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

	void Operators(Node& n) {
		++level;
		if (isID()) {
			output(std::string("Op"), n);
			n.addNode("Op");
			Op(n.getNode(0));
			n.tr = n.getNode(0).tr;
			if (lexem == ";") {
				output(std::string(";"), n);
				lexem = a.nextLexem();
				output(std::string("Operators'"), n);
				n.addNode("Operators'");
				Operators1(n.getNode(1));
				n.tr += ' ' + n.getNode(1).tr;
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
		}
		else if (isID()) {
			output(std::string("Operators"), n);
			n.addNode("Operators");
			Operators(n.getNode(0));
			n.tr = n.getNode(0).tr;
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
			Type(n.getNode(0));
			n.tr += n.getNode(0).tr;
			n.count += n.getNode(0).count;
			n.type += n.getNode(0).type;
			if (isID()) {
				output(std::string("VarList"), n);
				n.addNode("VarList");
				VarList(n.getNode(1));
				n.count += n.getNode(1).count;
				std::string var = n.getNode(1).tr;
				std::string cur;
				for (int i = 0; i < var.size(); ++i) {
					if (var[i] == ' ' && cur.size() > 0) {
						if (!varExsits(cur)) {
							vars.push_back(std::make_pair(cur, n.type));
							cur = "";
						}
						else {
							std::string ex = "var exsists: " + cur;
							throw std::runtime_error(ex);
						}
					}
					else {
						cur += var[i];
					}
				}
				n.tr += ' ' + n.getNode(1).tr + std::to_string(n.count) + " DECL";
				if (lexem == ";") {
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

	void VarList(Node& n) {
		++level;
		if (isID()) {
			output(std::string("Id"), n);
			n.addNode("Id");
			Id(n.getNode(0));
			n.tr += n.getNode(0).tr;
			n.count += n.getNode(0).count;
			if (lexem == ",") {
				output(std::string("VarList'"), n);
				n.addNode("VarList'");
				VarList1(n.getNode(1));
				n.tr += ' ' + n.getNode(1).tr;
				n.count += n.getNode(1).count;
			}
			else if (lexem == ";") {
				output(std::string("VarList'"), n);
				n.addNode("VarList'");
				VarList1(n.getNode(1));
				n.tr += ' ' + n.getNode(1).tr;
				n.count += n.getNode(1).count;
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
			lexem = a.nextLexem();
			output(std::string("VarList"), n);
			n.addNode("VarList");
			VarList(n.getNode(1));
			n.tr += n.getNode(1).tr;
			n.count += n.getNode(1).count;
		}
		else {
			output(std::string("eps"), n);
			n.addNode("eps");
		}
		--level;
	}

	void Type(Node& n) {
		++level;
		if (lexem == "int") {
			output(std::string("int"), n);
			n.tr = lexem;
			n.type = lexem;
			n.count = 1;
			n.addNode("int");
			n.getNode(0).type = lexem;
			n.getNode(0).tr = lexem;
			n.getNode(0).count = 1;
			lexem = a.nextLexem();
		}
		if (lexem == "double") {
			output(std::string("double"), n);
			n.tr = lexem;
			n.type = lexem;
			n.count = 1;
			n.addNode("double");
			n.getNode(0).type = lexem;
			n.getNode(0).tr = lexem;
			n.getNode(0).count = 1;
			lexem = a.nextLexem();
		}
		--level;
	}

	void Op(Node& n) {
		++level;
		if (isID()) {
			output(std::string("Id"), n);
			n.addNode("Id");
			if (!varExsits(lexem)) {
				std::string ex = "undefined " + lexem;
				throw std::runtime_error(ex);
			}
			Id(n.getNode(0));
			n.tr += n.getNode(0).tr;
			n.type = n.getNode(0).type;
			if (lexem == "=") {
				output(std::string("="), n);
				n.addNode("=");
				lexem = a.nextLexem();
			}
			else {
				std::string ex = "wrong lexem \nFound: " + lexem + " position: " + std::to_string(a.getLexemPosition());
				throw std::runtime_error(ex);
			}
			if (lexem == "(" || isID() || lexem == "itod" || lexem == "dtoi" || isIntNum() || isDoubleNum()) {
				output(std::string("Expr"), n);
				n.addNode("Expr");
				Expr(n.getNode(2));
				if (n.type != n.getNode(2).type) {
					std::string ex = "different types";
					throw std::runtime_error(ex);
				}
				n.tr += ' ' + n.getNode(2).tr + " =";
				if (lexem == ";") {
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
			output(std::string("("), n);
			n.addNode("(");
			lexem = a.nextLexem();
			if (lexem == "(" || isID() || lexem == "itod" || lexem == "dtoi" || isIntNum() || isDoubleNum()) {
				output(std::string("Expr"), n);
				n.addNode("Expr");
				Expr(n.getNode(1));
				n.tr += n.getNode(1).tr;
				n.type = n.getNode(1).type;
				if (lexem == ")") {
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

		else if (lexem == "itod") {
			output(std::string("itod"), n);
			n.addNode("itod");
			lexem = a.nextLexem();
			if (lexem == "(") {
				output(std::string("("), n);
				n.addNode("(");
				lexem = a.nextLexem();
				if (lexem == "(" || isID() || lexem == "itod" || lexem == "dtoi" || isIntNum() || isDoubleNum()) {
					output(std::string("Expr"), n);
					n.addNode("Expr");
					Expr(n.getNode(2));
					if (n.getNode(2).type != "int") {
						std::string ex = "wrong type for itod";
						throw std::runtime_error(ex);
					}
					n.tr += n.getNode(2).tr + ' ' + "itod";
					n.type = "double";
					if (lexem == ")") {
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
			output(std::string("dtoi"), n);
			n.addNode("dtoi");
			lexem = a.nextLexem();
			if (lexem == "(") {
				output(std::string("("), n);
				n.addNode("(");
				lexem = a.nextLexem();
				if (lexem == "(" || isID() || lexem == "itod" || lexem == "dtoi" || isIntNum() || isDoubleNum()) {
					output(std::string("Expr"), n);
					n.addNode("Expr");
					Expr(n.getNode(2));
					if (n.getNode(2).type != "double") {
						std::string ex = "wrong type for dtoi";
						throw std::runtime_error(ex);
					}
					n.tr += n.getNode(2).tr + ' ' + "dtoi";
					n.type = "int";
					if (lexem == ")") {
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
			output(std::string("Const"), n);
			n.addNode("Const");
			Const(n.getNode(0));
			n.type = n.getNode(0).type;
			n.tr = n.getNode(0).tr;
		}

		else if (isID()) {

			output(std::string("Id"), n);
			n.addNode("Id");
			Id(n.getNode(0));
			n.tr = n.getNode(0).tr;
			n.type = n.getNode(0).type;
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
			SimpleExpr(n.getNode(0));
			n.tr += n.getNode(0).tr;
			n.type = n.getNode(0).type;
			if (lexem == "+" || lexem == "-") {
				output(lexem, n);
				std::string operation = lexem;
				n.addNode(lexem);
				output(std::string("Expr'"), n);
				n.addNode("Expr'");
				Expr1(n.getNode(2));
				n.tr += ' ' + n.getNode(2).tr + ' ' + operation;
				if (n.type != n.getNode(2).type) {
					std::string ex = "different types";
					throw std::runtime_error(ex);
				}

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
			lexem = a.nextLexem();
			output(std::string("Expr"), n);
			n.addNode("Expr");
			Expr(n.getNode(0));
			n.tr = n.getNode(0).tr;
			n.type = n.getNode(0).type;
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
		if (varExsits(lexem)) {
			n.type = varInfo(lexem).second;
		}
		++level;
		output(lexem, n);
		n.addNode(lexem);
		n.tr = lexem;
		n.count = 1;
		n.getNode(0).tr = lexem;
		n.getNode(0).count = 1;
		lexem = a.nextLexem();
		--level;
	}

	void Const(Node& n) {
		++level;
		if (isDoubleNum() || isIntNum()) {
			output(lexem, n);
			n.addNode(lexem);
			if (isDoubleNum()) {
				n.tr = lexem;
				n.count = 1;
				n.type = "double";
				n.getNode(0).tr = lexem;
				n.getNode(0).type = "double";
				n.getNode(0).count = 1;
			}
			if (isIntNum()) {
				n.tr = lexem;
				n.count = 1;
				n.type = "int";
				n.getNode(0).tr = lexem;
				n.getNode(0).type = "int";
				n.getNode(0).count = 1;
			}
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