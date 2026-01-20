#ifndef LIBRARY_MANAGEMENT_SYSTEM_H
#define LIBRARY_MANAGEMENT_SYSTEM_H
#include <string>
#include <vector>
#include <iostream>
#include <algorithm>
#include <ctime>
#include <fstream>
#include <sstream>
#include <limits>

// 图书类
class Book {
private:
    std::string isbn;          // ISBN号
    std::string title;         // 书名
    std::string author;        // 作者
    std::string publisher;     // 出版社
    std::string publishDate;   // 出版日期
    double price;              // 价格
    int quantity;              // 数量

public:
    // 构造函数
    Book(std::string isbn, std::string title, std::string author,
        std::string publisher, std::string publishDate, double price, int quantity);

    // 获取属性
    std::string getISBN() const;
    std::string getTitle() const;
    std::string getAuthor() const;
    std::string getPublisher() const;
    std::string getPublishDate() const;
    double getPrice() const;
    int getQuantity() const;

    // 设置属性
    void setTitle(std::string title);
    void setAuthor(std::string author);
    void setPublisher(std::string publisher);
    void setPublishDate(std::string publishDate);
    void setPrice(double price);
    void setQuantity(int quantity);

    // 显示图书信息
    void displayInfo() const;

    // 转换为字符串格式（用于文件存储）
    std::string toString() const;

    // 从字符串解析（用于文件读取）
    static Book fromString(const std::string& str);
};

// 图书馆系统类
class Library {
private:
    std::vector<Book> books;   // 图书集合
    const std::string dataFile = "book.txt";  // 数据文件路径

    // 查找图书索引
    int findBookIndex(const std::string& isbn) const;

    // 加载图书数据
    void loadBooks();

    // 保存图书数据
    void saveBooks() const;

public:
    // 构造函数
    Library();

    // 图书管理功能
    bool addBook(const Book& book);
    bool updateBook(const std::string& isbn, const Book& newBookInfo);
    bool deleteBook(const std::string& isbn);
    bool searchBookByISBN(const std::string& isbn) const;
    void searchBookByTitle(const std::string& title) const;
    void searchBookByAuthor(const std::string& author) const;
    void displayAllBooks() const;
    void clearAllBooks();
    void generateStatistics() const;

    // 辅助功能
    bool isISBNExists(const std::string& isbn) const;
    int getTotalBooksCount() const;
    double getTotalValue() const;

    bool getBook(const std::string& isbn, Book& outBook) const;
};

// 用户界面类
class LibraryUI {
private:
    Library library;

    // 菜单显示
    void displayMainMenu() const;
    void displaySearchMenu() const;

    // 输入验证
    bool isValidISBN(const std::string& isbn) const;
    bool isValidPrice(double price) const;
    bool isValidQuantity(int quantity) const;

    // 输入处理
    std::string getStringInput(const std::string& prompt) const;
    double getDoubleInput(const std::string& prompt) const;
    int getIntInput(const std::string& prompt) const;

    // 新增：可选数值输入（空行表示保留原值）
    bool getOptionalDoubleInput(const std::string& prompt, double& out) const;
    bool getOptionalIntInput(const std::string& prompt, int& out) const;

    // 功能处理
    void handleAddBook();
    void handleUpdateBook();
    void handleDeleteBook();
    void handleSearchBook();
    void handleDisplayAllBooks();
    void handleClearAllBooks();
    void handleGenerateStatistics();

public:
    // 运行系统
    void run();
};

#endif // LIBRARY_MANAGEMENT_SYSTEM_H
