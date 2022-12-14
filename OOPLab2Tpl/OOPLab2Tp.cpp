#define _CRT_SECURE_NO_WARNINGS
#include "Windows.h"
#include <iostream>
#include <fstream>
#undef max

void CText(const char t[], int color) {
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), color);
    std::cout << t;
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 15);
}



int bMult(int a, int b) { // 2 3
    int result = 0;
    while (b > 0) {
        if (b & 1) { // Check on pairing
            result += a;
        }
        a = a << 1;
        b = b >> 1;
    }
    return result;
}
// 2 x 3 logic
// 2 3 0
// 4 1 2
// 8 0 6



long long bDiv(long long dividend, long long int divisor) {

    // Calculate sign of divisor i.e.,
    // sign will be negative only if
    // either one of them is negative
    // otherwise it will be positive
    long long sign = ((dividend < 0) ^ (divisor < 0)) ? -1 : 1;

    // Update both divisor and
    // dividend positive
    dividend = abs(dividend);
    divisor = abs(divisor);

    // Initialize the quotient
    long long quotient = 0;
    while (dividend >= divisor) {
        dividend -= divisor;
        ++quotient;
    }

    // Return the value of quotient with the appropriate
    // sign.
    return quotient * sign;
}



void inNum(long long& num) {
    char buff[255];
    std::cin >> num;

    if (std::cin.fail()) {
        std::cout << "  ";
        CText("  Невірно введене число!   \n", 112);
        CText("  Введіть ціле число знову : ", 3);
        std::cin.clear();
        std::cin >> buff;
        inNum(num);
    }
}



void V11_Task1() {
    long long a, b, c, d;

    CText("  Введіть ціле число 'a' : ", 3); inNum(a);
    CText("  Введіть ціле число 'b' : ", 3); inNum(b);
    CText("  Введіть ціле число 'c' : ", 3); inNum(c);
    CText("  Введіть ціле число 'd' : ", 3); inNum(d);
   
    CText("  Результат : ", 112);
    std::cout << "  " << bDiv(bMult(127, a) + bMult(32, c), 4096) - bMult(d, 1200) + bMult(b, 131);
}



void binaryView(unsigned char a) {
    int b[8];

    memset(b, 0, sizeof(b));

    b[0] = a & 0b10000000 ? 1 : 0;
    b[1] = a & 0b01000000 ? 1 : 0;
    b[2] = a & 0b00100000 ? 1 : 0;
    b[3] = a & 0b00010000 ? 1 : 0;
    b[4] = a & 0b00001000 ? 1 : 0;
    b[5] = a & 0b00000100 ? 1 : 0;
    b[6] = a & 0b00000010 ? 1 : 0;
    b[7] = a & 0b00000001 ? 1 : 0;

    for (int i = 0; i < 8; i++) {
        std::cout << b[i];
    }
}



void encrypt(unsigned char* text, unsigned char* destination) {
    unsigned char half1(0), half2(0);
    unsigned char row(0), collum(0);
    unsigned char pairing0_1(0), pairing13_14(0);

    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 16; j += 2) {
            row = i;                                             row <<= 5;   row &= 0b11100000;
            collum = j;
            half1 = text[(i << 3) + (j >> 1)] & 0b11110000;      half1 >>= 3; half1 &= 0b00011110;
            half2 = text[(i << 3) + (j >> 1)] & 0b00001111;      half2 <<= 4; half2 &= 0b11110000;
            pairing0_1 = row & 0b01000000 ? 1 : 0;
            pairing13_14 = collum & 0b00000010 ? 1 : 0;
            destination[(i << 4) + j] = row | half1 | pairing0_1;
            destination[(i << 4) + j + 1] = collum | half2 | pairing13_14;
        }
    }
}



void decrypt(unsigned char* text, unsigned char* destination) {
    unsigned char half1(0), half2(0);
    unsigned char row(0), collum(0);

    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 16; j += 2) {
            row = text[(i << 4) + j] & 0b11100000;               row >>= 5;     row &= 0b00000111;
            collum = text[(i << 4) + j + 1] & 0b00001110;        collum >>= 1;  collum &= 0b00000111;
            half1 = text[(i << 4) + j] & 0b00011110;             half1 <<= 3;   half1 &= 0b11110000;
            half2 = text[(i << 4) + j + 1] & 0b11110000;         half2 >>= 4;   half2 &= 0b00001111;
            destination[(row << 3) + collum] = half1 | half2;
        }
    }
}



int ranRange(int min, int max) {
    int range = max - min + 1;
    int num = rand() % range + min;

    return num;
}



void mixEncArray(unsigned char* text, unsigned char* destination) {
    int ranPos;
    int ranCheck[128];
    memset(destination, 137, sizeof(destination));
    memset(ranCheck, 0, sizeof(ranCheck));
    srand(time(NULL));

    for (int i = 0; i < 128; i += 2) {
        ranPos = (int)ranRange(0, 63) * 2;
        if (!ranCheck[ranPos]) {
            if (!ranPos) ranCheck[0] = 1;
            ranCheck[ranPos] = 1;
            destination[ranPos] = text[i];
            destination[ranPos + 1] = text[i + 1];
        }
        else i -= 2;
    }
}



void WriteArrayTextFile(int n, unsigned char* arr, const char* fileName) {
    std::ofstream fout(fileName);
    if (fout.fail()) return;

    for (int i = 0; i < n; i++)
        fout << arr[i];


   
    std::cout << "\n\n  ";
    CText("Дані збережено у файл  ", 112);
    CText(fileName, 128);
    CText("\n  ", 0);

    char full[_MAX_PATH];
    if (_fullpath(full, fileName, _MAX_PATH) != NULL)
        CText(full, 128);

    CText("\n", 0);

    fout.close();
}



int ReadArrayTextFile(int n, unsigned char* arr, const char* fileName) {
    char fileAddress[1024];
    memset(fileAddress, 0, sizeof(fileAddress));

    std::ifstream fin(fileName);
    fin >> std::noskipws;
    
    if (fin.fail()) {
        std::cout << "  ";
        CText("Неможливо відкрити файл!\n", 112);
        std::cout << "  ";
        CText("Введіть ім'я файлу (1.txt) (..\\1.txt) : ", 3);
        std::cin >> fileAddress;
        n = ReadArrayTextFile(n, arr, fileAddress);
        return n;
    }

    if (n <= 0) {
        std::cout << "  ";
        CText("Файл пустий!\n", 112);
        return 0;
    }
    std::cout << "  ";
    CText("Вміст файлу : \n", 112);
    std::cout << "  ";
    for (int i = 0; i < n; i++) {
        fin >> arr[i];
        std::cout << arr[i];
        if (arr[i] == '\n')
            std::cout << "  ";
    }
    std::cout << "\n";
    fin.close();
    return n;
}



void ConsoleInputArray(int sizeMax, unsigned char *A) {
    unsigned char buff[256];

    std::cout << "  ";
    CText("  Введіть текст (максимальна довжина - 64, лишне не опрацюється) : ", 112);
    std::cout << "\n  ";
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    for (int i = 0; i < sizeMax; i++) {
        A[i] = getchar();
    }
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
}



void V11_Task2(unsigned char *encrypChar = NULL, unsigned char *encrypAndMixChar = NULL) {
    unsigned char c;
    unsigned char decrypted[65];
    unsigned char encryptedBase[128];
    unsigned char encryptedAndMixedBase[128];
    unsigned char *encrypted = (unsigned char*)encryptedBase;
    unsigned char *encryptedAndMixed = (unsigned char*)encryptedAndMixedBase;
    memset(decrypted, 0, sizeof(decrypted));
    memset(encrypted, 0, sizeof(encrypted));
    memset(encryptedAndMixed, 0, sizeof(encryptedAndMixed));

    if ((encrypChar != NULL) && (encrypAndMixChar != NULL)) {
        encrypted = encrypChar;
        encryptedAndMixed = encrypAndMixChar;
    }


    std::cout << "\n";

    CText("  1. Зашифрувати массив  8х8 (з консолі)\n", 11);
    CText("  2. Зашифрувати массив  8х8 (з файлу \"decrypted.txt\")\n", 11);
    CText("  3. Розшифрувати массив 8х8 (у файл \"encrypted.txt\")\n\n\n\n", 11);
   
    do {
        CText("  Виберіть дію з меню : ", 3);
        std::cin >> c;
  
        if ((c != '1') && (c != '2') && (c != '3')) {
            std::cout << "  ";
            CText("Невірно введений запит! Введіть число від 1 до 2\n", 112);
        }
    } while ((c != '1') && (c != '2') && (c != '3'));

    if (c == '1') {
        ConsoleInputArray(64, decrypted);
        encrypt(decrypted, encrypted);

        std::cout << "\n  ";
        CText("Зашифрованний вигляд : \n", 112);
        std::cout << "  ";

        for (int i = 0; i < 128; i++) {
            std::cout << encrypted[i];
            if (i == 63) std::cout << "\n  ";
        }


        mixEncArray(encrypted, encryptedAndMixed);

        std::cout << "\n\n  ";
        CText("Зашифрованний і перемішаний вигляд : \n", 112);
        std::cout << "  ";

        for (int i = 0; i < 128; i++) {
            std::cout << encryptedAndMixed[i];
            if (i == 63) std::cout << "\n  ";
        }


        WriteArrayTextFile(128, encryptedAndMixed, "encrypted.txt");
    }
    if (c == '2') {
        ReadArrayTextFile(64, decrypted, "decrypted.txt");
        encrypt(decrypted, encrypted);

        std::cout << "\n  ";
        CText("Зашифрованний вигляд : \n", 112);
        std::cout << "  ";

        for (int i = 0; i < 128; i++) {
            std::cout << encrypted[i];
            if (i == 63) std::cout << "\n  ";
        }


        mixEncArray(encrypted, encryptedAndMixed);

        std::cout << "\n\n  ";
        CText("Зашифрованний і перемішаний вигляд : \n", 112);
        std::cout << "  ";

        for (int i = 0; i < 128; i++) {
            std::cout << encryptedAndMixed[i];
            if (i == 63) std::cout << "\n  ";
        }


        WriteArrayTextFile(128, encryptedAndMixed, "encrypted.txt");
    }
    if (c == '3') {
        ReadArrayTextFile(128, encrypted, "encrypted.txt");
        decrypt(encrypted, decrypted);

        std::cout << "\n  ";
        CText("Розшифрованийн вигляд : ", 112);
        std::cout << "\n  ";

        for (int i = 0; i < 64; i++) {
            std::cout << decrypted[i];
        }

        WriteArrayTextFile(64, decrypted, "decrypted.txt");
    }
}
  


struct encryptedChar {
    unsigned char row : 3;
    unsigned char half1 : 4;
    unsigned char pairing0_1 : 1;
    unsigned char half2 : 4;
    unsigned char collum : 3;
    unsigned char pairing13_14 : 1;
};



void V11_Task3() {
    encryptedChar encrypted[64]; 
    encryptedChar encryptedAndMixed[64];    
    unsigned char* encryptedAddress = (unsigned char*)encrypted;
    unsigned char* encryptedAndMixedAddress = (unsigned char*)encryptedAndMixed;
    memset(encrypted, 0, sizeof(encrypted));
    memset(encryptedAndMixed, 0, sizeof(encryptedAndMixed));

    V11_Task2(encryptedAddress, encryptedAndMixedAddress);
}



void V11_Task4() {
    char arr[16];

    for (int i = 0; i < 16; i++) {
        arr[i] = i;
        binaryView(arr[i]);
        std::cout << " = " << i << "    ";
        binaryView(255 - (i - (int)(i / 256) * 256));
        std::cout << " = " << 255 - (i - (int)(i / 256) * 256) << "  ";
        std::cout << "  ";
        arr[i] ^= 255 - (i - (int)(i / 256) * 256);
        binaryView(arr[i]);
        std::cout << "\n";
    }
}


void ShowMainMenu() {
    std::cout << "  ";
    CText("         Меню          \n", 225);
    std::cout << '\n';
    CText("  1. Обчислення виразів із використання побітових операцій   \n", 11);
    CText("  2. Шифрування інформації\n", 11);
    CText("  3. Шифрування інформації. Структура з полями бітів. Об'єднання\n", 11);
    CText("  4. Кодування бітів\n", 11);
    CText("  5. Показати меню\n", 11);
    CText("  6. Вийти \n\n\n", 11);

    CText("  Виберіть дію з меню : ", 3);
}



void MenuManager() {
    int key;
    char buff[256];


    std::cin >> key;

    if (std::cin.fail()) {
        std::cout << "  ";
        CText("Невірно введений запит! Введіть число від 1 до 5\n", 112);
        std::cin.clear();
        std::cin >> buff;
        MenuManager();
    }
    else switch (key) {

    case 1:
        V11_Task1();

        CText("\n\n\n  Виберіть дію з меню : ", 3);
        MenuManager();

    case 2:
        V11_Task2();
        CText("\n\n\n  Виберіть дію з меню : ", 3);
        MenuManager();

    case 3:
        V11_Task3();
        CText("\n\n\n  Виберіть дію з меню : ", 3);
        MenuManager();

    case 4:
        V11_Task4();
        CText("\n\n\n  Виберіть дію з меню : ", 3);
        MenuManager();

    case 5:
        system("cls");
        ShowMainMenu();
        MenuManager();

    case 6:
        exit(0);

    default:
        std::cout << "  ";
        CText("Вибране не правильне завдання!\n", 112);
        CText("\n\n\n  Виберіть дію з меню : ", 3);
        MenuManager();
        break;
    }
}



int main() {
    system("chcp 1251");
    system("cls");

    ShowMainMenu();
    MenuManager();
}