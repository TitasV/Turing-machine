#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <windows.h>
#include <conio.h>
#include <thread>
#include <mutex>
#include <chrono>

std::mutex m;

class Code
{
private:
    std::string cs; //Current state
    char CS; //Current symbol
    char NS; //New symbol
    char d; //Direction
    std::string ns; //New state
public:
    //Konstruktoriai su ir be parametrais
    Code(): cs("*"), CS('*'), NS('*'), d('*'), ns("*") {};
    Code(std::string cs1, char CS1, char NS1, char d1, std::string ns1) : cs(cs1), CS(CS1), NS(NS1), d(d1), ns(ns1) {};
    //Metodai
    void setdata(std::string cs1, char CS1, char NS1, char d1, std::string ns1)
    {
        cs = cs1;
        CS = CS1;
        NS = NS1;
        d = d1;
        ns = ns1;
    }
    void getdata()
    {
        std::cout << cs << " " << CS << " " << NS << " " << d << " " << ns << std::endl;
    }
    std::string getcs() { return cs; }
    char getCS() { return CS; }
    char getNS() { return NS; }
    char getd() { return d; }
    std::string getns() { return ns; }
};
///Funkciju antraste
//Meniu funkcija
std::string Meniu(std::string &x)
{
    system("CLS");
    std::cout << "1) Vykdyti 1 faila" << std::endl;
    std::cout << "2) Vykdyti 2 faila" << std::endl;
    std::cout << "3) Vykdyti 3 faila" << std::endl;
    std::cout << "4) Vykdyti 4 faila" << std::endl;
    std::cout << "5) Vygdyti visas programas vienu metu" << std::endl;
    std::cout << "6) Baigti programa" << std::endl;
    std::cin >> x;
    return x;
}
void Vykdymas(std::vector<Code> C, std::string Tape, std::string x, int eil, int miegoti)
{
    ///Skaitymas
    m.lock();
    int pos; //kintamasis nusakantis nuo kurios vietos pradeti skaityti "Tape";
    std::string cs1, ns1; //Tarpinis nuskaitimo kintamasis
    char CS1, NS1, d1; //Tarpinis nuskaitimo kintamasis
    Code Y; //Tarpinis nuskaitimo kintamasis
    std::ifstream fd(x + ".txt");;
    fd >> Tape;
    fd >> pos;
    //Skaitymas iki pabaigos
    while (fd >> cs1)
    {
        fd >> CS1 >> NS1 >> d1 >> ns1;
        Y.setdata(cs1, CS1, NS1, d1, ns1);
        C.push_back(Y);
    }
    m.unlock();
    ///Vykdymas
    std::string state = "0"; //Esamas "state"
    int I=0; //Nusako ar kodas baigesi
    bool y = true; //Nusako ar testi kodo seka;
    //Vykdo tol, kol galvute nenueis nuo Tape, nebus paspaustas klavisas ir bus rasta kodo eilute kuria galima vykdyti
    while (pos>0 && pos<Tape.size() && y)
    {
        //Lauks klaviso paspaudimo
        if (_kbhit()) y = false;
        //Tape spausdinimas
        m.lock();
        for (int i = 0; i < Tape.size(); i++)
        {
            COORD coord;
            coord.X = i;
            coord.Y = eil - 1;
            SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
            if (i == pos) std::cout << "_";
            else std::cout << Tape[i];
        }
        std::cout << std::endl << "Press any key to \"halt\" " << std::endl;
        //Tikrina ar galvute nenuejo nuo "Tape"
        if (I >= C.size())
        {
            y = false;
            std::cout << "halted";
            miegoti = miegoti + 10;
        }
        Sleep(miegoti);
        m.unlock();
        //Kodo seka
        for (int i = 0; i < C.size(); i++)
        {
            if (state == C[i].getcs() && Tape[pos] == C[i].getCS())
            {
                Tape[pos] = C[i].getNS();
                if (C[i].getd() == 'R') pos++;
                else pos--;
                state = C[i].getns();
                I = 0;
            }
            else I++;
        }
    }
    COORD coord;
    coord.X = 0;
    coord.Y = 13;
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
}

///Pagrindine funkcija
int main()
{
    std::vector <Code> C; //Kodo vektorius
    std::string Tape;
    int miegoti;
    std::string x = "1"; //Kintamasis nusakantis pasirinkima "Meniu"
    while (x != "6")
    {
        Meniu(x);
        miegoti = 10;
        if (x < "5")
        {
            system("cls");
            Vykdymas(C, Tape, x, 1, 50);
            system("pause");
        }
        if (x == "5")
        {
            system("cls");
            std::thread t1(Vykdymas, std::ref(C), std::ref(Tape), "1", 1, std::ref(miegoti));
            std::thread t2(Vykdymas, std::ref(C), std::ref(Tape), "2", 4, std::ref(miegoti));
            std::thread t3(Vykdymas, std::ref(C), std::ref(Tape), "3", 7, std::ref(miegoti));
            std::thread t4(Vykdymas, std::ref(C), std::ref(Tape), "4", 10, std::ref(miegoti));
            t1.join();
            t2.join();
            t3.join();
            t4.join();
            system("pause");
        }
    }
}