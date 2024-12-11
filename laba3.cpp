#include <iostream>
#include <fstream>
#include <string>
#include <regex>

bool isValidIntegerDeclaration(const std::string& line) {
    // Регулярное выражение для проверки только целочисленных типов
    std::regex intDeclarationRegex(
        R"(\b(signed|unsigned)?\s*(short|long\s+long|long|int|short\s+int|long\s+int)?\s+[_a-zA-Z][_a-zA-Z0-9]*\s*(=\s*[-+]?\d+)?\s*;)");
    return std::regex_match(line, intDeclarationRegex);
}

bool isLineRelevant(const std::string& line) {
    // Регулярное выражение для определения строк с целочисленными типами
    std::regex intTypeRegex(
        R"(\b(signed|unsigned|short|long|int)\b)");
    return std::regex_search(line, intTypeRegex);
}

//int main() {
  //  setlocale(LC_ALL, "Russian");
    //std::string fileName = "code.txt";
//    std::ifstream inputFile(fileName);
//
  //  if (!inputFile.is_open()) {
    //    std::cerr << "Ошибка: не удалось открыть файл " << fileName << std::endl;
      //  return 1;
    //}

    //std::string line;
    //int lineNumber = 0;
    //bool hasErrors = false;

    //while (std::getline(inputFile, line)) {
   //     lineNumber++;
        // Убираем пробелы в начале и конце строки
     //   line.erase(0, line.find_first_not_of(" \t"));
       // line.erase(line.find_last_not_of(" \t") + 1);

        // Проверяем только строки, связанные с целочисленными типами
       // if (isLineRelevant(line)) {
         //   if (!isValidIntegerDeclaration(line)) {
           //     hasErrors = true;
             //   std::cout << "Ошибка в строке " << lineNumber << ": " << line << std::endl;
            //}
        //}
    //}

   // inputFile.close();

   // if (!hasErrors) {
     //   std::cout << "Все объявления целочисленных переменных корректны." << std::endl;
    //}

   // return 0;
//}
