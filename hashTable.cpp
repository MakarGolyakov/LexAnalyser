#pragma once
#include "token.cpp"


class hash_table {
private:
	token* table;
	int size;

	int hfunc(token& tk) {
		int res = 0;
		for (int i = 0; i < tk.lexemSize; ++i) {
			res += tk.lexem[i];
		}
		for (int i = 0; i < tk.typeSize; ++i) {
			res += tk.type[i];
		}
		return res % size;
	}

	void rehash() {
		token* newTable = new token[2 * size];
		for (int i = 0; i < size; ++i) {
			if (table[i].lexemSize != 0) {
				int pos = hfunc(table[i]);
				for (int j = pos; j < 2 * size; ++j) {
					if (newTable[j].lexemSize == 0) {
						newTable[j] = table[i];
					}
				}
			}
		}
		delete[] table;
		table = newTable;
		size = 2 * size;
	}

public:
	hash_table() {
		table = new token[50];
		size = 50;
	}

	~hash_table() {
		delete[] table;
	}

	int getSize() {
		return size;
	}

	void add(token& tk) {
		int index = hfunc(tk);
		for (int i = index; i < size; ++i) {
			if (table[i].lexemSize == 0) {
				table[i] = tk;
				return;
			}
		}
		for (int i = 0; i < index; ++i) {
			if (table[i].lexemSize == 0) {
				table[i] = tk;
				return;
			}
		}
		rehash();
		index = hfunc(tk);
		for (int i = index; i < size; ++i) {
			if (table[i].lexemSize == 0) {
				table[i] = tk;
				return;
			}
		}
		for (int i = 0; i < index; ++i) {
			if (table[i].lexemSize == 0) {
				table[i] = tk;
				return;
			}
		}
	}

	bool find(token& tk) {
		int index = hfunc(tk);
		for (int i = index; i < size; ++i) {
			if (table[i] == tk) {
				return true;
			}
		}
		for (int i = 0; i < index; ++i) {
			if (table[i] == tk) {
				return true;
			}
		}
		return false;
	}

	bool empty(int pos) {
		if (table[pos].lexemSize == 0) {
			return true;
		}
		return false;
	}

	token operator [](const int& pos) {
		return table[pos];
	}
};