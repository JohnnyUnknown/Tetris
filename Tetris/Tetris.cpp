#include <iostream>
#include <windows.h>
#include <stdlib.h>
#include <time.h>
#include <conio.h>

using namespace std;

class Box 
{
    class Figure
    {
    public:
        int** figure;           // Массив для хранения фигуры
        int identifier;         // Идентификатор выбора фигуры
        int figureSize;         // Размер квадратного массива figure
        
        // Конструктор фигуры по умолчанию
        Figure();
        // Выделение памяти массиву figure и присвоение элементам значений 0 по умолчанию
        void sizeFigure();
        // Генерация новой фигуры
        void figureGenerate();
        // Удаление старой фигуры
        void nullFigure();
        // Поворот фигуры против часовой стрелки
        void turnRight();
        ~Figure() { nullFigure(); }
    };


    int box[16][11]{ 0 };       // Массив для хранения поля
    Figure actionFigure;        // Объект для действующей фигуры
    int leftShift;              // Смещение влево при передаче фигуры в массив box
    int stepDown;               // Смещение вниз при движении фигуры

public:
    Box();

    // Распечатка состояния поля
    void printBox() const;

    // Временная задержка. Выбирается в 10 раз меньше из-за множителя countStepDown в figureMove()
    void timeDelay() { Sleep(200); }  

    // Передача фигуры в игровое поле
    void figureTransfer();
    
    // Опускание фигуры до конца
    void figureMove();

    // Предотвращение выхода фигуры за пределы поля после поворота
    bool collision();

    // Проверки движения фигуры на остановку
    bool checkMoveDown();

    // Проверки движения фигуры на выход за пределы поля
    bool checkMoveLeft();

    // Проверки движения фигуры на выход за пределы поля
    bool checkMoveRight();

    // Присвоение фигуре статуса "3 - остановившаяся", удаление старой и генерация новой фигуры
    void endMove();

    // Удаление заполненных рядов
    void deleteRows();

    // Окончание игры
    bool gameOver();
    
    // Очистка поля
    void clearBox();
    
    // Управление фигурой
    void control();

    // Управление игрой
    void game();

    ~Box() { actionFigure.nullFigure(); }
};


//--------------------------------------------------------------------------


int main()
{
    setlocale(LC_ALL, "Rus");
    srand(time(NULL));
    Box b;
    b.game();
}


//--------------------------------------------------------------------------


// Конструктор фигуры по умолчанию
Box::Figure::Figure() : figure{ nullptr }, identifier{ 0 }, figureSize{ 0 }
{
    figureGenerate();
}

// Выделение памяти массиву figure и присвоение элементам значений 0 по умолчанию
void Box::Figure::sizeFigure()
{
    figure = new int* [figureSize];
    for (int i = 0; i < figureSize; i++)
    {
        figure[i] = new int[figureSize];
        for (int j = 0; j < figureSize; j++)
        {
            figure[i][j] = 0;
        }
    }
}

// Генерация новой фигуры
void Box::Figure::figureGenerate()
{
    identifier = rand() % 6 + 1;    // rand() % 6 + 1
    (identifier >= 1 && identifier <= 3) ? figureSize = 3 :
        (identifier == 4) ? figureSize = 4 : figureSize = 2;
    sizeFigure();
    switch (identifier)
    {
    case 1:
        figure[0][1] = 1;
        figure[1][0] = 1;
        figure[1][1] = 1;
        figure[1][2] = 1;
        break;
    case 2:
        figure[0][0] = 1;
        figure[1][0] = 1;
        figure[1][1] = 1;
        figure[1][2] = 1;
        break;
    case 3:
        figure[0][2] = 1;
        figure[1][0] = 1;
        figure[1][1] = 1;
        figure[1][2] = 1;
        break;
    case 4:
        figure[0][1] = 1;
        figure[1][1] = 1;
        figure[2][1] = 1;
        figure[3][1] = 1;
        break;
    case 5:
        figure[0][0] = 1;
        figure[1][0] = 1;
        figure[0][1] = 1;
        break;
    case 6:
        figure[0][0] = 1;
        figure[1][0] = 1;
        figure[0][1] = 1;
        figure[1][1] = 1;
        break;
    default:
        break;
    }
}

// Удаление старой фигуры
void Box::Figure::nullFigure()
{
    if (identifier)
    {
        for (int i = 0; i < figureSize; i++)
        {
            delete[] figure[i];
        }
        delete[]figure;
        identifier = 0;
        figureSize = 0;
    }
}

// Поворот фигуры против часовой стрелки
void Box::Figure::turnRight()
{
    // Временный массив для хранения текущего положения фигуры
    int** tempFigure;
    tempFigure = new int* [figureSize];
    for (int i = 0; i < figureSize; i++)
    {
        tempFigure[i] = new int[figureSize];
        for (int j = 0; j < figureSize; j++)
        {
            tempFigure[i][j] = figure[i][j];
        }
    }
    // Поворот текущей фигуры
    if (figureSize == 2)
    {
        figure[0][0] = tempFigure[0][1];
        figure[0][1] = tempFigure[1][1];

        figure[1][0] = tempFigure[0][0];
        figure[1][1] = tempFigure[1][0];
    }
    if (figureSize == 3)
    {
        figure[0][0] = tempFigure[0][2];
        figure[0][1] = tempFigure[1][2];
        figure[0][2] = tempFigure[2][2];

        figure[1][0] = tempFigure[0][1];
        figure[1][1] = tempFigure[1][1];
        figure[1][2] = tempFigure[2][1];

        figure[2][0] = tempFigure[0][0];
        figure[2][1] = tempFigure[1][0];
        figure[2][2] = tempFigure[2][0];
    }
    if (figureSize == 4)
    {
        figure[0][0] = tempFigure[0][3];
        figure[0][1] = tempFigure[1][3];
        figure[0][2] = tempFigure[2][3];
        figure[0][3] = tempFigure[3][3];

        figure[1][0] = tempFigure[0][2];
        figure[1][1] = tempFigure[1][2];
        figure[1][2] = tempFigure[2][2];
        figure[1][3] = tempFigure[3][2];

        figure[2][0] = tempFigure[0][1];
        figure[2][1] = tempFigure[1][1];
        figure[2][2] = tempFigure[2][1];
        figure[2][3] = tempFigure[3][1];

        figure[3][0] = tempFigure[0][0];
        figure[3][1] = tempFigure[1][0];
        figure[3][2] = tempFigure[2][0];
        figure[3][3] = tempFigure[3][0];
    }
    // Удаление временной массива
    for (int i = 0; i < figureSize; i++)
    {
        delete[] tempFigure[i];
    }
    delete[]tempFigure;
}

//--------------------------------------------------------------------------

// Конструктор поля по умолчанию
Box::Box() : box{ 0 }, actionFigure(), leftShift{ 4 }, stepDown{ 0 }
{
    // 0 - пустая клетка поля; 1 - движущаяся фигура; 2 - граница игрового поля; 3 - остановившаяся фигура.
    for (int i = 0; i < 16; i++)
    {
        for (int j = 0; j < 11; j++)
        {
            if ((j == 0 || j == 10) && i < 15) box[i][j] = 2;
            else if (i == 15) box[i][j] = 2;
            //else if (i == 11 && j > 2) box[i][j] = 3;
        }
    }
}

// Распечатка состояния поля
void Box::printBox() const
{
    system("cls");
    cout << "\t     T E T R I S\n\n";
    for (int i = 0; i < 16; i++)
    {
        cout << "\t";
        for (int j = 0; j < 11; j++)
        {
            if ((j == 0 || j == 10) && i < 15) cout << "||";
            else if (i == 15) cout << "==";
            else
            {
                if (box[i][j] == 0) cout << "  ";
                if (box[i][j] == 1) cout << "[]";
                if (box[i][j] == 3) cout << "[]";
            }
        }
        cout << endl;
    }
    cout << endl << endl;
}

// Передача фигуры в игровое поле
void Box::figureTransfer()
{
    clearBox();
    if (collision());
    for (int i = stepDown; i < actionFigure.figureSize + stepDown; i++)
    {
        for (int j = leftShift; j < actionFigure.figureSize + leftShift; j++)
        {
            if (actionFigure.figure[i - stepDown][j - leftShift] == 1)
            {
                box[i][j] = actionFigure.figure[i - stepDown][j - leftShift];
            }
        }
    }
}

// Опускание фигуры до конца
void Box::figureMove()
{
    figureTransfer();
    bool stop = false;
    int count = 0;
    int i = (actionFigure.identifier == 4) ? i = 3 : i = 1;  // Определение значения i для контроля нижней границы фигуры
    for (; stop == false; i++)
    {
        while (!_kbhit())
        {
            stop = checkMoveDown();
            if (!stop)
            {
                if (count++ == 5)           // Шаг вниз раз в count тактов
                {
                    stepDown++;             // Опускание фигуры на один шаг
                    count = 0;
                }
                figureTransfer();
                printBox();
                timeDelay();
            }
            else break;
        }
        if (stop) break;
        control();
        figureTransfer();
    }
    endMove();
}

// Предотвращение выхода фигуры за пределы поля после поворота
bool Box::collision()
{
    for (int i = stepDown; i < actionFigure.figureSize + stepDown; i++)
    {
        for (int j = leftShift; j < actionFigure.figureSize + leftShift; j++)
        {
            if ((box[i][j] == 2 || box[i][j] == 3)
                && actionFigure.figure[i - stepDown][j - leftShift] == 1 && j == 10)
            {
                leftShift--;
                return collision();
            }
            else if ((box[i][j] == 2 || box[i][j] == 3)
                && actionFigure.figure[i - stepDown][j - leftShift] == 1 && j == 0)
            {
                leftShift++;
                return collision();
            }
        }
    }
    return true;
}

// Проверки движения фигуры на остановку
bool Box::checkMoveDown()
{
    for (int k = 14; k >= 0; k--)
    {
        for (int j = 0; j < 11; j++)
        {
            if (box[k + 1][j] == 2 && box[k][j] == 1 || box[k + 1][j] == 3 && box[k][j] == 1)
            {
                return true;
            }
        }
    }
    return false;
}

// Проверки движения фигуры на выход за пределы поля
bool Box::checkMoveLeft()
{
    for (int k = 15; k >= 0; k--)
    {
        for (int j = 0; j < 11; j++)
        {
            if (box[k][j] == 1 && (box[k][j - 1] == 2 || box[k][j - 1] == 3))
            {
                return true;
            }
        }
    }
    return false;
}

// Проверки движения фигуры на выход за пределы поля
bool Box::checkMoveRight()
{
    for (int k = 15; k >= 0; k--)
    {
        for (int j = 0; j < 11; j++)
        {
            if ((box[k][j + 1] == 2 && box[k][j] == 1) || (box[k][j + 1] == 3 && box[k][j] == 1))
            {
                return true;
            }
        }
    }
    return false;
}

// Присвоение фигуре статуса "3 - остановившаяся", удаление старой и генерация новой фигуры
void Box::endMove()
{
    for (int i = 0; i < 15; i++)
    {
        for (int j = 0; j < 11; j++)
        {
            if (box[i][j] == 1) box[i][j] = 3;
        }
    }
    leftShift = 4;
    stepDown = 0;
    actionFigure.nullFigure();
    actionFigure.figureGenerate();
}

// Удаление заполненных рядов
void Box::deleteRows()
{
    int countCell = 0, numRow = 0;      // счетчик заполненных клеток
    bool metkaClear = false;        // метка будет true в случае очистки

    for (int i = 0; i < 15; i++)
    {
        countCell = 0;              // с новой строкой счетчик обнуляется
        for (int j = 1; j < 10; j++)
        {
            if (box[i][j] == 3)     // проверка на заполненность ряда
            {
                countCell++;
                if (j == 9 && countCell == 9)
                {
                    // если ряд заполнен, он очищается
                    metkaClear = true;
                    numRow = i;
                    for (int k = 1; k < 10; k++)
                    {
                        box[i][k] = 0;
                    }
                    break;
                }
            }
        }
        if (metkaClear) break;
    }
    // сдвиг значений поля на строку вниз от удаленной строки
    if (metkaClear)
    {
        for (int i = numRow; i >= 0; i--)
        {
            for (int j = 1; j < 10; j++)
            {
                if (box[i][j] == 0 || box[i][j] == 3)
                    box[i][j] = box[i - 1][j];
                if (i == 0) box[i][j] = 0;
            }
        }
        return deleteRows();         // рекурсия на случай нескольких заполненных рядов
    }
}

// Окончание игры
bool Box::gameOver()
{
    bool stopGame = false;
    int i = 0;
    for (int j = 1; j < 10; j++)
    {
        if (box[i][j] == 3) stopGame = true;
    }
    return stopGame;
}

// Очистка поля
void Box::clearBox()
{
    for (int i = 0; i < 15; i++)
    {
        for (int j = 0; j < 11; j++)
        {
            if (box[i][j] == 1) box[i][j] = 0;
        }
    }
}

// Управление фигурой
void Box::control()
{
    bool permission = false;
    int input;
    _getch();
    input = _getch();
    switch (input)
    {
        // Стрелка влево. Смещение влево
    case 75: permission = checkMoveLeft();
        if (!permission)
        {
            leftShift--;
        }
        break;
        // Стрелка вправо. Смещение вправо
    case 77: permission = checkMoveRight();
        if (!permission)
        {
            leftShift++;
        }
        break;
        // Стрелка вниз. Быстро вниз
    case 80:
        while (!checkMoveDown())
        {
            stepDown++;
            figureTransfer();
            printBox();
        }
        break;
        // Стрелка вверх. Поворот против часовой стрелки
    case 72: actionFigure.turnRight();
        break;
    default: break;
    }
}

// Управление игрой
void Box::game()
{
    while (!gameOver())
    {
        figureMove();
        deleteRows();
    }
}