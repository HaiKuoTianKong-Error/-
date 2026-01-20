#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <iomanip>
#include <limits>
#include <fstream>
#include <sstream>
#include "LibraryManagement.h"

// Book类实现
Book::Book(std::string isbn, std::string title, std::string author,
    std::string publisher, std::string publishDate, double price, int quantity)
    : isbn(isbn), title(title), author(author), publisher(publisher),
    publishDate(publishDate), price(price), quantity(quantity) {
}

std::string Book::getISBN() const { return isbn; }
std::string Book::getTitle() const { return title; }
std::string Book::getAuthor() const { return author; }
std::string Book::getPublisher() const { return publisher; }
std::string Book::getPublishDate() const { return publishDate; }
double Book::getPrice() const { return price; }
int Book::getQuantity() const { return quantity; }

void Book::setTitle(std::string title) { this->title = title; }
void Book::setAuthor(std::string author) { this->author = author; }
void Book::setPublisher(std::string publisher) { this->publisher = publisher; }
void Book::setPublishDate(std::string publishDate) { this->publishDate = publishDate; }
void Book::setPrice(double price) { this->price = price; }
void Book::setQuantity(int quantity) { this->quantity = quantity; }

void Book::displayInfo() const {
    std::cout << "================================" << std::endl;
    std::cout << "ISBN: " << isbn << std::endl;
    std::cout << "书名: " << title << std::endl;
    std::cout << "作者: " << author << std::endl;
    std::cout << "出版社: " << publisher << std::endl;
    std::cout << "出版日期: " << publishDate << std::endl;
    std::cout << "价格: " << std::fixed << std::setprecision(2) << price << "元" << std::endl;
    std::cout << "数量: " << quantity << "本" << std::endl;
    std::cout << "================================" << std::endl;
}

std::string Book::toString() const {
    std::ostringstream oss;
    oss << isbn << "|"
        << title << "|"
        << author << "|"
        << publisher << "|"
        << publishDate << "|"
        << std::fixed << std::setprecision(2) << price << "|"
        << quantity;
    return oss.str();
}

Book Book::fromString(const std::string& str) {
    std::istringstream iss(str);
    std::string isbn, title, author, publisher, publishDate, priceStr, quantityStr;

    std::getline(iss, isbn, '|');
    std::getline(iss, title, '|');
    std::getline(iss, author, '|');
    std::getline(iss, publisher, '|');
    std::getline(iss, publishDate, '|');
    std::getline(iss, priceStr, '|');
    std::getline(iss, quantityStr, '|');

    double price = std::stod(priceStr);
    int quantity = std::stoi(quantityStr);

    return Book(isbn, title, author, publisher, publishDate, price, quantity);
}

// Library类实现
Library::Library() {
    loadBooks();  // 构造时自动加载数据
}

void Library::loadBooks() {
    std::ifstream file(dataFile);
    if (!file.is_open()) {
        std::cout << "首次运行，正在创建新的图书数据库..." << std::endl;
        return;
    }

    std::string line;
    while (std::getline(file, line)) {
        if (line.empty()) continue;
        try {
            Book book = Book::fromString(line);
            books.push_back(book);
        }
        catch (...) {
            std::cerr << "警告：无法解析行数据，跳过此行。" << std::endl;
        }
    }

    file.close();
    std::cout << "成功加载 " << books.size() << " 本图书数据。" << std::endl;
}

void Library::saveBooks() const {
    std::ofstream file(dataFile);
    if (!file.is_open()) {
        std::cerr << "错误：无法打开数据文件进行保存。" << std::endl;
        return;
    }

    for (const auto& book : books) {
        file << book.toString() << std::endl;
    }

    file.close();
    std::cout << "图书数据已保存到 " << dataFile << "。" << std::endl;
}

int Library::findBookIndex(const std::string& isbn) const {
    for (size_t i = 0; i < books.size(); ++i) {
        if (books[i].getISBN() == isbn) {
            return static_cast<int>(i);
        }
    }
    return -1;
}

bool Library::getBook(const std::string& isbn, Book& outBook) const {
    int index = findBookIndex(isbn);
    if (index == -1) return false;
    outBook = books[static_cast<size_t>(index)];
    return true;
}

bool Library::addBook(const Book& book) {
    if (isISBNExists(book.getISBN())) {
        std::cout << "错误：ISBN " << book.getISBN() << " 已存在。" << std::endl;
        return false;
    }

    books.push_back(book);
    saveBooks();  // 添加后自动保存
    std::cout << "成功添加图书：" << book.getTitle() << std::endl;
    return true;
}

bool Library::updateBook(const std::string& isbn, const Book& newBookInfo) {
    int index = findBookIndex(isbn);
    if (index == -1) {
        std::cout << "错误：未找到ISBN " << isbn << " 的图书。" << std::endl;
        return false;
    }

    // 检查新的ISBN是否与其他图书冲突
    if (newBookInfo.getISBN() != isbn && isISBNExists(newBookInfo.getISBN())) {
        std::cout << "错误：新的ISBN " << newBookInfo.getISBN() << " 已存在。" << std::endl;
        return false;
    }

    books[index] = newBookInfo;
    saveBooks();  // 更新后自动保存
    std::cout << "成功更新图书信息。" << std::endl;
    return true;
}

bool Library::deleteBook(const std::string& isbn) {
    int index = findBookIndex(isbn);
    if (index == -1) {
        std::cout << "错误：未找到ISBN " << isbn << " 的图书。" << std::endl;
        return false;
    }

    std::string title = books[index].getTitle();
    books.erase(books.begin() + index);
    saveBooks();  // 删除后自动保存
    std::cout << "成功删除图书：" << title << std::endl;
    return true;
}

bool Library::searchBookByISBN(const std::string& isbn) const {
    int index = findBookIndex(isbn);
    if (index == -1) {
        std::cout << "未找到ISBN " << isbn << " 的图书。" << std::endl;
        return false;
    }

    books[index].displayInfo();
    return true;
}

void Library::searchBookByTitle(const std::string& title) const {
    bool found = false;
    std::cout << "\n=== 按书名搜索结果 ===" << std::endl;
    for (const auto& book : books) {
        if (book.getTitle().find(title) != std::string::npos) {
            book.displayInfo();
            found = true;
        }
    }
    if (!found) {
        std::cout << "未找到包含 \"" << title << "\" 的图书。" << std::endl;
    }
}

void Library::searchBookByAuthor(const std::string& author) const {
    bool found = false;
    std::cout << "\n=== 按作者搜索结果 ===" << std::endl;
    for (const auto& book : books) {
        if (book.getAuthor().find(author) != std::string::npos) {
            book.displayInfo();
            found = true;
        }
    }
    if (!found) {
        std::cout << "未找到作者包含 \"" << author << "\" 的图书。" << std::endl;
    }
}

void Library::displayAllBooks() const {
    if (books.empty()) {
        std::cout << "图书库中没有图书。" << std::endl;
        return;
    }

    std::cout << "\n=== 图书馆图书列表 ===" << std::endl;
    std::cout << "总共有 " << books.size() << " 本图书" << std::endl;
    std::cout << "------------------------" << std::endl;
    for (const auto& book : books) {
        std::cout << "ISBN: " << book.getISBN() << std::endl;
        std::cout << "书名: " << book.getTitle() << std::endl;
        std::cout << "作者: " << book.getAuthor() << std::endl;
        std::cout << "价格: " << std::fixed << std::setprecision(2) << book.getPrice() << "元" << std::endl;
        std::cout << "数量: " << book.getQuantity() << "本" << std::endl;
        std::cout << "------------------------" << std::endl;
    }
}

void Library::clearAllBooks() {
    if (books.empty()) {
        std::cout << "图书库已经是空的。" << std::endl;
        return;
    }

    std::cout << "警告：确定要删除所有图书记录吗？(y/n): ";
    char confirm;
    std::cin >> confirm;
    if (confirm == 'y' || confirm == 'Y') {
        books.clear();
        saveBooks();  // 清空后自动保存
        std::cout << "成功删除所有图书记录。" << std::endl;
    }
    else {
        std::cout << "操作已取消。" << std::endl;
    }
}

void Library::generateStatistics() const {
    if (books.empty()) {
        std::cout << "图书库中没有图书，无法生成统计信息。" << std::endl;
        return;
    }

    std::cout << "\n=== 图书馆统计信息 ===" << std::endl;

    int bookTypes = static_cast<int>(books.size());
    int totalCopies = 0;
    double totalValue = 0.0;
    double maxPrice = -1.0;
    std::string maxPriceTitle;

    for (const auto& book : books) {
        int qty = book.getQuantity();
        double price = book.getPrice();

        totalCopies += qty;
        totalValue += price * qty;

        if (price > maxPrice) {
            maxPrice = price;
            maxPriceTitle = book.getTitle();
        }
    }

    double averagePrice = totalCopies > 0 ? totalValue / totalCopies : 0.0;

    std::cout << "图书类目: " << bookTypes << " 种" << std::endl;
    std::cout << "图书总价值: " << std::fixed << std::setprecision(2) << totalValue << " 元" << std::endl;
    std::cout << "平均价格: " << std::fixed << std::setprecision(2) << averagePrice << " 元" << std::endl;
    if (!maxPriceTitle.empty()) {
        std::cout << "价格最高的图书: " << maxPriceTitle << " (" << std::fixed << std::setprecision(2) << maxPrice << "元)" << std::endl;
    } else {
        std::cout << "价格最高的图书: 无" << std::endl;
    }
    std::cout << "总数量: " << totalCopies << " 本" << std::endl;
}

bool Library::isISBNExists(const std::string& isbn) const {
    return findBookIndex(isbn) != -1;
}

int Library::getTotalBooksCount() const {
    return static_cast<int>(books.size());
}

double Library::getTotalValue() const {
    double total = 0.0;
    for (const auto& book : books) {
        total += book.getPrice() * book.getQuantity();
    }
    return total;
}

// LibraryUI类实现
void LibraryUI::displayMainMenu() const {
    std::cout << "\n================================" << std::endl;
    std::cout << "        图书馆管理系统" << std::endl;
    std::cout << "================================" << std::endl;
    std::cout << "1. 添加图书" << std::endl;
    std::cout << "2. 修改图书" << std::endl;
    std::cout << "3. 删除图书" << std::endl;
    std::cout << "4. 搜索图书" << std::endl;
    std::cout << "5. 显示所有图书" << std::endl;
    std::cout << "6. 生成统计信息" << std::endl;
    std::cout << "7. 清空所有记录" << std::endl;
    std::cout << "0. 退出系统" << std::endl;
    std::cout << "================================" << std::endl;
    std::cout << "请选择操作 (0-7): ";
}

void LibraryUI::displaySearchMenu() const {
    std::cout << "\n=== 搜索图书 ===" << std::endl;
    std::cout << "1. 按ISBN搜索" << std::endl;
    std::cout << "2. 按书名搜索" << std::endl;
    std::cout << "3. 按作者搜索" << std::endl;
    std::cout << "0. 返回主菜单" << std::endl;
    std::cout << "请选择搜索方式 (0-3): ";
}

bool LibraryUI::isValidISBN(const std::string& isbn) const {
    // 简单的ISBN验证，实际应用应使用更详细的验证
    if (isbn.empty()) return false;
    if (isbn.length() < 10 || isbn.length() > 13) return false;

    // 检查是否只包含数字和连字符
    for (char c : isbn) {
        if (!isdigit(c) && c != '-') {
            return false;
        }
    }
    return true;
}

bool LibraryUI::isValidPrice(double price) const {
    return price >= 0.0;
}

bool LibraryUI::isValidQuantity(int quantity) const {
    return quantity >= 0;
}

std::string LibraryUI::getStringInput(const std::string& prompt) const {
    std::string input;
    std::cout << prompt;
    std::getline(std::cin >> std::ws, input);
    return input;
}

double LibraryUI::getDoubleInput(const std::string& prompt) const {
    double input;
    while (true) {
        std::cout << prompt;
        if (std::cin >> input) {
            // consume trailing newline for subsequent getline
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            return input;
        }
        else {
            std::cout << "请输入有效的数字！" << std::endl;
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        }
    }
}

int LibraryUI::getIntInput(const std::string& prompt) const {
    int input;
    while (true) {
        std::cout << prompt;
        if (std::cin >> input) {
            // consume trailing newline for subsequent getline
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            return input;
        }
        else {
            std::cout << "请输入有效的整数！" << std::endl;
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        }
    }
}

bool LibraryUI::getOptionalDoubleInput(const std::string& prompt, double& out) const {
    std::string line;
    std::cout << prompt << "（留空以保留原值）: ";
    std::getline(std::cin >> std::ws, line);
    if (line.empty()) return false;
    try {
        out = std::stod(line);
        return true;
    }
    catch (...) {
        std::cout << "输入无效，请输入有效的数字或直接回车保留原值。" << std::endl;
        return getOptionalDoubleInput(prompt, out);
    }
}

bool LibraryUI::getOptionalIntInput(const std::string& prompt, int& out) const {
    std::string line;
    std::cout << prompt << "（留空以保留原值）: ";
    std::getline(std::cin >> std::ws, line);
    if (line.empty()) return false;
    try {
        out = std::stoi(line);
        return true;
    }
    catch (...) {
        std::cout << "输入无效，请输入有效的整数或直接回车保留原值。" << std::endl;
        return getOptionalIntInput(prompt, out);
    }
}

void LibraryUI::handleAddBook() {
    std::cout << "\n=== 添加新图书 ===" << std::endl;
    std::string isbn = getStringInput("请输入ISBN: ");

    while (!isValidISBN(isbn)) {
        std::cout << "ISBN格式无效，请重新输入：" << std::endl;
        isbn = getStringInput("请输入ISBN: ");
    }

    if (library.isISBNExists(isbn)) {
        std::cout << "错误：ISBN " << isbn << " 已存在。" << std::endl;
        return;
    }

    std::string title = getStringInput("请输入书名: ");
    std::string author = getStringInput("请输入作者: ");
    std::string publisher = getStringInput("请输入出版社: ");
    std::string publishDate = getStringInput("请输入出版日期 (YYYY-MM-DD): ");

    double price = getDoubleInput("请输入价格: ");
    while (!isValidPrice(price)) {
        std::cout << "价格必须为正数！" << std::endl;
        price = getDoubleInput("请输入价格: ");
    }

    int quantity = getIntInput("请输入数量: ");
    while (!isValidQuantity(quantity)) {
        std::cout << "数量必须为非负数！" << std::endl;
        quantity = getIntInput("请输入数量: ");
    }

    Book newBook(isbn, title, author, publisher, publishDate, price, quantity);
    library.addBook(newBook);
}

void LibraryUI::handleUpdateBook() {
    std::cout << "\n=== 修改图书信息 ===" << std::endl;
    std::string isbn = getStringInput("请输入要修改的图书ISBN: ");

    if (!library.isISBNExists(isbn)) {
        std::cout << "错误：未找到ISBN " << isbn << " 的图书！" << std::endl;
        return;
    }

    // 读取原图书信息
    Book oldBook("", "", "", "", "", 0.0, 0);
    if (!library.getBook(isbn, oldBook)) {
        std::cout << "错误：无法读取原图书信息。" << std::endl;
        return;
    }
    std::cout << "原始信息：" << std::endl;
    oldBook.displayInfo();
    std::cout << "\n请输入新的图书信息，按回车键保留原有信息。" << std::endl;

    std::string newISBN = getStringInput("新ISBN: ");
    std::string newTitle = getStringInput("新书名: ");
    std::string newAuthor = getStringInput("新作者: ");
    std::string newPublisher = getStringInput("新出版社: ");
    std::string newPublishDate = getStringInput("新出版日期: ");

    double newPrice = oldBook.getPrice();
    int newQuantity = oldBook.getQuantity();
    // 可选输入（空表示保留原值）
    getOptionalDoubleInput("新价格", newPrice);
    getOptionalIntInput("新数量", newQuantity);

    // 如果新字段为空，保留原值
    if (newISBN.empty()) newISBN = oldBook.getISBN();
    if (newTitle.empty()) newTitle = oldBook.getTitle();
    if (newAuthor.empty()) newAuthor = oldBook.getAuthor();
    if (newPublisher.empty()) newPublisher = oldBook.getPublisher();
    if (newPublishDate.empty()) newPublishDate = oldBook.getPublishDate();

    Book updatedBook(newISBN, newTitle, newAuthor, newPublisher, newPublishDate, newPrice, newQuantity);
    library.updateBook(isbn, updatedBook);
}

void LibraryUI::handleDeleteBook() {
    std::cout << "\n=== 删除图书 ===" << std::endl;
    std::string isbn = getStringInput("请输入要删除的图书ISBN: ");

    if (!library.isISBNExists(isbn)) {
        std::cout << "错误：未找到ISBN " << isbn << " 的图书！" << std::endl;
        return;
    }

    std::cout << "确定要删除ISBN " << isbn << " 的图书吗？(y/n): ";
    char confirm;
    std::cin >> confirm;
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

    if (confirm == 'y' || confirm == 'Y') {
        library.deleteBook(isbn);
    }
    else {
        std::cout << "删除操作已取消。" << std::endl;
    }
}

void LibraryUI::handleSearchBook() {
    int choice;
    do {
        displaySearchMenu();
        std::cin >> choice;
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

        switch (choice) {
        case 1: {
            std::string isbn = getStringInput("请输入ISBN: ");
            library.searchBookByISBN(isbn);
            break;
        }
        case 2: {
            std::string title = getStringInput("请输入书名关键词: ");
            library.searchBookByTitle(title);
            break;
        }
        case 3: {
            std::string author = getStringInput("请输入作者关键词: ");
            library.searchBookByAuthor(author);
            break;
        }
        case 0:
            std::cout << "返回主菜单..." << std::endl;
            break;
        default:
            std::cout << "无效的选择，请重新输入！" << std::endl;
        }

        if (choice != 0) {
            std::cout << "\n按回车键继续...";
            std::string dummy;
            std::getline(std::cin >> std::ws, dummy);
        }
    } while (choice != 0);
}

void LibraryUI::handleDisplayAllBooks() {
    library.displayAllBooks();
    std::cout << "\n按回车键继续...";
    std::string dummy;
    std::getline(std::cin >> std::ws, dummy);
}

void LibraryUI::handleClearAllBooks() {
    library.clearAllBooks();
    std::cout << "\n按回车键继续...";
    std::string dummy;
    std::getline(std::cin >> std::ws, dummy);
}

void LibraryUI::handleGenerateStatistics() {
    library.generateStatistics();
    std::cout << "\n按回车键继续...";
    std::string dummy;
    std::getline(std::cin >> std::ws, dummy);
}

void LibraryUI::run() {
    int choice;
    do {
        displayMainMenu();
        if (!(std::cin >> choice)) {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cout << "无效的选择，请重新输入！" << std::endl;
            continue;
        }
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

        switch (choice) {
        case 1:
            handleAddBook();
            break;
        case 2:
            handleUpdateBook();
            break;
        case 3:
            handleDeleteBook();
            break;
        case 4:
            handleSearchBook();
            break;
        case 5:
            handleDisplayAllBooks();
            break;
        case 6:
            handleGenerateStatistics();
            break;
        case 7:
            handleClearAllBooks();
            break;
        case 0:
            std::cout << "感谢使用图书馆管理系统，再见！" << std::endl;
            break;
        default:
            std::cout << "无效的选择，请重新输入！" << std::endl;
        }
    } while (choice != 0);
}