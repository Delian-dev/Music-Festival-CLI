///Proiect2+Proiect3 - Dan Andrei-Delian: Artisti Muzicali

#include <iostream>
#include <cstring>
#include <cmath>
#include <string>
#include <regex>
#include <vector>
#include <unordered_map>
#include <set>
#include <list>
#include <random>
#include "SFML/Audio.hpp"
#include "SFML/Graphics.hpp"
#include <filesystem>
using namespace std;
namespace fs = std::filesystem;

class IOInterface { //interfata utilizata pentru functiile de citire/afisare ce vor fi rescrise in toate clasele din diamant pt a nu mai da overload la operatori
public:
    virtual ostream& afisare(ostream& out) const = 0;
    virtual istream& citire(istream& in) = 0;

    virtual ~IOInterface() = default;
};

class Instrument { //clasa de instrumente, va fi folosit la clasa Instrumentalist
private:
    string instrumentName;
    string instrumentType;
    long instrumentPrice;
    int instrumentAge; //vechime
public:
    Instrument();
    Instrument(string instrumentName, string instrumentType, long instrumentPrice, int instrumentAge);
    Instrument(const Instrument& obj);
    Instrument& operator=(const Instrument& obj);

    //setters and getters
    string getInstrumentName();
    string getInstrumentType();
    long getInstrumentPrice();
    int getInstrumentAge();

    void setInstrumentName(string instrumentName);
    void setInstrumentType(string instrumentType);
    void setInstrumentPrice(long instrumentPrice);
    void setInstrumentAge(int instrumentAge);

    //functionalitate
    void instrumentCurrentValue() const; //calculeaza valoarea actuala a instrumentlui, considerand pretul initial si vechimea
    bool operator<(const Instrument& obj) const;
    bool operator>(const Instrument& obj) const;

    //afisare/citire
    friend istream& operator>>(istream& in, Instrument& obj); //citire/afisare facute cu overload de operatori - doar pt aceasta clasa
    friend ostream& operator<<(ostream& out, const Instrument& obj);

    ~Instrument() = default;
};

Instrument::Instrument() {
    this->instrumentName = "Instrument name not known";
    this->instrumentType = "Instrument type not known";
    this->instrumentAge = 0;
    this->instrumentPrice = 0;
}

Instrument::Instrument(string instrumentName, string instrumentType, long instrumentPrice, int instrumentAge) {
    this->instrumentName = instrumentName;
    this->instrumentType = instrumentType;
    this->instrumentPrice = instrumentPrice;
    this->instrumentAge = instrumentAge;
}

Instrument::Instrument(const Instrument& obj) {
    this->instrumentName = obj.instrumentName;
    this->instrumentType = obj.instrumentType;
    this->instrumentPrice = obj.instrumentPrice;
    this->instrumentAge = obj.instrumentAge;
}

Instrument& Instrument::operator=(const Instrument& obj) {
    if (&obj != this) {
        this->instrumentName = obj.instrumentName;
        this->instrumentType = obj.instrumentType;
        this->instrumentPrice = obj.instrumentPrice;
        this->instrumentAge = obj.instrumentAge;
    }
    return *this;
}

string Instrument::getInstrumentName() { return this->instrumentName; }
string Instrument::getInstrumentType() { return this->instrumentType; }
long Instrument::getInstrumentPrice() { return this->instrumentPrice; }
int Instrument::getInstrumentAge() { return this->instrumentAge; }

void Instrument::setInstrumentName(string instrumentName) { this->instrumentName = instrumentName; }
void Instrument::setInstrumentType(string instrumentType) { this->instrumentType = instrumentType; }
void Instrument::setInstrumentPrice(long instrumentPrice) { this->instrumentPrice = instrumentPrice; }
void Instrument::setInstrumentAge(int instrumentAge) { this->instrumentAge = instrumentAge; }

void Instrument::instrumentCurrentValue() const {
    int value = this->instrumentPrice;
    if (this->instrumentAge < 5)
        value -= 0.1 * value;
    else if (this->instrumentAge <= 10)
        value += 0.2 * value;
    else
        value *= this->instrumentAge / 5;
    cout << "Valoarea curenta a instrumentului: " << this->instrumentName << " este de " << value << " dolari!\n";
}

bool Instrument::operator<(const Instrument& obj) const {
    if (this->instrumentPrice < obj.instrumentPrice)
        return true;
    if (this->instrumentPrice > obj.instrumentPrice)
        return false;
    //au acelasi pret - sortam dupa vechime
    if (this->instrumentAge < obj.instrumentAge)
        return true;
    return false;
}

bool Instrument::operator>(const Instrument& obj) const {
    if (this->instrumentPrice > obj.instrumentPrice)
        return true;
    if (this->instrumentPrice < obj.instrumentPrice)
        return false;
    //au acelasi pret - sortam dupa vechime
    if (this->instrumentAge > obj.instrumentAge)
        return true;
    return false;
}

istream& operator>>(istream& in, Instrument& obj) {
    cout << "Introduceti numele instrumentului: ";
    getline(in, obj.instrumentName);
    cout << "Introduceti tipul instrumentului: ";
    getline(in, obj.instrumentType);

    while (true) {
        cout << "Introduceti pretul instrumentului: ";
        string instrPrice;
        long price = 0;
        getline(in, instrPrice);
        try
        {
            for (char c : instrPrice) {
                if (!isdigit(c))
                    throw invalid_argument("Valoarea introdusa nu este una numerica!\n");
                price = price * 10 + (c - '0');
            }
            obj.instrumentPrice = price;
            break;
        }
        catch (const invalid_argument& e)
        {
            cout << e.what() << "Trebuie introdusa o valoare de tip LONG\n";
        }
    }

    while (true) {
        cout << "Introduceti vechimea instrumentului: ";
        string instrVechime;
        int vechime = 0;
        getline(in, instrVechime);
        try {
            for (char c : instrVechime) {
                if (!isdigit(c))
                    throw invalid_argument("Valoarea introdusa nu este una numerica!\n");
                vechime = vechime * 10 + (c - '0');
            }
            obj.instrumentAge = vechime;
            break;
        }
        catch (const invalid_argument& e) {
            cout << e.what() << "Trebuie introdusa o valoare de tip INT\n";
        }
    }
    return in;
}

ostream& operator<<(ostream& out, const Instrument& obj) {
    cout << "Numele instrumentului este: " << obj.instrumentName << '\n';
    cout << "Tipul instrumentului este: " << obj.instrumentType << '\n';
    cout << "Pretul instrumentului este de: " << obj.instrumentPrice << " lei\n";
    cout << "Vechimea instrumentului este de " << obj.instrumentAge << " ani\n";
    return out;
}

class Artist :public IOInterface { //clasa abstracta artist - varful diamantului
protected:
    string name;
    int age;
    string genre;
    string careerStart;
    string nationality;
    float monthlyListeners;
public:
    Artist();
    Artist(string name, int age, string genre, string careerStart, string nationality, float monthlyListeners);
    Artist(const Artist& obj);
    Artist& operator=(const Artist& obj);

    //setters and getters
    string getName();
    int getAge();
    string getGenre();
    float getMonthlyListeners();

    void setName(string name);
    void setAge(int age);
    void setGenre(string genre);
    void setMonthlyListeners(float monthlyListeners);

    //afisare,citire
    istream& citire(istream& in);
    ostream& afisare(ostream& out) const;

    friend istream& operator>>(istream& in, Artist& obj);
    friend ostream& operator<<(ostream& out, const Artist& obj);

    //functionalitate
    virtual long artistRevenue() = 0; //Artist este acum clasa abstracta
    int expectedTicketSales() const; //Numarul de bilete care se preconizeaza ca se vor vinde pt acest artist

    virtual bool lessThan(const Artist& obj) const;
    virtual bool greaterThan(const Artist& obj) const;

    bool operator<(const Artist& obj) const;
    bool operator>(const Artist& obj) const;

    ~Artist() = default;
};

Artist::Artist() {
    this->name = "Unknown name";
    this->age = 30;
    this->genre = "Unknown genre";
    this->careerStart = "Unknown date";
    this->nationality = "Unknown nationality";
    this->monthlyListeners = 0;
}

Artist::Artist(string name, int age, string genre, string careerStart, string nationality, float monthlyListeners) {
    this->name = name;
    this->age = age;
    this->genre = genre;
    this->careerStart = careerStart;
    this->nationality = nationality;
    this->monthlyListeners = monthlyListeners;
}

Artist::Artist(const Artist& obj) {
    this->name = obj.name;
    this->age = obj.age;
    this->genre = obj.genre;
    this->careerStart = obj.careerStart;
    this->nationality = obj.nationality;
    this->monthlyListeners = obj.monthlyListeners;
}

Artist& Artist::operator=(const Artist& obj) {
    if (&obj != this) {
        this->name = obj.name;
        this->age = obj.age;
        this->genre = obj.genre;
        this->careerStart = obj.careerStart;
        this->nationality = obj.nationality;
        this->monthlyListeners = obj.monthlyListeners;
    }
    return *this;
}

string Artist::getName() { return this->name; }
int Artist::getAge() { return this->age; }
string Artist::getGenre() { return this->genre; }
float Artist::getMonthlyListeners() { return this->monthlyListeners; }

void Artist::setName(string name) { this->name = name; }
void Artist::setAge(int age) { this->age = age; }
void Artist::setGenre(string genre) { this->genre = genre; }
void Artist::setMonthlyListeners(float monthlyListeners) { this->monthlyListeners = monthlyListeners; }

int Artist::expectedTicketSales() const {
    int tickets = 0;
    if (this->age >= 50)
        tickets = 3000;
    else if (this->age <= 20)
        tickets = 2000;
    else
        tickets = 5000;
    if (this->genre == "Rock" || this->genre == "Trap")
        tickets += 10000;
    else if (this->genre == "Pop")
        tickets += 20000;
    else
        tickets += 5000;
    tickets += this->monthlyListeners * 10000;
    return tickets;
}

bool Artist::lessThan(const Artist& obj) const {
    if (this->monthlyListeners < obj.monthlyListeners)
        return true;
    if (this->monthlyListeners > obj.monthlyListeners)
        return false;
    //tiebreaker - dupa varsta
    if (this->age < obj.age)
        return true;
    return false;
}

bool Artist::greaterThan(const Artist& obj) const {
    if (this->monthlyListeners > obj.monthlyListeners)
        return true;
    if (this->monthlyListeners < obj.monthlyListeners)
        return false;
    //tiebreaker - dupa varsta
    if (this->age > obj.age)
        return true;
    return false;
}

bool Artist::operator<(const Artist& obj) const {
    return this->lessThan(obj);
}

bool Artist::operator>(const Artist& obj) const {
    return this->greaterThan(obj);
}

istream& Artist::citire(istream& in) {
    cout << "Introduceti numele artistului: ";
    getline(in, this->name);

    while (true) {
        cout << "Introduceti varsta artistului: ";
        string artistAge;
        int varsta = 0;
        getline(in, artistAge);
        try {
            for (char c : artistAge) {
                if (!isdigit(c))
                    throw invalid_argument("Valoarea introdusa nu este una numerica!\n");
                varsta = varsta * 10 + (c - '0');
            }
            this->age = varsta;
            break;
        }
        catch (const invalid_argument& e) {
            cout << e.what() << "Trebuie introdusa o valoare de tip INT\n";
        }
    }

    cout << "Introduceti genul muzical al artistului: ";
    getline(in, this->genre);

    while (true) {
        cout << "Introduceti data la care artistul si-a inceput cariera (dd-mm-yyyy): ";
        string dataArtist;
        regex pattern("^(0[1-9]|[1-2][0-9]|3[0-1])-(0[1-9]|1[0-2])-(19|20)\\d{2}$");
        getline(in, dataArtist);
        try {
            if (!regex_match(dataArtist, pattern))
                throw logic_error("Valoarea introdusa nu reprezinta o data valida\n");
            this->careerStart = dataArtist;
            break;
        }
        catch (const logic_error& e) {
            cout << e.what() << "Data trebuie sa fie de forma dd-mm-yyyy\n";
        }
    }


    cout << "Introduceti nationalitatea artistului: ";
    in >> this->nationality;
    in.get();
    while (true) {
        cout << "Introduceti numarul de ascultatori lunari ai artistului (in milioane) :";
        string artistAsc;
        float ascultatori = 0;
        getline(in, artistAsc);
        try {
            bool virgula = false;
            int cnt_unit = 0, cnt_zecim = 0;
            for (char c : artistAsc) {
                if (!isdigit(c)) {
                    if (c != '.')
                        throw invalid_argument("Valorea introdusa nu este una numerica!\n");
                    else {
                        if (!virgula)
                            virgula = true;
                        else
                            throw invalid_argument("Valorea introdusa nu este una numerica!\n");
                    }
                }
                else {
                    if (virgula)
                        cnt_zecim++;
                    else
                        cnt_unit++;
                    ascultatori = ascultatori * 10 + (c - '0');
                }
            }
            this->monthlyListeners = ascultatori / pow(10, cnt_zecim);
            break;
        }
        catch (const invalid_argument& e) {
            cout << e.what() << "Trebuie introdusa o valoare de tip FLOAT\n";
        }
    }

    return in;
}

ostream& Artist::afisare(ostream& out) const {
    out << "Numele artistului este: " << this->name << '\n';
    out << "Varsta lui " << this->name << " este de " << this->age << " ani\n";
    out << "Genul muzical al lui " << this->name << " este: " << this->genre << '\n';
    out << this->name << " si-a inceput cariera la date de: " << this->careerStart << '\n';
    out << this->name << " este de nationalitate " << this->nationality << '\n';
    out << "Numarul de ascultatori lunari ai lui " << this->name << " este de " << this->monthlyListeners << " milioane\n";
    return out;
}

istream& operator>>(istream& in, Artist& obj) {
    return obj.citire(in);
}

ostream& operator<<(ostream& out, const Artist& obj) {
    return obj.afisare(out);
}

class Solist : public virtual Artist { //latura stanga a diamantului
protected:
    float vocalRange; //numarul de octave pe care le poate atinge
    string vocalStyle;
    bool vocalTraining;
    list <string> repertoriu;
public:
    Solist();
    Solist(string name, int age, string genre, string careerStart, string nationality,
        float monthlyListeners, float vocalRange, string vocalStyle,
        bool vocalTraining, list <string> repertoriu); //constructor cu toti parametrii - atat var proprii cat si mostenite din Artist
    Solist(const Solist& obj);
    Solist& operator=(const Solist& obj);

    //setters and getters
    bool getVocalTraining();
    const list <string> getRepertoriu();
    float getVocalRange();

    void setVocalTraining(bool vocalTraining);
    void setRepertoriu(list <string> repertoriu);

    //afisare,citire
    istream& citire(istream& in);
    ostream& afisare(ostream& out) const;

    //operatori
    Solist& operator++(); //pre-incrementare
    Solist operator++(int); //post-incrementare
    Solist operator+(float addOctave);
    friend Solist operator+(float addOctave, Solist obj); //adunarea dintre float si Solist la stanga si la dreapta
    string operator[](int index);

    //functionalitate
    long artistRevenue(); //functie virtuala pasata de la Artist
    int expectedTicketSales() const; //functie din Artist care isi ia overwrite
    void updateRepertoriu(string melodie);

    ~Solist() = default;
};

Solist::Solist() :Artist() {
    this->vocalRange = 1;
    this->vocalStyle = "Unknown vocal style";
    this->vocalTraining = false;
    this->repertoriu = {};
}

Solist::Solist(string name, int age, string genre, string careerStart, string nationality,
    float monthlyListeners, float vocalRange, string vocalStyle, bool vocalTraining,
    list <string> repertoriu) :Artist(name, age, genre, careerStart, nationality, monthlyListeners) {
    this->vocalRange = vocalRange;
    this->vocalStyle = vocalStyle;
    this->vocalTraining = vocalTraining;
    this->repertoriu = repertoriu;
}

Solist::Solist(const Solist& obj) :Artist(obj) {
    this->vocalRange = obj.vocalRange;
    this->vocalStyle = obj.vocalStyle;
    this->vocalTraining = obj.vocalTraining;
    this->repertoriu = obj.repertoriu;
}

Solist& Solist::operator=(const Solist& obj) {
    if (&obj != this) {
        Artist::operator=(obj);
        this->vocalRange = obj.vocalRange;
        this->vocalStyle = obj.vocalStyle;
        this->vocalTraining = obj.vocalTraining;
        this->repertoriu = obj.repertoriu;
    }
    return *this;
}

bool Solist::getVocalTraining() { return this->vocalTraining; }
const list <string> Solist::getRepertoriu() { return this->repertoriu; }
float Solist::getVocalRange() { return this->vocalRange; }

void Solist::setVocalTraining(bool vocalTraining) { this->vocalTraining = vocalTraining; }
void Solist::setRepertoriu(list <string> repertoriu) { this->repertoriu = repertoriu; }

Solist& Solist::operator++() {
    this->vocalRange++;
    if (!this->vocalTraining)
        this->vocalTraining = true;
    cout << "Introduceti numele melodiei ce va fi adaugata in repertoriu: ";
    string melodie;
    getline(cin, melodie);
    this->repertoriu.push_back(melodie);
    return *this;
}

Solist Solist::operator++(int) {
    Solist aux = *this;
    ++(*this);
    return aux;
}

Solist Solist::operator+(float addOctave) {
    Solist solistUpdated = *this;
    solistUpdated.vocalRange += addOctave;
    return solistUpdated;
}

Solist operator+(float addOctave, Solist obj) {
    obj.vocalRange += addOctave;
    return obj;
}

// string Solist::operator[](int index){
//     if(index>=0 && index<this->repertoriu.size())
//         return this->repertoriu[index];
//     throw runtime_error("Index invalid!");
// }

long Solist::artistRevenue() {
    //cout<<"Solist revenue\n";
    long revenue = 0;
    revenue = 10000 * this->monthlyListeners; //100 listeners = 1 dollar
    revenue *= 1.5 * this->vocalRange;
    if (this->vocalStyle == "Tenor" || this->vocalStyle == "Soprano")
        revenue += 50000;
    if (this->vocalTraining)
        revenue *= 2;
    revenue += 1000000 * this->repertoriu.size();
    return revenue;
}

int Solist::expectedTicketSales() const {
    //cout<<"Tickets solist\n";
    int rez = this->Artist::expectedTicketSales();
    rez *= vocalRange / 2;
    rez += 5000 * this->repertoriu.size();
    return rez;
}

void Solist::updateRepertoriu(string melodie) {
    this->repertoriu.push_back(melodie);
    cout << "Melodie adaugata cu succes!\n";
}


istream& Solist::citire(istream& in) {
    this->Artist::citire(in);

    while (true) {
        cout << "Introduceti numarul de octave pe care le poate atinge " << this->getName() << ": ";
        string octvArtist;
        float octave = 0;
        getline(in, octvArtist);
        try {
            bool virgula = false;
            int cnt_zecim = 0;
            for (char c : octvArtist) {
                if (!isdigit(c)) {
                    if (c != '.')
                        throw invalid_argument("Valorea introdusa nu este una numerica!\n");
                    else {
                        if (!virgula)
                            virgula = true;
                        else
                            throw invalid_argument("Valorea introdusa nu este una numerica!\n");
                    }
                }
                else {
                    if (virgula)
                        cnt_zecim++;
                    octave = octave * 10 + (c - '0');
                }
            }
            this->vocalRange = octave;
            break;
        }
        catch (const invalid_argument& e) {
            cout << e.what() << "Trebuie introdusa o valore de tip FLOAT\n";
        }
    }
    cout << "Introduceti stilul vocal al artistului: ";
    in >> this->vocalStyle;
    in.get();

    while (true) {
        cout << "Precizati daca solistul are pregatire vocala profesionista (0-nu,1-da): ";
        string training;
        getline(in, training);
        try {
            if (training != "0" && training != "1")
                throw invalid_argument("Valoarea introdusa nu este adecvata!\n");
            if (training == "0")
                this->vocalTraining = 0;
            else
                this->vocalTraining = 1;
            break;
        }
        catch (const invalid_argument& e) {
            cout << e.what() << "Trebuie introdusa una din valorile 0/1\n";
        }
    }

    int comanda, k = 1;
    while (k == 1) {
        cout << "Apasa 1 pentru a adauga o melodie in repertoriul artistului: \n";
        cout << "Apasa 2 pentru a iesi din meniu: \n";

        string testComanda;
        getline(in, testComanda);
        try {
            if (testComanda != "1" && testComanda != "2")
                throw invalid_argument("Eroare.");

            if (testComanda == "1")
                comanda = 1;
            else
                comanda = 2;
            switch (comanda)
            {
            case 1:
            {
                string melodie;
                cout << "Introduceti numele melodiei: ";
                getline(in, melodie);
                this->repertoriu.push_back(melodie);
                break;
            }

            case 2:
            {
                k = 2;
                break;
            }
            }
        }
        catch (const invalid_argument& e) {
            cout << "Comanda nerecunoscuta, incearca din nou!\n";
        }
    }
    return in;
}

ostream& Solist::afisare(ostream& out) const {
    this->Artist::afisare(out);
    out << "Solistul poate atinge " << this->vocalRange << " octave\n";
    out << "Stilul vocal al solistului este: " << this->vocalStyle << '\n';
    if (this->vocalTraining)
        out << "Solistul are antrenament vocal profesional\n";
    else
        out << "Solistul nu are antrenament vocal profesional\n";
    out << "Melodiile din repertoriul Solistului sunt:\n";
    // for(int i=0; i<this->repertoriu.size(); i++)
    //     out<<"Melodia "<<i+1<<": "<<this->repertoriu[i]<<'\n';
    int i = 1;
    for (const auto& elem : this->repertoriu) {
        out << "Melodia " << i << ": " << elem << '\n';
        i++;
    }
    return out;
}


class Instrumentalist : public virtual Artist { //latura din dreapta a diamantului
protected:
    vector <Instrument> playedInstruments; //vector de elemente tip Instrument
    int yearsOfPractice;
    bool selfTaught;
    int skillLevel; //number from 1-10
public:
    Instrumentalist();
    Instrumentalist(string name, int age, string genre, string careerStart, string nationality,
        float monthlyListeners, vector <Instrument> playedInstruments, int yearsOfPractice,
        bool selfTaught, int skillLevel); //variabile mostenite din Artist+Proprii
    Instrumentalist(const Instrumentalist& obj);
    Instrumentalist& operator=(const Instrumentalist& obj);

    //setters and getters
    const vector <Instrument> getPlayedInstruments();
    int getSkillLevel();

    void setPlayedInstruments(vector <Instrument> playedInstruments);
    void setSkillLevel(int skillLevel);

    //operatori
    Instrumentalist& operator++();
    Instrumentalist operator++(int);
    Instrument operator[](int index);

    //afisare,citire
    istream& citire(istream& in);
    ostream& afisare(ostream& out) const;

    //functionalitate
    long artistRevenue(); //functia virtuala mostenita din Artist. Virtualizarea permite compilatorului sa diferentieze intre clasele ce au aceasta functie
    void updateInstruments();

    ~Instrumentalist() = default;
};

Instrumentalist::Instrumentalist() :Artist() {
    this->playedInstruments = {};
    this->yearsOfPractice = 0;
    this->selfTaught = false;
    this->skillLevel = 1;
}

Instrumentalist::Instrumentalist(string name, int age, string genre, string careerStart,
    string nationality, float monthlyListeners, vector <Instrument> playedInstruments,
    int yearsOfPractice, bool selfTaught, int skillLevel) :Artist(name, age, genre, careerStart, nationality, monthlyListeners) {
    this->playedInstruments = playedInstruments;
    this->yearsOfPractice = yearsOfPractice;
    this->selfTaught = selfTaught;
    this->skillLevel = skillLevel;
}

Instrumentalist::Instrumentalist(const Instrumentalist& obj) :Artist(obj) {
    this->playedInstruments = obj.playedInstruments;
    this->yearsOfPractice = obj.yearsOfPractice;
    this->selfTaught = obj.selfTaught;
    this->skillLevel = obj.skillLevel;
}

Instrumentalist& Instrumentalist::operator=(const Instrumentalist& obj) {
    if (&obj != this) {
        Artist::operator=(obj);
        this->playedInstruments = obj.playedInstruments;
        this->yearsOfPractice = obj.yearsOfPractice;
        this->selfTaught = obj.selfTaught;
        this->skillLevel = obj.skillLevel;
    }
    return *this;
}

const vector <Instrument> Instrumentalist::getPlayedInstruments() { return this->playedInstruments; }
int Instrumentalist::getSkillLevel() { return this->skillLevel; }

void Instrumentalist::setPlayedInstruments(vector <Instrument> playedInstruments) { this->playedInstruments = playedInstruments; }
void Instrumentalist::setSkillLevel(int skillLevel) { this->skillLevel = skillLevel; }

Instrumentalist& Instrumentalist::operator++() {
    cout << "Introduceti datele despre instrumentul ce va fi adaugat in repertoriul lui " << this->name << ": \n";
    Instrument instrument;
    cin >> instrument;
    this->playedInstruments.push_back(instrument);

    this->yearsOfPractice++;
    if (this->skillLevel < 10)
        this->skillLevel++;
    return *this;
}

Instrumentalist Instrumentalist::operator++(int) {
    Instrumentalist aux = *this;
    ++(*this);
    return aux;
}

Instrument Instrumentalist::operator[](int index) {
    if (index >= 0 && index < this->playedInstruments.size())
        return this->playedInstruments[index];
    throw runtime_error("Index invalid!");
}

long Instrumentalist::artistRevenue() {
    //cout<<"Instrumentalist revenue\n";
    long revenue = 0;
    revenue = 1000 * this->monthlyListeners; //instrumentalistii castiga de 10 ori mai putin decat un solist din ascultatori
    for (int i = 0; i < this->playedInstruments.size(); i++) {
        revenue += (i + 1) * 50000;
        revenue -= this->playedInstruments[i].getInstrumentPrice();
    }
    revenue *= this->yearsOfPractice;
    if (this->selfTaught)
        revenue += 100000;
    else
        revenue -= 20000;
    revenue += 100000 * this->skillLevel;
    return revenue;
}

void Instrumentalist::updateInstruments() {
    int k = 1;
    while (k == 1) {
        Instrument instrument;
        cout << "Precizati datele instrumentului pe care vreti sa-l adaugati:\n";
        cin >> instrument;
        this->playedInstruments.push_back(instrument);

        while (true) {
            cout << "Doriti sa mai adaugati un instrument? (1-Da, 2-Nu):\n";
            int comanda;
            string testComanda;
            getline(cin, testComanda);
            try {
                if (testComanda != "1" && testComanda != "2")
                    throw invalid_argument("Eroare.");
                if (testComanda == "1")
                    comanda = 1;
                else
                    comanda = 2;
                if (comanda == 2) {
                    k = 2;
                    cout << "Iesire din meniu...\n";
                }
                break;
            }
            catch (const invalid_argument& e) {
                k = 2;
                cout << "Comanda nerecunoscuta. Iesire din meniu...\n";
                cout << '\n';
                break;
            }
        }
    }
}


istream& Instrumentalist::citire(istream& in) {
    this->Artist::citire(in);

    int comanda, k = 1;
    while (k == 1) {
        cout << "Apasa 1 pentru a adauga un instrument in repertoriul artistului: \n";
        cout << "Apasa 2 pentru a iesi din meniu: \n";
        in >> comanda;
        switch (comanda)
        {
        case 1:
        {
            Instrument instrument;
            cout << "Introduceti datele instrumentlui: \n";
            in.get();
            in >> instrument;
            this->playedInstruments.push_back(instrument);
            break;
        }

        case 2:
        {
            k = 2;
            break;
        }

        default:
        {
            cout << "Comanda nerecunoscuta, incearca din nou!\n";
            break;
        }
        }
    }

    while (true) {
        cout << "Introduceti numarul de ani de antrenament in materie de instrumente: ";
        string artistPractice;
        int practice = 0;
        in.get();
        getline(in, artistPractice);
        try {
            for (char c : artistPractice) {
                if (!isdigit(c))
                    throw invalid_argument("Valoarea introdusa nu este una numerica!\n");
                practice = practice * 10 + (c - '0');
            }
            this->yearsOfPractice = practice;
            break;
        }
        catch (const invalid_argument& e) {
            cout << e.what() << "Trebuie introdusa o valoare de tip INT\n";
        }
    }

    while (true) {
        cout << "Precizati daca " << this->getName() << " a invatat de unul singur sa cante la instrumente (0-nu,1-da): ";
        string taught;
        getline(in, taught);
        try {
            if (taught != "0" && taught != "1")
                throw invalid_argument("Valoarea introdusa nu este adecvata!\n");
            if (taught == "0")
                this->selfTaught = 0;
            else
                this->selfTaught = 1;
            break;
        }
        catch (const invalid_argument& e) {
            cout << e.what() << "Trebuie introdusa una din valorile 0/1\n";
        }
    }

    while (true) {
        cout << "Nivelul de pricepere a lui " << this->getName() << " (pe o scara la 1-10): ";
        string artistSkill;
        int skill = 0;
        getline(in, artistSkill);
        try {
            for (char c : artistSkill) {
                if (!isdigit(c))
                    throw invalid_argument("Valoarea introdusa nu este una numerica!\n");
                skill = skill * 10 + (c - '0');
            }
            try {
                if (skill > 10 || skill == 0)
                    throw logic_error("Numarul introdus nu respecta criteriile!\n");
                this->skillLevel = skill;
                break;
            }
            catch (const logic_error& e) {
                cout << e.what() << "Trebuie introdusa o valoare intreaga cuprinsa intre 1-10\n";
            }
        }
        catch (const invalid_argument& e) {
            cout << e.what() << "Trebuie introdusa o valoare de tip INT\n";
        }
    }

    return in;
}

ostream& Instrumentalist::afisare(ostream& out) const {
    this->Artist::afisare(out);
    out << "Instrumentele la care artistul poate canta sunt:\n";
    out << '\n';
    for (int i = 0; i < this->playedInstruments.size(); i++)
        out << "Instrumentul " << i + 1 << ": " << this->playedInstruments[i] << '\n';

    out << "Artistul canta la instrumente de " << this->yearsOfPractice << " ani\n";
    if (this->selfTaught)
        out << "Artistul a invatat de unul singur sa cante la instrumente\n";
    else
        out << "Artistul nu a invatat de unul singur sa cante la instrumente\n";
    out << "Pe o scara 1-10, nivelul de pricepere a artistului in materie de instrumente este: " << this->skillLevel << '\n';
    return out;
}

class OneManBand :public Solist, public Instrumentalist { //clasa de jos din Diamant
private:
    bool signedToLabel;
    vector <string> loopingEquipment; //echipament pre-programat pt a permite artistului sa faca totul de unul singur
    int stagePresence; //numar de la 1-10
public:
    OneManBand();
    OneManBand(string name, int age, string genre, string careerStart, string nationality,
        float monthlyListeners, float vocalRange, string vocalStyle, bool vocalTraining,
        list <string> repertoriu, vector <Instrument> playedInstruments,
        int yearsOfPractice, bool selfTaught, int skillLevel,
        bool signedToLabel, vector <string> loopingEquipment, int stagePresence); //toate variabilele din clasele ce formeaza diamantul

    OneManBand(const OneManBand& obj);
    OneManBand& operator=(const OneManBand& obj);

    //setteri,getteri
    const vector <string> getLoopingEquipment();
    int getStagePresence();

    void setLoopingEquipment(vector <string> loopingEquipment);
    void setStagePresence(int stagePresence);

    //citire,afisare
    istream& citire(istream& in);
    ostream& afisare(ostream& out) const;

    //operatori
    OneManBand& operator++();
    OneManBand operator++(int);
    OneManBand operator+(float addOctave);
    friend OneManBand operator+(float addOctave, OneManBand obj);
    string operator[](int index);

    //functionalitate
    long artistRevenue(); //functia mostenita virtual
    int expectedTicketSales() const;
    void recalculateStagePresence();

    ~OneManBand() = default;
};

OneManBand::OneManBand() :Artist(), Solist(), Instrumentalist() { //mosteniri virtuale => compilatorul va sti ce variabile sa initializeze in functie de constructorul corespunzator
    this->signedToLabel = false;
    this->loopingEquipment = {};
    this->stagePresence = 1;
}

OneManBand::OneManBand(string name, int age, string genre, string careerStart, string nationality,  //clase mostenite virtual -> compilatorul va sti de la care clasa sa preia variabilele 
    float monthlyListeners, float vocalRange, string vocalStyle, bool vocalTraining,  //chiar daca variabila "Genre" este in toti cei 3 constructori, va fi intializata doar in constructorul de la Artist
    list <string> repertoriu, vector <Instrument> playedInstruments,
    int yearsOfPractice, bool selfTaught, int skillLevel,
    bool signedToLabel, vector <string> loopingEquipment, int stagePresence
) :Artist(name, age, genre, careerStart, nationality, monthlyListeners),
Solist(name, age, genre, careerStart, nationality, monthlyListeners, vocalRange, vocalStyle, vocalTraining, repertoriu),
Instrumentalist(name, age, genre, careerStart, nationality, monthlyListeners, playedInstruments, yearsOfPractice, selfTaught, skillLevel)
{
    this->signedToLabel = signedToLabel;
    this->loopingEquipment = loopingEquipment;
    this->stagePresence = stagePresence;

}

OneManBand::OneManBand(const OneManBand& obj) :Artist(obj), Solist(obj), Instrumentalist(obj) {
    this->signedToLabel = obj.signedToLabel;
    this->loopingEquipment = obj.loopingEquipment;
    this->stagePresence = obj.stagePresence;
}

OneManBand& OneManBand::operator=(const OneManBand& obj) {
    if (&obj != this) {
        //Artist::operator=(obj);
        Solist::operator=(obj);
        //Instrumentalist::operator=(obj);
        this->playedInstruments = obj.playedInstruments;
        this->yearsOfPractice = obj.yearsOfPractice;
        this->selfTaught = obj.selfTaught;
        this->skillLevel = obj.skillLevel;
        this->signedToLabel = obj.signedToLabel;
        this->loopingEquipment = obj.loopingEquipment;
        this->stagePresence = obj.stagePresence;
    }
    return *this;
}

const vector <string> OneManBand::getLoopingEquipment() { return this->loopingEquipment; }
int OneManBand::getStagePresence() { return this->stagePresence; }

void OneManBand::setLoopingEquipment(vector <string> loopingEquipment) { this->loopingEquipment = loopingEquipment; }
void OneManBand::setStagePresence(int stagePresence) { this->stagePresence = stagePresence; }

OneManBand& OneManBand::operator++() {
    this->Solist::operator++();
    this->Instrumentalist::operator++(); //incrementam si variabilele ce apartin claselor de deasupra
    //cin.get();
    cout << "Introduceti numele device-ului ce va fi adaugat in echipamentele lui " << this->name << ": ";
    string device;
    getline(cin, device);
    this->loopingEquipment.push_back(device);
    this->stagePresence++;
    return *this;
}

OneManBand OneManBand::operator++(int) {
    OneManBand aux = *this;
    ++(*this);
    return aux;
}

OneManBand OneManBand::operator+(float addOctave) {
    OneManBand varUpdated = *this;
    varUpdated.vocalRange += addOctave;
    return varUpdated;
}

OneManBand operator+(float addOctave, OneManBand obj) {
    obj.vocalRange += addOctave;
    return obj;
}

string OneManBand::operator[](int index) {
    if (index >= 0 && index < this->loopingEquipment.size())
        return this->loopingEquipment[index];
    throw runtime_error("Index invalid!");
}

long OneManBand::artistRevenue() {
    //cout<<"OneManBand revenue\n";
    long revenue = 0;
    revenue += this->Solist::artistRevenue();
    revenue += this->Instrumentalist::artistRevenue();
    if (this->signedToLabel)
        revenue /= 10;
    else
        revenue *= 10;
    for (int i = 0; i < this->loopingEquipment.size(); i++)
        revenue += (i + 1) * 15000;
    if (this->skillLevel < 5)
        revenue -= 50000;
    else
        revenue += 50000 * this->skillLevel;
    return revenue;
}

int OneManBand::expectedTicketSales() const { //overwrite la functia din Solist/Artist
    //cout<<"OneManBand tickets\n";
    int rez = this->Solist::expectedTicketSales(); //completam in functie de rezultatul din Solist
    if (this->signedToLabel)
        rez += 25000;
    rez += 7000 * this->stagePresence;
    return rez;
}

void OneManBand::recalculateStagePresence() {
    int current_stagePresence = this->stagePresence;
    if (this->vocalRange >= 5) {
        current_stagePresence = min(current_stagePresence + 2, 10);
    }
    else {
        if (this->vocalRange >= 2.5)
            current_stagePresence = min(current_stagePresence + 1, 10);
    }
    if (this->repertoriu.size() > 6 || this->playedInstruments.size() > 3)
        current_stagePresence = min(current_stagePresence + 1, 10);
    current_stagePresence = min(current_stagePresence + this->yearsOfPractice / 3, 10);
    if (this->skillLevel >= 5)
        current_stagePresence = min(current_stagePresence + this->skillLevel / 2, 10);

    this->stagePresence = current_stagePresence;
    //cout<<"Nivelul de spectacol al artistului "<<this->name<<" a fost recalculat cu succes!\n";
}


istream& OneManBand::citire(istream& in) {
    this->Solist::citire(in);

    int comanda, k = 1;
    while (k == 1) {
        cout << "Apasa 1 pentru a adauga un instrument in repertoriul artistului: \n";
        cout << "Apasa 2 pentru a iesi din meniu: \n";
        in >> comanda;
        switch (comanda)
        {
        case 1:
        {
            Instrument instrument;
            cout << "Introduceti datele instrumentlui: \n";
            in.get();
            in >> instrument;
            this->playedInstruments.push_back(instrument);
            break;
        }

        case 2:
        {
            k = 2;
            break;
        }

        default:
        {
            cout << "Comanda nerecunoscuta, incearca din nou!\n";
            break;
        }
        }
    }

    while (true) {
        cout << "Introduceti numarul de ani de antrenament in materie de instrumente: ";
        string artistPractice;
        int practice = 0;
        in.get();
        getline(in, artistPractice);
        try {
            for (char c : artistPractice) {
                if (!isdigit(c))
                    throw invalid_argument("Valoarea introdusa nu este una numerica!\n");
                practice = practice * 10 + (c - '0');
            }
            this->yearsOfPractice = practice;
            break;
        }
        catch (const invalid_argument& e) {
            cout << e.what() << "Trebuie introdusa o valoare de tip INT\n";
        }
    }

    while (true) {
        cout << "Precizati daca " << this->getName() << " a invatat de unul singur sa cante la instrumente (0-nu,1-da): ";
        string taught;
        getline(in, taught);
        try {
            if (taught != "0" && taught != "1")
                throw invalid_argument("Valoarea introdusa nu este adecvata!\n");
            if (taught == "0")
                this->selfTaught = 0;
            else
                this->selfTaught = 1;
            break;
        }
        catch (const invalid_argument& e) {
            cout << e.what() << "Trebuie introdusa una din valorile 0/1\n";
        }
    }

    while (true) {
        cout << "Nivelul de pricepere a lui " << this->getName() << " (pe o scara la 1-10): ";
        string artistSkill;
        int skill = 0;
        getline(in, artistSkill);
        try {
            for (char c : artistSkill) {
                if (!isdigit(c))
                    throw invalid_argument("Valoarea introdusa nu este una numerica!\n");
                skill = skill * 10 + (c - '0');
            }
            try {
                if (skill > 10 || skill == 0)
                    throw logic_error("Numarul introdus nu respecta criteriile!\n");
                this->skillLevel = skill;
                break;
            }
            catch (const logic_error& e) {
                cout << e.what() << "Trebuie introdusa o valoare intreaga cuprinsa intre 1-10\n";
            }
        }
        catch (const invalid_argument& e) {
            cout << e.what() << "Trebuie introdusa o valoare de tip INT\n";
        }
    }

    while (true) {
        cout << "Precizati daca " << this->getName() << " este semnat la o casa de discuri (0-nu,1-da): ";
        string isSigned;
        getline(in, isSigned);
        try {
            if (isSigned != "0" && isSigned != "1")
                throw invalid_argument("Valoarea introdusa nu este adecvata!\n");
            if (isSigned == "0")
                this->signedToLabel = 0;
            else
                this->signedToLabel = 1;
            break;
        }
        catch (const invalid_argument& e) {
            cout << e.what() << "Trebuie introdusa una din valorile 0/1\n";
        }
    }

    int comanda2, k2 = 1;
    while (k2 == 1) {
        cout << "Apasa 1 pentru a adauga un device ajutator in repertoriul artistului: \n";
        cout << "Apasa 2 pentru a iesi din meniu: \n";
        in >> comanda2;
        in.get();
        switch (comanda2)
        {
        case 1:
        {
            string device;
            cout << "Introduceti numele device-ului: ";
            getline(in, device);
            this->loopingEquipment.push_back(device);
            break;
        }

        case 2:
        {
            k2 = 2;
            break;
        }

        default:
        {
            cout << "Comanda nerecunoscuta, incearca din nou!\n";
            break;
        }
        }
    }

    while (true) {
        cout << "Precizati pe o scara 1-10 cat de spectaculos este artistul pe scena: ";
        string artistSpectacle;
        int spectacle = 0;
        getline(in, artistSpectacle);
        try {
            for (char c : artistSpectacle) {
                if (!isdigit(c))
                    throw invalid_argument("Valoarea introdusa nu este una numerica!\n");
                spectacle = spectacle * 10 + (c - '0');
            }
            try {
                if (spectacle > 10 || spectacle == 0)
                    throw logic_error("Numarul introdus nu respecta criteriile!\n");
                this->stagePresence = spectacle;
                break;
            }
            catch (const logic_error& e) {
                cout << e.what() << "Trebuie introdusa o valoarea intreaga cuprinsa intre 1-10\n";
            }
        }
        catch (const invalid_argument& e) {
            cout << e.what() << "Trebuie introdusa o valoare de tip INT\n";
        }
    }

    return in;
}

ostream& OneManBand::afisare(ostream& out) const {
    this->Solist::afisare(out);

    out << "Instrumentele la care artistul poate canta sunt:\n";
    for (int i = 0; i < this->playedInstruments.size(); i++)
        out << "Instrumentul " << i + 1 << ": " << this->playedInstruments[i] << '\n';

    out << "Artistul canta la instrumente de " << this->yearsOfPractice << " ani\n";
    if (this->selfTaught)
        out << "Artistul a invatat de unul singur sa cante la instrumente\n";
    else
        out << "Artistul nu a invatat de unul singur sa cante la instrumente\n";
    out << "Pe o scara 1-10, nivelul de pricepere a artistului in materie de instrumente este: " << this->skillLevel << '\n';

    if (this->signedToLabel)
        out << "Artistul este semnat la o casa de discuri\n";
    else
        out << "Artistul nu este semnat la o casa de discuri\n";

    out << "Echipamentul ajutator folosit de artist este:\n";
    for (int i = 0; i < this->loopingEquipment.size(); i++)
        out << "Device " << i + 1 << ": " << this->loopingEquipment[i] << '\n';

    out << "Pe o scara 1-10, artistul produce un spectacol de nota " << this->stagePresence << '\n';

    return out;
}

vector<string> salvareFisiereAudio(const string& numeFolder) {
    vector<string> audioFiles;
    for (const auto& fisier : fs::directory_iterator(numeFolder)) {
        if (fisier.path().extension() == ".mp3")
            audioFiles.push_back(fisier.path().string());
    }
    return audioFiles;
}

string chooseAudioFile(const vector<string>& audioFiles) {
    cout << "Fisierele audio pe care le avem la dispozitie:\n";
    for (int i = 0; i < audioFiles.size(); i++)
        cout << "Audio " << i + 1 << ": " << audioFiles[i] << '\n';
    
    int comanda = 0;
    while (true) {
        cout << "Alegeti fisierul prin introducerea numarului corespunzator: ";
        string strComanda;
        try {
            getline(cin, strComanda);
            for (char c : strComanda) {
                if (!isdigit(c))
                    throw invalid_argument("Valoarea introdusa nu este una numerica!\n");
                comanda = comanda * 10 + (c - '0');
            }
            break;
        }
        catch (const invalid_argument& e) {
            cout << e.what() << "Trebuie introdusa o valoare de tip INT\n";
        }
    }

    if (comanda > 0 && comanda <= audioFiles.size())
        return audioFiles[comanda - 1];
    else {
        cout << "Comanda nu esta valabila, se va alege default prima melodie din lista\n";
        return audioFiles[0];
    }
}

//METODE TEMPLATE
template <class T>
void sortDataAscending(vector<T>& arr) {
    int size = arr.size();
    for (int i = 0; i < size - 1; i++)
        for (int j = i + 1; j < size; j++)
            if (arr[i] > arr[j])
                swap(arr[i], arr[j]);
}

template <class T>
void sortDataDescending(vector<T>& arr) {
    int size = arr.size();
    for (int i = 0; i < size - 1; i++)
        for (int j = i + 1; j < size; j++)
            if (arr[i] < arr[j])
                swap(arr[i], arr[j]);
}

template<class T>
void shuffleArray(vector<T>& arr) {
    random_device rd;
    mt19937 g(rd());
    shuffle(arr.begin(), arr.end(), g);
}

//CLASA TEMPLATE
template <class T>
class CustomCollection {
private:
    vector<T*> colectie;
    unordered_map<string, sf::SoundBuffer> audioMap; //MAP pentru fisierele tip audio si buffer
    sf::Sound audio;
public:
    CustomCollection() = default;

    void addElement(T* elem);

    int collectionSize();

    void sortAscending();

    void sortDescending();

    void shuffleCollection();

    T* operator[](int index);

    T* collectionBack();

    void eraseFromCollection(int index, const string& objName);

    void printData();

    void associateSound(const string& objName, const string& fileName);

    void playSound(const string& objName);

    void stopSound(const string& objName);

    ~CustomCollection() = default;
};

template<class T>
void CustomCollection<T>::addElement(T* elem) {
    this->colectie.push_back(elem);
}

template<class T>
int CustomCollection<T>::collectionSize() {
    return this->colectie.size();
}

template<class T>
void CustomCollection<T>::sortAscending() {
    sortDataAscending(this->colectie);
}

template<class T>
void CustomCollection<T>::sortDescending() {
    sortDataDescending(this->colectie);
}

template<class T>
void CustomCollection<T>::shuffleCollection(){
    shuffleArray(this->colectie);
}

template<class T>
T* CustomCollection<T>::operator[](int index) {
    if (index >= this->colectie.size())
        throw out_of_range("Index out of range");
    return this->colectie[index];
}

template<class T>
T* CustomCollection<T>::collectionBack() {
    if (this->colectie.empty()) {
        throw out_of_range("Collection is empty");
    }
    return this->colectie.back();
}

template<class T>
void CustomCollection<T>::eraseFromCollection(int index, const string& objName) {
    if (index >= this->colectie.size()) {
        throw out_of_range("Index out of range");
    }
    delete this->colectie[index];
    this->colectie.erase(this->colectie.begin() + index);
    this->audioMap.erase(objName);
}

template<class T>
void CustomCollection<T>::printData() {
    for (int i = 0; i < this->colectie.size(); i++) {
        cout << "Element " << i + 1 << ":\n";
        cout << this->colectie[i] << "\n\n";
    }
}

template<class T>
void CustomCollection<T>::associateSound(const string& objName, const string& fileName) {
    sf::SoundBuffer buffer;
    if (!buffer.loadFromFile(fileName)) {
        cerr << "Error loading sound file: " << fileName << '\n';
        return;
    }
    this->audioMap[objName] = buffer;
}

template<class T>
void CustomCollection<T>::playSound(const string& objName) {
    if (this->audioMap.find(objName) != this->audioMap.end()) {
        this->audio.setBuffer(this->audioMap[objName]);
        this->audio.play();
    }
    else
        cerr << "Nu am gasit un sunet asociat cu elementul  " << objName << '\n';
}

template<class T>
void CustomCollection<T>::stopSound(const string& objName) {
    auto it = this->audioMap.find(objName);
    if (it != this->audioMap.end()) {

        this->audio.stop();
    }
    else {
        cerr << "Nu am gasit un sunet asociat cu elementul " << objName << '\n';
    }
}

class Singleton {
private:
    static Singleton* instance;
    string tested_menu;

    Singleton()=default;
    Singleton(const Singleton& obj) = delete;

public:
    static Singleton* getInstance();
    int meniu();
    string getTestedMenu();
    void setTestedMenu(string tested_menu);
    ~Singleton() = default;
};

Singleton* Singleton::instance = NULL;

int Singleton::meniu() {
    tested_menu = "started_menu";

    vector<string> audioFiles = salvareFisiereAudio("tracks");
    /*for (const auto& value : audioFiles) {
        cout << value << '\n';
    }*/

    CustomCollection<Artist> artisti;
    //Initializam vectorul cu valori default din fiecare clasa pentru a testa mai usor meniul
    artisti.addElement(new Solist);
    artisti.addElement(new Instrumentalist);
    artisti.addElement(new OneManBand);

    artisti[0]->setName("Solist 1"), artisti[0]->setGenre("Rap"), artisti[0]->setMonthlyListeners(12.5);
    artisti[1]->setName("Instrumentalist 1"), artisti[1]->setGenre("Jazz"), artisti[1]->setMonthlyListeners(7.6);
    artisti[2]->setName("OneManBand 1"), artisti[2]->setGenre("Rock"), artisti[2]->setMonthlyListeners(18.5);


    Solist* solistptr = dynamic_cast<Solist*>(artisti[0]);
    solistptr->setRepertoriu({ "Aviatia","Chitara mea","O mie de pahare" });

    Instrument i1("Chitara Acustica", "Corzi", 1500, 5);
    Instrument i2("Tobe", "Percutie", 2250, 7);
    Instrument i3("Keyboard", "Clape", 500, 3);

    Instrumentalist* instrumentalistptr = dynamic_cast<Instrumentalist*>(artisti[1]);
    instrumentalistptr->setPlayedInstruments({ i1,i2,i3 });

    OneManBand* onemanbandptr = dynamic_cast<OneManBand*>(artisti[2]);
    onemanbandptr->setLoopingEquipment({ "Predefined Drums","Sampling Machine" });

    artisti.associateSound(artisti[0]->getName(), "tracks/Burning Love.mp3");
    artisti.associateSound(artisti[1]->getName(), "tracks/Dragoste de inchiriat.mp3");
    artisti.associateSound(artisti[2]->getName(), "tracks/Go With The Flow.mp3");
    
    //Creare MAP pt a retine perechile gen:nr_aparitii - necesar pentru a sti cand nu mai avem niciun artist cu un anumit genre in program pt a-l putea elimina si din set
    unordered_map<string, set<string>>mapOfGenres; 
    mapOfGenres[artisti[0]->getGenre()].insert(artisti[0]->getName());
    mapOfGenres[artisti[1]->getGenre()].insert(artisti[1]->getName());
    mapOfGenres[artisti[2]->getGenre()].insert(artisti[2]->getName());


    cout << "                               BINE AI VENIT LA PRIMA EDITIE A FESTIVALULUI SOLO-FMI: CEL MAI MARE FESTIVAL AL ARTISTILOR SOLO DIN TARA                              \n";
    cout << "______________________________________________________________________________________________________________________________\n";
    cout << "                               ASTAZI TU DECIZI CE ARTISTI VOR FI PREZENTI SI CE CARACTERISTICI AU ACESTIA                                                            \n";
    cout << "________________________________________________________________________________________________________________________________ \n";
    int appStarted = 1;
    while (appStarted == 1) {
        cout << "Alegeti tipul de operatiune pe care doriti sa o efectuati!\n";
        cout << "___________________________________________________________\n";
        cout << "Apasati 1 pentru a vizualiza artistii deja confirmati la festival\n";
        cout << "Apasati 2 pentru a adauga un nou artist la lista de invitati\n";
        cout << "Apasati 3 pentru a modifica/vizualiza datele despre artistii care vor fi la festival\n";
        cout << "Apasati 4 pentru a elimina unul dintre artistii confirmati\n";
        cout << "Apasati 5 pentru a reorganiza ordinea artistilor de la festival\n";
        cout << "Apasati 6 pentru a vedea statistici legate de genurile muzicale din cadrul festivalului\n";
        cout << "Apasati 7 pentru a asculta toate melodiile de prezentare pentru fiecare artist in ordine\n";
        cout << "Apasati 8 pentru a parasi meniul interactiv\n";
        int cod_start;
        while (true) {
            cout << "Introduceti comanda: ";
            string str_cod_start;
            getline(cin, str_cod_start);
            try {
                if (str_cod_start != "1" && str_cod_start != "2" && str_cod_start != "3" && 
                    str_cod_start != "4" && str_cod_start != "5" && str_cod_start!="6" && str_cod_start!="7"
                    && str_cod_start!="8")
                    throw invalid_argument("Valoarea introdusa nu respecta criteriile!\n");
                if (str_cod_start == "1")
                    cod_start = 1;
                else if (str_cod_start == "2")
                    cod_start = 2;
                else if (str_cod_start == "3")
                    cod_start = 3;
                else if (str_cod_start == "4")
                    cod_start = 4;
                else if (str_cod_start == "5")
                    cod_start = 5;
                else if (str_cod_start == "6")
                    cod_start = 6;
                else if (str_cod_start == "7")
                    cod_start = 7;
                else
                    cod_start=8;
                break;
            }
            catch (const invalid_argument& e) {
                cout << e.what() << "Trebuie introdusa o valoare intreaga cuprinsa intre 1-8\n";
            }
        }
        //cin>>cod_start;
        switch (cod_start) {
        case 1:
        {
            if (artisti.collectionSize() == 0)
                cout << "Nu avem niciun artist confirmat in acest moment!\n";
            else {
                for (int i = 0; i < artisti.collectionSize(); i++)
                    cout << "Artistul " << i + 1 << ": " << artisti[i]->getName() << '\n';
                cout << '\n';
                int nr_artisti = artisti.collectionSize();
                int k = 1;
                while (k == 1) {
                    int index;
                    while (true) {
                        cout << "Introdu numarul artistului despre care vrei sa vezi date suplimentare, sau 0 pentru a te intoarce la meniul principal: ";
                        string testIndex;
                        getline(cin, testIndex);
                        try {
                            int nr = 0;
                            for (char c : testIndex) {
                                if (!isdigit(c))
                                    throw invalid_argument("Valorea introdusa trebuie sa fie pozitiva de tip INT\n");
                                nr = nr * 10 + (c - '0');
                            }
                            try {
                                index = nr;
                                if (index > nr_artisti)
                                    throw runtime_error("Valoarea introdusa este prea mare!\n");
                                break;
                            }
                            catch (const runtime_error& e) {
                                cout << e.what() << "Trebuie introdusa o valoare intreaga cuprinsa intre 1 si " << nr_artisti << " sau 0!\n";
                            }
                        }
                        catch (const invalid_argument& e) {
                            cout << e.what() << "Trebuie introdusa o valoare intreaga cuprinsa intre 1 si " << nr_artisti << " sau 0!\n";
                        }
                    }

                    if (index == 0)
                        k = 2;
                    else {
                        cout << *artisti[index - 1] << '\n';
                    }
                }
            }
            break;
        }

        case 2:
        {
            cout << "Precizati ce fel de artist doriti sa adaugati in lista festivalului\n";
            cout << "Apasati 1 pentru a adauga un artist tip Solist\n";
            cout << "Apasati 2 pentru a adauga un artist tip Instrumentalist\n";
            cout << "Apasati 3 pentru a adauga un artist de forma Solist+Instrumentalist (OneManBand)\n";
            cout << "Apasati 4 pentru a va intoarce inapoi la meniul principal\n";
            int comanda_artist;
            while (true) {
                cout << "Introduceti comanda: ";
                string str_comanda_artist;
                getline(cin, str_comanda_artist);
                try {
                    if (str_comanda_artist != "1" && str_comanda_artist != "2" && str_comanda_artist != "3" && str_comanda_artist != "4")
                        throw invalid_argument("Valoarea introdusa nu respecta criteriile!\n");
                    if (str_comanda_artist == "1")
                        comanda_artist = 1;
                    else if (str_comanda_artist == "2")
                        comanda_artist = 2;
                    else if (str_comanda_artist == "3")
                        comanda_artist = 3;
                    else
                        comanda_artist = 4;
                    break;
                }
                catch (const invalid_argument& e) {
                    cout << e.what() << "Trebuie introdus un numar intreg cuprins intre 1-4\n";
                }
            }

            switch (comanda_artist) {
            case 1:
            {
                artisti.addElement(new Solist);
                cin >> *artisti.collectionBack();
                cout << "Artistul a fost adaugat cu succes la festival!\n";
                mapOfGenres[artisti.collectionBack()->getGenre()].insert(artisti.collectionBack()->getName());

                cout << "Alegeti un fisier audio pentru artistul " << artisti[artisti.collectionSize()-1]->getName() << ":\n";
                string fisierSelectatSolist = chooseAudioFile(audioFiles);
                artisti.associateSound(artisti.collectionBack()->getName(), fisierSelectatSolist);

                break;
            }

            case 2:
            {
                artisti.addElement(new Instrumentalist);
                cin >> *artisti.collectionBack();
                cout << "Artistul a fost adaugat cu succes la festival!\n";
                mapOfGenres[artisti.collectionBack()->getGenre()].insert(artisti.collectionBack()->getName());
                
                cout << "Alegeti un fisier audio pentru artistul " << artisti[artisti.collectionSize() - 1]->getName() << ":\n";
                string fisierSelectatInstrumentalist = chooseAudioFile(audioFiles);
                artisti.associateSound(artisti.collectionBack()->getName(), fisierSelectatInstrumentalist);

                break;
            }

            case 3:
            {
                artisti.addElement(new OneManBand);
                cin >> *artisti.collectionBack();
                cout << "Artistul a fost adaugat cu succes la festival!\n";
                string newArtistGenre = artisti.collectionBack()->getGenre();
                mapOfGenres[artisti.collectionBack()->getGenre()].insert(artisti.collectionBack()->getName());

                cout << "Alegeti un fisier audio pentru artistul " << artisti[artisti.collectionSize() - 1]->getName() << ":\n";
                string fisierSelectatOneManBand = chooseAudioFile(audioFiles);
                artisti.associateSound(artisti.collectionBack()->getName(), fisierSelectatOneManBand);

                break;
            }

            case 4:
            {
                cout << "Intoarcere la meniul principal...\n";
                break;
            }

            }

            break;
        }

        case 3:
        {
            if (artisti.collectionSize() == 0)
                cout << "Nu avem artisti pe care sa-i putem modifica/vizualiza!\n";
            else {
                cout << "Artistii confirmati pentru festival:\n";
                for (int i = 0; i < artisti.collectionSize(); i++)
                    cout << "Artistul " << i + 1 << ": " << artisti[i]->getName() << '\n';
                cout << '\n';

                int k = 1;
                while (k == 1) {
                    int index;
                    while (true) {
                        cout << "Introduceti numarul artistului pe care vrei sa il modifici/vezi date suplimentare, sau 0 pentru a te intoarce la meniul principal: ";
                        string str_index;
                        getline(cin, str_index);
                        try {
                            int nr = 0;
                            for (char c : str_index) {
                                if (!isdigit(c))
                                    throw invalid_argument("Valoarea introdusa nu respecta criteriile!\n");
                                nr = nr * 10 + (c - '0');
                            }
                            try {
                                index = nr;
                                if (index > artisti.collectionSize())
                                    throw runtime_error("Valoarea introdusa este prea mare!\n");
                                break;
                            }
                            catch (const runtime_error& e) {
                                cout << e.what() << "Trebuie introdusa o valoare intreaga cuprinsa intre 1 si " << artisti.collectionSize() << " sau 0!\n";
                            }
                        }
                        catch (const invalid_argument& e) {
                            cout << e.what() << "Trebuie introdusa o valoare intreaga cuprinsa intre 1 si " << artisti.collectionSize() << " sau 0!\n";
                        }
                    }

                    if (index == 0)
                        k = 2;
                    else {
                        cout << "A fost ales artistul " << artisti[index - 1]->getName() << '\n';
                        if (OneManBand* ptrOneManBand = dynamic_cast<OneManBand*>(artisti[index - 1])) { //punem intai verificare pt OneManBand. Altfel, compilatorul ar fi intrat la dowcast pe una din clasele parinte, chiar daca avem OneManBand
                            cout << "Artistul " << artisti[index - 1]->getName() << " este de tip OneManBand!\n";
                            int l = 1;
                            while (l == 1) {
                                cout << "Apasati 1 pentru a calcula suma pe care o va incasa artistul\n";
                                cout << "Apasati 2 pentru a estima numarul de bilete pe care artistul le va vinde\n";
                                cout << "Apasati 3 pentru a recalcula nivelul de spectacol al alrtistului\n";
                                cout << "Apasati 4 pentru a mari puterea vocala a artistului\n";
                                cout << "Apasati 5 pentru a incrementa atributele artistului\n";
                                cout << "Apasati 6 pentru a asculta melodia asociata artistului\n";
                                cout << "Apasati 7 pentru a modifica melodia asociata artistului\n";
                                cout << "Apasati 8 pentru a va intoarce inapoi\n";

                                int comanda_onemanband;
                                while (true) {
                                    cout << "Introduceti comanda: ";
                                    string strComanda_onemanband;
                                    getline(cin, strComanda_onemanband);
                                    try {
                                        if (strComanda_onemanband != "1" && strComanda_onemanband != "2" && strComanda_onemanband != "3"
                                            && strComanda_onemanband != "4" && strComanda_onemanband != "5" && strComanda_onemanband != "6"
                                            && strComanda_onemanband!="7" && strComanda_onemanband!="8")
                                            throw invalid_argument("Valoarea introdusa nu respecta criteriile!\n");
                                        if (strComanda_onemanband == "1")
                                            comanda_onemanband = 1;
                                        else if (strComanda_onemanband == "2")
                                            comanda_onemanband = 2;
                                        else if (strComanda_onemanband == "3")
                                            comanda_onemanband = 3;
                                        else if (strComanda_onemanband == "4")
                                            comanda_onemanband = 4;
                                        else if (strComanda_onemanband == "5")
                                            comanda_onemanband = 5;
                                        else if (strComanda_onemanband == "6")
                                            comanda_onemanband = 6;
                                        else if (strComanda_onemanband == "7")
                                            comanda_onemanband = 7;
                                        else
                                            comanda_onemanband = 8;
                                        break;
                                    }
                                    catch (const invalid_argument& e) {
                                        cout << e.what() << "Trebuie introdusa o valoarea intreaga cuprinsa intre 1-8!\n";
                                    }
                                }

                                switch (comanda_onemanband) {
                                case 1:
                                {
                                    cout << "Suma ce va fi incasata de " << ptrOneManBand->getName() << " este: " << ptrOneManBand->artistRevenue() << '\n';
                                    break;
                                }

                                case 2:
                                {
                                    cout << "Se estimeaza ca " << ptrOneManBand->getName() << " va vinde un total de " << ptrOneManBand->expectedTicketSales() << " de bilete\n";
                                    break;
                                }

                                case 3:
                                {
                                    ptrOneManBand->recalculateStagePresence();
                                    cout << "Nivel de spectacol recalculat cu succes, acum acesta are valoarea " << onemanbandptr->getStagePresence() << '\n';
                                    break;
                                }

                                case 4:
                                {
                                    cout << "Precizati numarul de octave cu care va creste puterea vocala a artistului: ";
                                    float octave;
                                    cin >> octave;
                                    *ptrOneManBand = *ptrOneManBand + octave;
                                    cout << "Puterea vocala a artistului " << ptrOneManBand->getName() << " este de " << ptrOneManBand->getVocalRange() << " octave\n";
                                    break;
                                }

                                case 5:
                                {
                                    ++*ptrOneManBand;
                                    cout << "Atributele artistului " << ptrOneManBand->getName() << " au fost incrementate cu succes\n";
                                    break;
                                }

                                case 6:
                                {
                                    cout << "Playing sound for: " << artisti[index-1]->getName() << ":\n";
                                    artisti.playSound(artisti[index - 1]->getName());
                                    string tasta;
                                    cout << "Apasati Enter pentru a da skip: ";
                                    getline(cin, tasta);
                                    artisti.stopSound(artisti[index - 1]->getName());
                                    break;
                                }

                                case 7:
                                {
                                    cout << "Alegeti un fisier audio pentru artistul " << artisti[index - 1]->getName() << ":\n";
                                    string fisierSelectat = chooseAudioFile(audioFiles);
                                    artisti.associateSound(artisti[index - 1]->getName(), fisierSelectat);
                                    break;
                                }

                                case 8:
                                {
                                    cout << "Intoarcere cu un pas inapoi...\n";
                                    l = 2;
                                    break;
                                }

                                }
                            }
                        }
                        else if (Instrumentalist* ptrInstrumentalist = dynamic_cast<Instrumentalist*>(artisti[index - 1])) {
                            cout << "Artistul " << artisti[index - 1]->getName() << " este de tip Instrumentalist!\n";
                            int l = 1;
                            while (l == 1) {
                                cout << "Apasati 1 pentru a calcula suma pe care o va incasa artistul\n";
                                cout << "Apasati 2 pentru a actualiza lista de instrumente la care artistul canta\n";
                                cout << "Apasati 3 pentru a afisa date despre un anumit instrument\n";
                                cout << "Apasati 4 pentru a incrementa atributele artistului\n";
                                cout << "Apasati 5 pentru a asculta melodia asociata artistului\n";
                                cout << "Apasati 6 pentru a modifica melodia asociata artistului\n";
                                cout << "Apasati 7 pentru a va intoarce inapoi\n";

                                int comanda_instrumentalist;
                                while (true) {
                                    cout << "Introduceti comanda: ";
                                    string strComanda_instrumentalist;
                                    getline(cin, strComanda_instrumentalist);
                                    try {
                                        if (strComanda_instrumentalist != "1" && strComanda_instrumentalist != "2" && strComanda_instrumentalist != "3"
                                            && strComanda_instrumentalist != "4" && strComanda_instrumentalist != "5" 
                                            && strComanda_instrumentalist!="6" && strComanda_instrumentalist!="7")
                                            throw invalid_argument("Valoarea introdusa nu respecta criteriile!\n");
                                        if (strComanda_instrumentalist == "1")
                                            comanda_instrumentalist = 1;
                                        else if (strComanda_instrumentalist == "2")
                                            comanda_instrumentalist = 2;
                                        else if (strComanda_instrumentalist == "3")
                                            comanda_instrumentalist = 3;
                                        else if (strComanda_instrumentalist == "4")
                                            comanda_instrumentalist = 4;
                                        else if (strComanda_instrumentalist == "5")
                                            comanda_instrumentalist = 5;
                                        else if (strComanda_instrumentalist == "6")
                                            comanda_instrumentalist = 6;
                                        else
                                            comanda_instrumentalist = 7;
                                        break;
                                    }
                                    catch (const invalid_argument& e) {
                                        cout << e.what() << "Trebuie introdusa o valoare intreaga cuprinsa intre 1-7!\n";
                                    }
                                }

                                switch (comanda_instrumentalist) {
                                case 1:
                                {
                                    cout << "Suma ce va fi incasata de " << ptrInstrumentalist->getName() << " este: " << ptrInstrumentalist->artistRevenue() << '\n';
                                    break;
                                }

                                case 2:
                                {
                                    ptrInstrumentalist->updateInstruments();
                                    vector<Instrument> instruments = ptrInstrumentalist->getPlayedInstruments();
                                    if (instruments.size() > 0) {
                                        cout << "Instrumentele la care poate canta artistul sunt:\n";
                                        for (int i = 0; i < instruments.size(); i++)
                                            cout << instruments[i].getInstrumentName() << "   ";
                                        cout << '\n';
                                    }
                                    else
                                        cout << "Nu avem niciun instrument in acest moment pentru acest artist\n";
                                    break;
                                }

                                case 3:
                                {
                                    vector<Instrument> instrumentList = ptrInstrumentalist->getPlayedInstruments();
                                    if (instrumentList.size() > 0) {
                                        cout << "Instrumentele din repertoriul artistului sunt:\n";
                                        for (int i = 0; i < instrumentList.size(); i++)
                                            cout << "Instrument " << i + 1 << ": " << instrumentList[i].getInstrumentName() << '\n';

                                        int index_instr;
                                        while (true) {
                                            cout << "Introduceti indicele instrumentului despre care vreti sa vedeti date suplimentare: ";
                                            string strIndex_instr;
                                            getline(cin, strIndex_instr);
                                            try {
                                                int nr = 0;
                                                for (char c : strIndex_instr) {
                                                    if (!isdigit(c))
                                                        throw logic_error("Valoarea trebuie sa fie una pozitiva de tip INT!\n");
                                                    nr = nr * 10 + (c - '0');
                                                }
                                                try {
                                                    index_instr = nr;
                                                    if (index_instr > instrumentList.size())
                                                        throw runtime_error("Valoarea introdusa este prea mare!\n");
                                                    cout << instrumentList[index_instr - 1] << '\n';
                                                    break;
                                                }
                                                catch (const runtime_error& e) {
                                                    cout << e.what() << "Trebuie introdusa o valoare intreaga cuprinsa intre 1 si " << instrumentList.size() << '\n';
                                                }
                                            }
                                            catch (const logic_error& e) {
                                                cout << e.what() << "Trebuie introdusa o valoare intreaga cuprinsa intre 1 si " << instrumentList.size() << '\n';
                                            }
                                        }

                                    }
                                    else
                                        cout << "Nu avem niciun instrument in acest moment pentru acest artist\n";
                                    break;
                                }

                                case 4:
                                {
                                    ++*ptrInstrumentalist;
                                    cout << "Atributele artistului " << ptrInstrumentalist->getName() << " au fost incrementate cu succes\n";
                                    break;
                                }

                                case 5:
                                {
                                    cout << "Playing sound for: " << artisti[index - 1]->getName() << ":\n";
                                    artisti.playSound(artisti[index - 1]->getName());
                                    string tasta;
                                    cout << "Apasati Enter pentru a da skip: ";
                                    getline(cin, tasta);
                                    artisti.stopSound(artisti[index - 1]->getName());
                                    break;
                                }

                                case 6:
                                {
                                    cout << "Alegeti un fisier audio pentru artistul " << artisti[index - 1]->getName() << ":\n";
                                    string fisierSelectat = chooseAudioFile(audioFiles);
                                    artisti.associateSound(artisti[index - 1]->getName(), fisierSelectat);
                                    break;
                                }

                                case 7:
                                {
                                    cout << "Intoarcere cu un pas inapoi...\n";
                                    l = 2;
                                    break;
                                }
                                }
                            }
                        }
                        else if (Solist* ptrSolist = dynamic_cast<Solist*>(artisti[index - 1])) {
                            cout << "Artistul " << artisti[index - 1]->getName() << " este de tip Solist!\n";
                            int l = 1;
                            while (l == 1) {
                                cout << "Apasati 1 pentru a calcula suma pe care o va incasa artistul\n";
                                cout << "Apasati 2 pentru a estima numarul de bilete pe care artistul le va vinde\n";
                                cout << "Apasati 3 pentru a actualiza repertoriul artistului\n";
                                cout << "Apasati 4 pentru a mari puterea vocala a artistului\n";
                                cout << "Apasati 5 pentru a incrementa atributele artistului\n";
                                cout << "Apasati 6 pentru a asculta melodia asociata artistului\n";
                                cout << "Apasati 7 pentru a modifica melodia asociata artistului\n";
                                cout << "Apasati 8 pentru a va intoarce inapoi\n";

                                int comanda_solist;
                                while (true) {
                                    cout << "Introduceti comanda: ";
                                    string strComanda_solist;
                                    getline(cin, strComanda_solist);
                                    try {
                                        if (strComanda_solist != "1" && strComanda_solist != "2" && strComanda_solist != "3"
                                            && strComanda_solist != "4" && strComanda_solist != "5" && strComanda_solist != "6"
                                            && strComanda_solist!="7" && strComanda_solist!="8")
                                            throw invalid_argument("Valoarea introdusa nu respecta criteriile!\n");

                                        if (strComanda_solist == "1")
                                            comanda_solist = 1;
                                        else if (strComanda_solist == "2")
                                            comanda_solist = 2;
                                        else if (strComanda_solist == "3")
                                            comanda_solist = 3;
                                        else if (strComanda_solist == "4")
                                            comanda_solist = 4;
                                        else if (strComanda_solist == "5")
                                            comanda_solist = 5;
                                        else if (strComanda_solist == "6")
                                            comanda_solist = 6;
                                        else if (strComanda_solist == "7")
                                            comanda_solist = 7;
                                        else
                                            comanda_solist = 8;
                                        break;
                                    }
                                    catch (const invalid_argument& e) {
                                        cout << e.what() << "Trebuie introdusa o valoare intreaga cuprinsa intre 1-8!\n";
                                    }
                                }

                                switch (comanda_solist) {
                                case 1:
                                {
                                    cout << "Suma ce va fi incasata de " << ptrSolist->getName() << "este: " << ptrSolist->artistRevenue() << '\n';
                                    break;
                                }

                                case 2:
                                {
                                    cout << "Se estimeaza ca " << ptrSolist->getName() << " va vinde un total de " << ptrSolist->expectedTicketSales() << " de bilete\n";
                                    break;
                                }

                                case 3:
                                {
                                    cout << "Introduceti numele melodiei pe care vreti sa o adaugati in repertoriul artistului: ";
                                    string melodie;
                                    getline(cin, melodie);
                                    ptrSolist->updateRepertoriu(melodie);
                                    cout << "Apasati 1 daca doriti sa vedeti intregul repertoriu al artistului: ";
                                    string StrMiniCmd;
                                    getline(cin, StrMiniCmd);
                                    if (StrMiniCmd == "1") {
                                        list <string> melodii = ptrSolist->getRepertoriu();
                                        int i = 0;
                                        for (const auto& melodie : melodii) {
                                            cout << melodie << '\n';
                                        }
                                    }
                                    else
                                        cout << "Intoarcere cu un pas inapoi...\n";
                                    break;
                                }

                                case 4:
                                {
                                    cout << "Precizati numarul de octave cu care va creste puterea vocala a artistului: ";
                                    float octave;
                                    cin >> octave;
                                    cin.get();
                                    *ptrSolist = *ptrSolist + octave;
                                    cout << "Puterea vocala a artistului " << ptrSolist->getName() << " este de " << ptrSolist->getVocalRange() << " octave\n";
                                    break;
                                }

                                case 5:
                                {
                                    ++*ptrSolist;
                                    cout << "Atributele artistului " << ptrSolist->getName() << " au fost incrementate cu succes\n";
                                    break;
                                }

                                case 6:
                                {
                                    cout << "Playing sound for: " << artisti[index - 1]->getName() << ":\n";
                                    artisti.playSound(artisti[index - 1]->getName());
                                    string tasta;
                                    cout << "Apasati Enter pentru a da skip: ";
                                    getline(cin, tasta);
                                    artisti.stopSound(artisti[index - 1]->getName());
                                    break;
                                }

                                case 7:
                                {
                                    cout << "Alegeti un fisier audio pentru artistul " << artisti[index - 1]->getName() << ":\n";
                                    string fisierSelectat = chooseAudioFile(audioFiles);
                                    artisti.associateSound(artisti[index - 1]->getName(), fisierSelectat);
                                    break;
                                }

                                case 8:
                                {
                                    cout << "Intoarcere cu un pas inapoi...\n";
                                    l = 2;
                                    break;
                                }
                                }
                            }
                        }
                        else
                            cout << "Tip necunoscut!\n";
                    }
                }
            }
            break;
        }

        case 4:
        {
            if (artisti.collectionSize() == 0) {
                cout << "Nu putem elimina artisti deoarece nu avem!:)))\n";
            }
            else {
                int k = 1;
                while (k == 1) {
                    cout << "Lista de artisti:\n";
                    for (int i = 0; i < artisti.collectionSize(); i++) {
                        cout << "Artistul " << i + 1 << ": " << artisti[i]->getName() << '\n';
                    }
                    int comanda_delete;
                    while (true) {
                        cout << "Introduceti numarul corespunzator al artistului pe care doriti sa-l eliminati sau 0 pentru a va intoarce la meniul principal\n";
                        cout << "Numarul: ";
                        string str_comanda_delete;
                        getline(cin, str_comanda_delete);
                        try {
                            int nr = 0;
                            for (char c : str_comanda_delete) {
                                if (!isdigit(c))
                                    throw invalid_argument("Valoarea introdusa trebuie sa fie una pozitiva de tip INT\n");
                                nr = nr * 10 + (c - '0');
                            }
                            try {
                                comanda_delete = nr;
                                if (comanda_delete > artisti.collectionSize())
                                    throw runtime_error("Valoarea introdusa este prea mare!\n");
                                break;
                            }
                            catch (const runtime_error& e) {
                                cout << e.what() << "Trebuie introdus un numar intreg cuprins intre 1 si " << artisti.collectionSize() << " sau 0!\n";
                            }
                        }
                        catch (const invalid_argument& e) {
                            cout << e.what() << "Trebuie introdus un numar intreg cuprins intre 1 si " << artisti.collectionSize() << " sau 0!\n";
                        }
                    }
                    //cin>>comanda_delete;
                    if (comanda_delete == 0) {
                        cout << "Intoarcere la meniul principal...\n";
                        k = 2;
                    }
                    else {
                        string deleted_artist = artisti[comanda_delete - 1]->getName();

                        string deleted_artistGenre = artisti[comanda_delete - 1]->getGenre();
                        string deleted_artistName = artisti[comanda_delete - 1]->getName();

                        mapOfGenres[deleted_artistGenre].erase(deleted_artistName);
                        if (mapOfGenres[deleted_artistGenre].empty()) //nu mai avem artisti cu acest gen muzical
                            mapOfGenres.erase(deleted_artistGenre);

                        artisti.eraseFromCollection(comanda_delete - 1,deleted_artistName);
                        cout << "Artistul " << deleted_artist << " a fost eliminat cu succes!\n";
                        if (artisti.collectionSize() == 0) {
                            cout << "Toti artistii au fost eliminati! Intoarcere la meniul principal...\n";
                            k = 2;
                        }
                    }
                }
            }
            break;
        }

        case 5:
        {
            if (artisti.collectionSize() == 0) {
                cout << "Nu putem reorganiza artistio deoarece nu avem!:)))\n";
            }
            else {
               
                int l = 1;
                while (l == 1) {
                    cout << "Apasati 1 pentru a ordona artistii in ordine crescatoare (in functie de numarul de ascultatori lunari\n";
                    cout << "Apasati 2 pentru a ordona artistii in ordine descrescatoare (in functie de numarul de ascultatori lunari\n";
                    cout << "Apasati 3 pentru a amesteca artistii in mod aleator\n";
                    cout << "Apasati 4 pentru a va intoarce inapoi\n";

                    int comanda_ordonare;
                    while (true) {
                        cout << "Introduceti comanda: ";
                        string strComanda_ordonare;
                        getline(cin, strComanda_ordonare);
                        try {
                            if(strComanda_ordonare!="1" && strComanda_ordonare!="2" && 
                                strComanda_ordonare!="3" && strComanda_ordonare!="4")
                                throw invalid_argument("Valoarea introdusa nu respecta criteriile!\n");
                            if (strComanda_ordonare == "1")
                                comanda_ordonare = 1;
                            else if (strComanda_ordonare == "2")
                                comanda_ordonare = 2;
                            else if (strComanda_ordonare == "3")
                                comanda_ordonare = 3;
                            else
                                comanda_ordonare = 4;
                            break;
                        }
                        catch (const invalid_argument& e) {
                            cout << e.what() << "Trebuie introdusa o valoare intreaga cuprinsa intre 1-4\n";
                        }
                    }

                    switch (comanda_ordonare) {
                        case 1:
                        {
                            artisti.sortAscending();
                            cout << "Artistii au fost sortati crescator cu succes!\n";

                            cout << "Noua ordine a artistilor este:\n";
                            for (int i = 0; i < artisti.collectionSize(); i++)
                                cout << "Artistul " << i + 1 << ": " << artisti[i]->getName() << '\n';
                            cout << '\n';
                            break;
                        }

                        case 2:
                        {
                            artisti.sortDescending();
                            cout << "Artistii au fost sortati descrescator cu succes!\n";

                            cout << "Noua ordine a artistilor este:\n";
                            for (int i = 0; i < artisti.collectionSize(); i++)
                                cout << "Artistul " << i + 1 << ": " << artisti[i]->getName() << '\n';
                            cout << '\n';
                            break;
                        }

                        case 3:
                        {
                            artisti.shuffleCollection();
                            cout << "Artistii au fost amestecati in mod aleator cu succes!\n";

                            cout << "Noua ordine a artistilor este:\n";
                            for (int i = 0; i < artisti.collectionSize(); i++)
                                cout << "Artistul " << i + 1 << ": " << artisti[i]->getName() << '\n';
                            cout << '\n';
                            break;
                        }

                        case 4:
                        {
                            cout << "Intoarcere cu un pas inapoi...\n";
                            l = 2;
                            break;
                        }
                    }
                }
            }
            break;
        }

        case 6:
        {
            if (artisti.collectionSize() == 0) {
                cout << "Nu putem arata astfel de date deoarece nu avem artisti!:)))\n";
            }
            else {

                int l = 1;
                while (l == 1) {
                    cout << "Apasati 1 pentru a vedea cel mai popular gen muzical in cadrul festivalului\n";
                    cout << "Apasati 2 pentru a vedea numele artistilor in functie de genul muzical cantat\n";
                    cout << "Apasati 3 pentru a va intoarce inapoi\n";

                    int comanda_genuri;
                    while (true) {
                        cout << "Introduceti comanda: ";
                        string strComanda_genuri;
                        getline(cin, strComanda_genuri);
                        try {
                            if (strComanda_genuri != "1" && strComanda_genuri != "2" &&
                                strComanda_genuri != "3")
                                throw invalid_argument("Valoarea introdusa nu respecta criteriile!\n");
                            if (strComanda_genuri == "1")
                                comanda_genuri = 1;
                            else if (strComanda_genuri == "2")
                                comanda_genuri = 2;
                            else
                                comanda_genuri = 3;
                            break;
                        }
                        catch (const invalid_argument& e) {
                            cout << e.what() << "Trebuie introdusa o valoare intreaga cuprinsa intre 1-3\n";
                        }
                    }

                    switch (comanda_genuri) {
                    case 1:
                    {
                        string popularGenre = "unknown";
                        int popularitate = 0;
                        for (const auto& pair : mapOfGenres) {
                            if (pair.second.size() > popularitate) {
                                popularitate = pair.second.size();
                                popularGenre = pair.first;
                            }
                        }
                        
                        cout << "Cel mai populat gen muzical este: " << popularGenre << "!\n";

                        break;
                    }

                    case 2:
                    {
                        cout << "Mai jos se afla artistii grupati in functie de genum muzical cantat\n";
                        for (const auto& pair : mapOfGenres) {
                            cout << pair.first << ": ";
                            for (const auto& artist : pair.second)
                                cout << artist << "     ";
                            cout << '\n';
                        }
                        break;
                    }

                    case 3:
                    {
                        cout << "Intoarcere cu un pas inapoi...\n";
                        l = 2;
                        break;
                    }
                    }
                }
            }
            break;
        }

        case 7:
        {
            for (int i = 0; i < artisti.collectionSize(); i++) {
                cout << "Playing sound for: " << artisti[i]->getName() << ":\n";
                artisti.playSound(artisti[i]->getName());
                string tasta;
                cout << "Apasati tasta Enter pentru a da skip: ";
                getline(cin, tasta);
                artisti.stopSound(artisti[i]->getName());
            }
            break;
        }

        case 8:
        {
            cout << "Iesire din program...\n";
            appStarted = 2;
            break;
        }
        }
    }
    return 0;
}

Singleton* Singleton::getInstance() {
    if (instance == NULL)
        instance = new Singleton();
    return instance;
}


string Singleton::getTestedMenu() {
    return this->tested_menu;
}

void Singleton::setTestedMenu(string tested_menu) {
    this->tested_menu = tested_menu;
}


int main()
{
    
    Singleton* a = Singleton::getInstance();
    a->meniu();

    return 0;
}