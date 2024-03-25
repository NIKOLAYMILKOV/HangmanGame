#include <iostream>
#include <cstring>
#include <fstream>
#include <Windows.h>
// #include <ctime>
#include <thread>
#include <chrono>
const size_t lettersInAlphabeth = 26;


void printPicture(int mistakes)
{
    char c;
    char a = mistakes + '0';
    char mstk[5] = {a};
    char file[15] = "pic";
    strcat(file, strcat(mstk, ".txt"));

    std::ifstream f(file);
    if (!f.is_open())
    {
        //std::cout << "Error" << std::endl;
        return;
    }

    while (!f.eof())
    {
        c = f.get();
        std::cout << c;
    }
}

char* createHiddenWord(const char* str, const size_t size)
{
    char* hiddenWord = new(std::nothrow) char[size + 1];
    if (!hiddenWord)
    {
        std::cout << "Memory Error";
        delete[] hiddenWord;
        return nullptr;
    }

    for (size_t i = 1; i < size - 1; i++)
    {
        hiddenWord[i] = '_';
    }
    
    hiddenWord[size] = 0;
    hiddenWord[size - 1] = str[size - 1];
    hiddenWord[0] = str[0];

    return hiddenWord;
}

int randomNumberRange()
{
    std::ifstream f("words.txt");
    if (!f.is_open())
    {
        std::cout << "Error" << std::endl;
        return -1;
    }

    int range;
    char c;
    while (!f.eof())
    {
        c = f.get();
        if (c == '\n' || f.eof()) range++;
    }
    
    f.seekg(0);
    f.close();
    return range;
}

int randomNumber()
{
    int range = randomNumberRange();
    srand(time(0));
    int x = (rand() % range);

    return x;
}

char* words()
{
    int randomIndex = randomNumber();
    
    std::ifstream f("words.txt");
    if (!f.is_open())
    {
        std::cout << "File could not open";
        return nullptr;
    }
    const size_t size = 100;
    char* word = new(std::nothrow) char[size];
    
    for (size_t i = 0; i < randomIndex; i++)
    {
        f >> word;
    }

    f.close();
    return word;
}

int foundLetters(const char* str, const size_t n, char input, char* hiddenWord)
{
    int currCount = 0;
    int lettersFound = 0;
    
    for (size_t i = 1; i < n - 1; i++)
    {
        if (input == str[i] || input == str[i] - ' ')
        {
            hiddenWord[i] = str[i];
            lettersFound++;
        }else currCount++;  
    }

    if (currCount == n - 2) return 0;
    
    return lettersFound;
}



void printUsedLetters(char* usedLetters, size_t usedLettersSize)
{
    for (size_t i = 0; i < usedLettersSize; i++)
    {
        std::cout << "|" << usedLetters[i] << "|";
    }
    std::cout << std::endl;
}

bool isUsedLetter(char* usedLettersArray, char letter, size_t& size)
{
    size++;
    for (size_t i = 0; i < size; i++)
    {
        if (usedLettersArray[i] == '0')
        {
            usedLettersArray[i] = letter;
            size--;
        }else if (usedLettersArray[i] == letter)
        {
            size--;
            return true;
        }
    }
    size++;
    return false;
}

void inputValidation(char& guess)
{   
    while ((guess < 'a' || guess > 'z') && (guess < 'A' || guess > 'Z'))
    {
        std::cout << "You must input a letter!" << std::endl;
        std::cin >> guess;
    }
}

int main()
{
    //system("CLS");
    char* wordToGuess = words();
    const size_t size = strlen(wordToGuess);
    char usedLetters[lettersInAlphabeth];
    std::fill(usedLetters, usedLetters + 26, '0');
    size_t usedLettersSize = 0;
    //std::cout << wordToGuess << std::endl;



    char* hiddenWord = createHiddenWord(wordToGuess, size);
    //std::cout << hiddenWord << std::endl;

    //printPicture(5);

    int counterCorect = 0;
    int counterIncorect = 0;
    char guess;

    while (counterCorect < size - 2 && counterIncorect < 9)
    {
        system("CLS");
        std::cout << hiddenWord << std::endl;
        printPicture(counterIncorect);
        std::cout << "Remaining tries: " << 9 - counterIncorect << std::endl;
        std::cout << "Used Letters" << std::endl;
        printUsedLetters(usedLetters, usedLettersSize);
        
        std::cout << "Input letter: " << std::endl;
        std::cin >> guess;
        inputValidation(guess);
        
        if (isUsedLetter(usedLetters, guess, usedLettersSize))
        {
            std::cout << "You have already used this letter" << std::endl;
            std::chrono::seconds duration(2); 
            std::this_thread::sleep_for(duration); 
        }else
        {
            if (foundLetters(wordToGuess, size, guess, hiddenWord) == 0)
            {
                std::cout << "Invalid" << std::endl;
                counterIncorect++;
                
                printPicture(counterIncorect);
            }
            else counterCorect += foundLetters(wordToGuess, size, guess, hiddenWord);
        }
        
        //system("CLS");
        
    }

    system("CLS");
    printPicture(counterIncorect);

    if (counterCorect == size - 2)
    {
        std::cout << hiddenWord << std::endl;
        std::cout << "You Won!";
    }
    
    if(counterIncorect == 9)
    {
        std::cout << "You Lost!" << std::endl << "The word is: " << wordToGuess << std::endl;
        std::cout << "Good luck next time!" << std::endl;
    }

    delete[] wordToGuess;
    delete[] hiddenWord;

    return 0;
}