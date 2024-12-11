#define  _CRT_SECURE_NO_WARNINGS

#include <iostream>
#include <vector>
#include <fstream>
#include <string>
#include <regex>

using namespace std;

enum TokenType { TYPE, OPERATOR, FOR, NAME, MODIFICATOR, EQUAL, NUMBER, SEMICOLON, INVALID, EMPTY, OTHER, WHILE, BREAK, SWITCH, CASE, IF, LBRA, LPAR, RPAR, RBRA, COLON, ELSE , OTHERTYPE, SPACE, LIB};

vector<string> TokenName = { "TYPE", "OPERATOR", "FOR", "NAME", "MODIFICATOR", "EQUAL", "NUMBER", "SEMICOLON", "INVALID", "EMPTY", "OTHER", "WHILE", "BREAK", "SWITCH", "CASE", "IF", "LBRA", "LPAR", "RPAR", "RBRA", "COLON", "ELSE" , "OTHERTYPE", "SPACE", "LIB"};

struct Token {
	TokenType type;
	std::string val;
};

string trimString(string line) {
	bool backspace = false;
	string trimmedLine = "";
	for (auto oldChar : line) {
		if (oldChar != ' ') {
			backspace = true;
		}
		if (backspace == true) {
			trimmedLine += oldChar;
		}
	}
	return trimmedLine;
}

vector<string> readFile(string filename) {
	ifstream in(filename);
	vector<string> codeLines;
	string line;
	while (std::getline(in, line)) {
		line.erase(line.find_last_not_of(" \t\n") + 1);
		line = trimString(line);
		codeLines.push_back(line);
	}
	in.close();
	//for (auto code : codeLines) {
		//cout << code << endl;
	//}

	return codeLines;
}

vector<Token> TokenizeLine(string& codeLine) {
	vector<Token> tokenLine;
	if (codeLine.empty()) {
		tokenLine.push_back({ EMPTY, codeLine });
		return tokenLine;
	}

	vector<char> NewTokEl = {
		'{','(',')','}','+','-',' ','\n','\0',';','[',']','=', '>', '<', '=', '\t', ':'
	};
	regex typeRegex(R"(unsigned\s+int|signed\s+int|int|short|long)");
	regex nameRegex(R"([a-zA-Z_][a-zA-Z0-9_]*)");
	regex numberRegex(R"([0-9]+)");
	regex equalRegex(R"(=)");
	regex semicolonRegex(R"(;)");
	regex forRegex(R"(^for)");
	regex whileRegex(R"(^while)");
	regex breakRegex(R"(^break)");
	regex switchRegex(R"(^switch)");
	regex caseRegex(R"(^case)");
	regex deafultRegex(R"(^default)");
	regex ifRegex(R"(^if)");
	regex lbraRegex(R"(^\{)");
	regex rbraRegex(R"(^\})");
	regex lparRegex(R"(^\()");
	regex rparRegex(R"(^\))");
	regex colonRegex(R"(:)");
	regex elseRegex(R"(^else)");
	regex otherTypeRegex(R"(float|double|bool|string|char)");
	regex spaceRegex(R"( )");
	regex otherRegex(R"(#include|#define|using|namespace)");
	regex libRegex(R"(<*>)");


	vector <pair<TokenType, regex>> regs = { {TYPE, typeRegex}, {NAME, nameRegex}, {NUMBER, numberRegex}, {EQUAL, equalRegex}, {SEMICOLON, semicolonRegex}, {FOR, forRegex}, {WHILE, whileRegex}, {BREAK, breakRegex}, {SWITCH, switchRegex}, {CASE, caseRegex}, {IF, ifRegex}, {LPAR, lparRegex}, {RPAR, rparRegex}, {LBRA, lbraRegex}, {RBRA, rbraRegex}, {COLON, colonRegex}, {ELSE, elseRegex}, {OTHERTYPE, otherTypeRegex}, {SPACE, spaceRegex}, {OTHER, otherRegex}, {LIB, libRegex} };
	string substr = "";
	bool find = false;

	for (auto codeChar : codeLine) {
		find = false;
		bool skip = false;
		for (auto endChar: NewTokEl) {
			if (codeChar == endChar) {
				smatch match;
				skip = true;
				bool divFind = false;
				for (auto reg : regs) {
					if (regex_match(substr, match, reg.second)) {
						if (reg.first != SPACE) {
							skip = false;
							tokenLine.push_back({ reg.first, substr });
						}
						if (substr.length() == 1) {
							divFind = true;
						}
						find = true;
						break;
					}
				}
				if (!find) {
					tokenLine.push_back({ INVALID, substr });
				}
				if (!divFind) {
					substr = "";
					substr += codeChar;
					for (auto reg : regs) {
						if (regex_match(substr, match, reg.second)) {
							if (reg.first != SPACE) {
								skip = false;
								tokenLine.push_back({ reg.first, substr });
							}
							divFind = true;
						}
					}
					if (!divFind) {
						tokenLine.push_back({ INVALID, substr });
					}
				}
				substr = "";
				break;
			}
		}
		if (skip) {
			continue;
		}
		substr += codeChar;

	}
	smatch match;
	for (auto reg : regs) {
		if (regex_match(substr, match, reg.second)) {
			tokenLine.push_back({ reg.first, substr });
			break;
		}
	}
}

vector<Token> tokenizeLine(string& codeLine) {
	vector<Token> tokenLine;

	if (codeLine.empty()) {
		tokenLine.push_back({ EMPTY, codeLine });
		return tokenLine;
	}
	else if (codeLine[0] == '#' || codeLine[0] == 'u') {
		tokenLine.push_back({ EMPTY, codeLine });
		return tokenLine;
	}

	//regexes
	regex typeRegex(R"(\b(unsigned\s+int|signed\s+int|int|short|long)\b)");
	regex nameRegex(R"(\b[a-zA-Z_][a-zA-Z0-9_]*\b)");
	regex numberRegex(R"(\b[0-9]+\b)");
	regex equalRegex(R"(=)");
	regex semicolonRegex(R"(;)");

	string::const_iterator start(codeLine.cbegin());
	while (start != codeLine.cend()) {
		smatch match;

		if (regex_search(start, codeLine.cend(), match, typeRegex)) {
			tokenLine.push_back({ TYPE, match.str() });
		}
		else if (regex_search(start, codeLine.cend(), match, nameRegex)) {
			tokenLine.push_back({ NAME, match.str() });
		}
		else if (regex_search(start, codeLine.cend(), match, equalRegex)) {
			tokenLine.push_back({ EQUAL, match.str() });
		}
		else if (regex_search(start, codeLine.cend(), match, numberRegex)) {
			tokenLine.push_back({ NUMBER, match.str() });
		}
		else if (regex_search(start, codeLine.cend(), match, semicolonRegex)) {
			tokenLine.push_back({ SEMICOLON, match.str() });
		}
		else {
			tokenLine.push_back({ INVALID, string(start, start + 1) });
		}
		auto prev_start = start;
		start += match.position() + match.length();
		if (prev_start == start) {
			break;
		}
	}

	std::cout << "Токены:" << endl;
	for (auto& token : tokenLine) {
		std::cout << "  Value: \"" << token.val << "\", Type: " << token.type << endl;
	}

	return tokenLine;
}

vector<vector<Token>> tokenizeCode(vector<string> codeLines) {
	vector<vector<Token>> tokenLines;
	for (auto codeLine : codeLines) {
		auto tokenLine = TokenizeLine(codeLine);
		for (auto token : tokenLine) {
			cout << TokenName[token.type] << " ";
		}
		cout << endl;
		tokenLines.push_back(tokenLine);
	}

	return tokenLines;
}

bool checkTokenLine(vector<Token> tokenLine) {
	if (tokenLine.empty()) {
		return true;
	}
	if (tokenLine[0].type != TYPE && tokenLine[0].type != NAME) {
		return true;
	}
	else if (tokenLine[0].type == NAME) {
		return false;
	}

	if (tokenLine.size() == 5) {
		if (tokenLine[0].type == TYPE && tokenLine[1].type == NAME && tokenLine[2].type == EQUAL && tokenLine[3].type == NUMBER && tokenLine[4].type == SEMICOLON) {
			return true;
		}
	}
	else if (tokenLine.size() == 3) {
		if (tokenLine[0].type == TYPE && tokenLine[1].type == NAME && tokenLine[2].type == SEMICOLON) {
			return true;
		}
	}

	return false;
}

bool parseLine(vector<Token>);

void checkTokenLines(vector<vector<Token>> tokenLines) {
	for (int i = 0; i < tokenLines.size(); i++) {
		if (parseLine(tokenLines[i])) {
			cout << "Vse good " << tokenLines[i][0].val << "   " << tokenLines[i].size() << endl;
			continue;
		}

		cout << "Error in line " << i + 1 << "  " << tokenLines[i][0].val << "   " << tokenLines[i].size() << endl;
	}
}

bool parseLine(vector<Token> tokenLine) {
	int i = 0;

	if (i < tokenLine.size() && tokenLine[i].type == TYPE) {
		i++;
	}
	else {
		return false;
	}

	if (i < tokenLine.size() && tokenLine[i].type == NAME) {
		i++;
	}
	else {
		return false;
	}

	if (i < tokenLine.size() && tokenLine[i].type == EQUAL) {
		i++;
		if (i < tokenLine.size() && (tokenLine[i].type == NUMBER || tokenLine[i].type == NAME)) {
			i++;
		}
		else {
			return false;
		}
	}

	if (i < tokenLine.size() && tokenLine[i].type == SEMICOLON) {
		i++;
	}
	else {
		return false;
	}

	if (i == tokenLine.size()) {
		return true;
	}
	else {
		return false;
	}
}

int main() {
	setlocale(LC_ALL, "Russian");
	checkTokenLines(tokenizeCode(readFile("code.txt")));
}