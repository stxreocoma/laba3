#include <iostream>
#include <regex>
#include <string>
#include <fstream>
#include <vector>

bool isReservedWord(const std::string& word) {
    static const std::vector<std::string> reservedWords = {
        "alignas", "alignof", "and", "and_eq", "asm", "atomic_cancel", "atomic_commit", "atomic_noexcept",
        "auto", "bitand", "bitor", "bool", "break", "case", "catch", "char", "char8_t", "char16_t", "char32_t",
        "class", "compl", "concept", "const", "consteval", "constexpr", "constinit", "const_cast", "continue",
        "co_await", "co_return", "co_yield", "decltype", "default", "delete", "do", "double", "dynamic_cast",
        "else", "enum", "explicit", "export", "extern", "false", "float", "for", "friend", "goto", "if",
        "inline", "int", "long", "mutable", "namespace", "new", "noexcept", "not", "not_eq", "nullptr",
        "operator", "or", "or_eq", "private", "protected", "public", "register", "reinterpret_cast", "requires",
        "return", "short", "signed", "sizeof", "static", "static_assert", "static_cast", "struct", "switch",
        "synchronized", "template", "this", "thread_local", "throw", "true", "try", "typedef", "typeid",
        "typename", "union", "unsigned", "using", "virtual", "void", "volatile", "wchar_t", "while", "xor",
        "xor_eq"
    };

    for (auto reservedWord : reservedWords) {
        if (word == reservedWord) {
            return true;
        }
    }
    return false;
}

bool isValidIntegerDeclaration(const std::string& line) {
    std::regex intRegex(
        R"(^\s*(int|short|long|long long)\s+([a-zA-Z_][a-zA-Z0-9_]*\s*(?:=\s*-?[0-9]+)?)\s*(,\s*[a-zA-Z_][a-zA-Z0-9_]*(?:\s*=\s*-?[0-9]+)*)*\s*;\s*$)"
    );

    std::smatch match;
    if (std::regex_match(line, match, intRegex)) {
        std::regex variableRegex(R"(([a-zA-Z_][a-zA-Z0-9_]*)(?:\s*=\s*(-?[0-9]+))?)");
        std::string variables = line.substr(match.position(2));

        auto variablesBegin = std::sregex_iterator(variables.begin(), variables.end(), variableRegex);
        auto variablesEnd = std::sregex_iterator();

        for (std::sregex_iterator i = variablesBegin; i != variablesEnd; ++i) {
            std::smatch varMatch = *i;
            std::string varName = varMatch[1].str();
            if (isReservedWord(varName)) {
                return false;
            }
        }

        return true;
    }

    return false;
}

std::string trimString(std::string line) {
    std::string trimmedLine = "";
    bool charFlag = false;
    for (auto c : line) {
        if ((c == ' ' || c == '\b' || c == '\n') && charFlag == false) {
            continue;
        }
        charFlag = true;
        trimmedLine += c;
    }
    return trimmedLine;
}

std::vector<std::string> parseCode(const std::string& filename) {
    std::ifstream in(filename);
    std::vector<std::string> codeLines;
    std::string line;
    while (std::getline(in, line, ';')) {
        line.erase(line.find_last_not_of(" \t\n") + 1);
        codeLines.push_back(trimString(line) + ';');
    }
    return codeLines;
}

int main() {
    std::vector<std::string> code = parseCode("str.txt");
    for (const auto& line : code) {
        if (!isValidIntegerDeclaration(line)) {
            std::cout << "error: " << line << std::endl;
        }
    }
}
