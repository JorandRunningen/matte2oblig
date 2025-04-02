#include "funksjon.h"
vector<CoefAndFrequency> Function::none(){
    vector<CoefAndFrequency> cof;
    return cof;
}

vector<CoefAndFrequency> Function::getNewFunction(){
    return newFunctionCoef;
}

Function::Function(){
    newFunctionCoef = none();
    try{loadAllFunctions();} // prøver å hente inn alle funksjonene fra .txt filen, og skriver ut feilmelding hvis ikke det går, men fortsetter programmet uansett
    catch(string &e){
        cout << e << endl;
    }
}

void Function::fourier(const GuitarString& guitarString, const GuitarString& speed){
    // an = integ(0, pi)(f(x)*sin(nx)*dx),   f(x) er initialposisjon
    // bn = 1/(c*n*pi) * integ(0, pi)(g(x)*sin(nx)*dx),   g(x) er initialfart
    
    vector<CoefAndFrequency> coefVec;
    double an = 0, bn = 0, x = 0, yPos = 0, ySpe = 0;
    double c = guitarString.getSpeed();
    double dx = guitarString.getString().at(1).x - guitarString.getString().at(0).x;
    CoefAndFrequency coef;
    for (int n = 1; n < 200; n++){
        an = 0;
        bn = 0;
        for (int i = 0; i < guitarString.getString().size() - 1; i++){
            x = guitarString.getString().at(i).x;
            yPos = guitarString.getString().at(i).y;
            ySpe = speed.getString().at(i).y;
            an +=  yPos*sin(n*c*x)*dx;
            bn += ySpe*sin(n*c*x)*dx; 
        }
        an *= 2/pi;
        bn *= 2/(n*c*pi);
        coef.n = n;
        coef.a = an;
        coef.b = bn;
        coefVec.push_back(coef);
    }
    newFunctionCoef = coefVec;
}

bool Function::addFunction(string name){
    if (name.size() == 0){name = "funksjon" + to_string(allFunctions.size() + 1);}

    // sørge for at navnet ikke består av spesialtegn, så kun bokstaver og tall
    name = onlyLettersAndNumbers(name);

    // sjekke om navnet er tatt 
    for (string i : allFunctionsName){
        if (name == i) {return false;}
    }
    if (name == "Ny"){return false;}

    allFunctionsName.push_back(name);
    allFunctions[name] = newFunctionCoef;
    return true;
}

void Function::removeFunction(string name){
    allFunctions.erase(name);
    for (int i=0; i < allFunctionsName.size(); i++){
        if (name == allFunctionsName.at(i)) {
            allFunctionsName.erase(allFunctionsName.begin() + i);
            break;
        }
    }
}

bool Function::saveAllFunctions(){
    filesystem::path pathToFile{"koefissienter.txt"};
    ofstream fileStream(pathToFile);
    if (fileStream.fail()){
        return false;
    }
    fileStream << allFunctions.size() << endl;
    for (auto [name, function]: allFunctions){
        fileStream << name << " "; // navn 
        fileStream << function.size() << " "; // antall koefisienter
        for (CoefAndFrequency coef : function){
            fileStream << coef.n << " " << coef.a << " " << coef.b << " ";
        }
        fileStream << -1 << endl; // markere slutt 
    }
    return true;
}

void Function::loadAllFunctions(){
    filesystem::path pathToFile{"koefissienter.txt"};
    ifstream fileStream(pathToFile);
    
    // kaster et unntak om filstørmmen ikke lykkes
    if (fileStream.fail()){
        string feil = "Funksjonen kunne ikke lastes inn";
        throw feil;
    }

    allFunctionsName.clear();
    allFunctions.clear();
    
    
    int numberOfFunctions;
    fileStream >> numberOfFunctions;

    string name;
    for (int i = 0; i < numberOfFunctions; i++){
        fileStream >> name;
        
        vector<CoefAndFrequency> function;
        int numberOfCoef;
        fileStream >> numberOfCoef;
        for (int i = 0; i < numberOfCoef; i++){
            CoefAndFrequency coef;
            fileStream >> coef.n;
            fileStream >> coef.a;
            fileStream >> coef.b;
            function.push_back(coef);
        }
        int allOkayUpload;
        fileStream >> allOkayUpload;

        // lagre kun hvis hentingen av data ser ut til å ha gått greit, og da skal siste element være -1, systemet er ikke vellykt om ting slettes fra linjene
        if (allOkayUpload == -1){
            allFunctions[name] = function;
            allFunctionsName.push_back(name); 
        }
    }
}

string Function::onlyLettersAndNumbers(const string& word){
    string newWord = "";
    int letter;
    for (int i = 0; i < word.size(); i++){
        letter = word.at(i);
        if (((48 <= letter) && (letter <= 57)) || // tall
            ((65 <= letter) && (letter <= 90)) || // store bokstaver 
            ((97 <= letter) && (letter <= 122)))  // små bokstraver
            {newWord += letter;} 
    }
    return newWord;
}

vector<CoefAndFrequency> Function::getCoef(string name){
    return allFunctions.at(name);
}
