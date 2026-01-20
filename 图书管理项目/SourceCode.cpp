#include <iostream>
#include "LibraryManagement.h"

int main() {
    std::cout << "================================" << std::endl;
    std::cout << "      图书馆管理系统 v2.0" << std::endl;
    std::cout << "      数据存储：book.txt" << std::endl;
    std::cout << "================================" << std::endl;

    LibraryUI librarySystem;
    librarySystem.run();

    return 0;
}