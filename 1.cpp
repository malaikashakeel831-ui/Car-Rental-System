#include <iostream>
#include <fstream>
#include <iomanip>
#include <string>

using namespace std;

/* -----------------------
   Global dynamic arrays
------------------------*/
int *carID = nullptr;
string *carModel = nullptr;
string *carCategory = nullptr;
double *pricePerDay = nullptr;
bool *isAvailable = nullptr;
string *renterName = nullptr;
int *rentedDays = nullptr;
double *currentBill = nullptr;

int carCount = 0;
int capacity = 0;

/* Files */
const string DATA_FILE = "cars.txt";
const string BACKUP_FILE = "backup.dat";

/* -----------------------
   Function Prototypes
------------------------*/
// Initialization
void initializeSystem();
void shutdownSystem();
void ensureCapacity();
void resizeArrays(int newCapacity);

// ID & Input
int generateCarID();
string getString(const string &prompt);
int getInt(const string &prompt);
double getDouble(const string &prompt);
void inputCarData(int idx);

// Core Car Operations
void addCar();
void viewCars();
void printCarHeader();
void printCarRow(int idx);
void searchCar();
void displayCarByIndex(int idx);
void deleteCar();
bool confirmDelete();
void shiftLeftFromIndex(int idx);
void rentCar();
void returnCar();
double calculateBill(int idx);
void billingReceipt(int idx);
void processReturn(int idx);

// File I/O
void saveToFile();
void loadFromFile();
void saveBinary();
void loadBinary();
void backupAuto();

// Extra Reports / Sorting
void sortCarsByPrice();
void sortCarsByModel();
void filterAvailableCars();
void filterRentedCars();
double totalRevenue();
int countAvailableCars();
int countRentedCars();
double averagePrice();
void highestPricedCar();
void lowestPricedCar();

// UI
void displayMenu();
void pause();
int findCarIndexByID(int id);

/* -----------------------
   Implementation
------------------------*/

// Initialization & Memory
void initializeSystem() {
    capacity = 4;
    carCount = 0;
    resizeArrays(capacity);
}

void shutdownSystem() {
    delete[] carID; delete[] carModel; delete[] carCategory; delete[] pricePerDay;
    delete[] isAvailable; delete[] renterName; delete[] rentedDays; delete[] currentBill;
    carID = nullptr; carModel = nullptr; carCategory = nullptr; pricePerDay = nullptr;
    isAvailable = nullptr; renterName = nullptr; rentedDays = nullptr; currentBill = nullptr;
    carCount = capacity = 0;
}

void ensureCapacity() {
    if (carCount >= capacity) resizeArrays(capacity*2);
}

void resizeArrays(int newCapacity) {
    int *newCarID = new int[newCapacity];
    string *newModel = new string[newCapacity];
    string *newCategory = new string[newCapacity];
    double *newPrice = new double[newCapacity];
    bool *newAvail = new bool[newCapacity];
    string *newRenter = new string[newCapacity];
    int *newRentedDays = new int[newCapacity];
    double *newBill = new double[newCapacity];

    for (int i=0;i<carCount;i++){
        newCarID[i]=carID[i];
        newModel[i]=carModel[i];
        newCategory[i]=carCategory[i];
        newPrice[i]=pricePerDay[i];
        newAvail[i]=isAvailable[i];
        newRenter[i]=renterName[i];
        newRentedDays[i]=rentedDays[i];
        newBill[i]=currentBill[i];
    }

    delete[] carID; delete[] carModel; delete[] carCategory; delete[] pricePerDay;
    delete[] isAvailable; delete[] renterName; delete[] rentedDays; delete[] currentBill;

    carID=newCarID; carModel=newModel; carCategory=newCategory; pricePerDay=newPrice;
    isAvailable=newAvail; renterName=newRenter; rentedDays=newRentedDays; currentBill=newBill;
    capacity=newCapacity;
}

// ID & Input
int generateCarID() {
    static int lastID = 1000;
    lastID++;
    return lastID;
}

string getString(const string &prompt) {
    cout << prompt;
    string s; getline(cin, s);
    return s;
}

int getInt(const string &prompt) {
    int x;
    while (true){
        cout << prompt;
        if (cin >> x){ cin.ignore(1000,'\n'); return x; }
        else { cout << "Invalid input.\n"; cin.clear(); cin.ignore(1000,'\n'); }
    }
}

double getDouble(const string &prompt) {
    double x;
    while (true){
        cout << prompt;
        if (cin >> x){ cin.ignore(1000,'\n'); return x; }
        else { cout << "Invalid input.\n"; cin.clear(); cin.ignore(1000,'\n'); }
    }
}

void inputCarData(int idx) {
    carID[idx]=generateCarID();
    carModel[idx]=getString("Enter model: ");
    carCategory[idx]=getString("Enter category: ");
    carCount++; // increment temporarily
}

// Core Operations
void addCar() {
    ensureCapacity();
    int idx=carCount;
    inputCarData(idx);
    pricePerDay[idx]=getDouble("Enter price per day: ");
    isAvailable[idx]=true;
    renterName[idx]="";
    rentedDays[idx]=0;
    currentBill[idx]=0.0;
    carCount++;
    cout<<"Car added with ID: "<<carID[idx]<<"\n";
    saveToFile(); saveBinary();
}

void printCarHeader(){
    cout << left << setw(6) << "ID" << setw(20) << "Model" << setw(12) << "Category"
         << setw(10) << "Price" << setw(12) << "Status" << setw(18) << "Renter\n";
    cout << string(80,'-') << "\n";
}

void printCarRow(int idx){
    cout << left << setw(6) << carID[idx] << setw(20) << carModel[idx] << setw(12) << carCategory[idx]
         << setw(10) << fixed << setprecision(2) << pricePerDay[idx]
         << setw(12) << (isAvailable[idx]?"Available":"Rented")
         << setw(18) << (isAvailable[idx]?"-":renterName[idx]) << "\n";
}

void viewCars(){
    if (carCount==0){ cout<<"No cars.\n"; return; }
    printCarHeader();
    for (int i=0;i<carCount;i++) printCarRow(i);
}

int findCarIndexByID(int id){
    for(int i=0;i<carCount;i++) if (carID[i]==id) return i;
    return -1;
}

void displayCarByIndex(int idx){
    if (idx>=0 && idx<carCount) { printCarHeader(); printCarRow(idx); }
    else cout<<"Invalid index.\n";
}

void searchCar(){
    int id=getInt("Enter Car ID: ");
    int idx=findCarIndexByID(id);
    if (idx==-1) cout<<"Car not found.\n";
    else displayCarByIndex(idx);
}

bool confirmDelete(){
    string c=getString("Confirm delete? (y/n): ");
    return (c=="y" || c=="Y");
}

void shiftLeftFromIndex(int idx){
    for (int i=idx;i<carCount-1;i++){
        carID[i]=carID[i+1];
        carModel[i]=carModel[i+1];
        carCategory[i]=carCategory[i+1];
        pricePerDay[i]=pricePerDay[i+1];
        isAvailable[i]=isAvailable[i+1];
        renterName[i]=renterName[i+1];
        rentedDays[i]=rentedDays[i+1];
        currentBill[i]=currentBill[i+1];
    }
    carCount--;
}

void deleteCar(){
    int id=getInt("Enter Car ID to delete: ");
    int idx=findCarIndexByID(id);
    if (idx==-1){ cout<<"Not found.\n"; return; }
    if (!isAvailable[idx]){ cout<<"Car rented.\n"; return; }
    if (confirmDelete()){
        shiftLeftFromIndex(idx);
        cout<<"Car deleted.\n";
        saveToFile(); saveBinary();
    }
}

double calculateBill(int idx){
    return pricePerDay[idx]*rentedDays[idx]*1.10; // 10% tax
}

void rentCar(){
    int id=getInt("Enter Car ID to rent: ");
    int idx=findCarIndexByID(id);
    if (idx==-1){ cout<<"Not found.\n"; return; }
    if (!isAvailable[idx]){ cout<<"Already rented.\n"; return; }
    renterName[idx]=getString("Renter Name: ");
    rentedDays[idx]=getInt("Days to rent: ");
    currentBill[idx]=calculateBill(idx);
    isAvailable[idx]=false;
    cout<<"Bill: "<<fixed<<setprecision(2)<<currentBill[idx]<<"\n";
    saveToFile(); saveBinary();
}

void billingReceipt(int idx){
    cout<<"\n-- BILLING --\n";
    cout<<"Car ID: "<<carID[idx]<<"\nModel: "<<carModel[idx]<<"\nRenter: "<<renterName[idx]
        <<"\nDays: "<<rentedDays[idx]<<"\nTotal: "<<currentBill[idx]<<"\n";
}

void processReturn(int idx){
    billingReceipt(idx);
    isAvailable[idx]=true;
    renterName[idx]="";
    rentedDays[idx]=0;
    currentBill[idx]=0.0;
}

void returnCar(){
    int id=getInt("Car ID to return: ");
    int idx=findCarIndexByID(id);
    if (idx==-1){ cout<<"Not found.\n"; return; }
    if (isAvailable[idx]){ cout<<"Already available.\n"; return; }
    processReturn(idx);
    saveToFile(); saveBinary();
}

/* -----------------------
   File I/O
------------------------*/
/* -----------------------
   File I/O with CSV header
   ----------------------- */
void saveToFile() {
    ofstream fout(DATA_FILE);
    if(!fout){ cout<<"Error opening file\n"; return; }

    // CSV Header
    fout << "ID|Model|Category|Price|Available|Renter|Days|Bill\n";

    for(int i=0;i<carCount;i++){
        fout << carID[i] << "|" 
             << carModel[i] << "|" 
             << carCategory[i] << "|" 
             << pricePerDay[i] << "|" 
             << (isAvailable[i]?1:0) << "|" 
             << renterName[i] << "|" 
             << rentedDays[i] << "|" 
             << currentBill[i] << "\n";
    }
}

void loadFromFile() {
    ifstream fin(DATA_FILE); 
    if(!fin) return;

    carCount = 0; 
    string line;

    // Skip CSV header
    getline(fin, line);

    while(getline(fin, line)){
        string tmp=line; 
        int id, avail, days; 
        double price, bill;
        string model, category, renter;

        auto nextToken = [&](string &s)->string {
            size_t p=s.find('|'); 
            string t=(p==string::npos)?s:s.substr(0,p);
            if(p!=string::npos) s=s.substr(p+1); else s="";
            return t;
        };

        id = stoi(nextToken(tmp));
        model = nextToken(tmp);
        category = nextToken(tmp);
        price = stod(nextToken(tmp));
        avail = stoi(nextToken(tmp));
        renter = nextToken(tmp);
        days = stoi(nextToken(tmp));
        bill = stod(nextToken(tmp));

        ensureCapacity();
        carID[carCount] = id;
        carModel[carCount] = model;
        carCategory[carCount] = category;
        pricePerDay[carCount] = price;
        isAvailable[carCount] = (avail==1);
        renterName[carCount] = renter;
        rentedDays[carCount] = days;
        currentBill[carCount] = bill;
        carCount++;
    }
    fin.close();
    cout << "Loaded " << carCount << " cars from " << DATA_FILE << "\n";
}

void saveBinary(){
    ofstream fout(BACKUP_FILE,ios::binary);
    fout.write((char*)&carCount,sizeof(carCount));
    for(int i=0;i<carCount;i++){
        fout.write((char*)&carID[i],sizeof(int));
        int len=carModel[i].size();
        fout.write((char*)&len,sizeof(len));
        fout.write(carModel[i].c_str(),len);
        len=carCategory[i].size();
        fout.write((char*)&len,sizeof(len));
        fout.write(carCategory[i].c_str(),len);
        fout.write((char*)&pricePerDay[i],sizeof(double));
        fout.write((char*)&isAvailable[i],sizeof(bool));
        len=renterName[i].size();
        fout.write((char*)&len,sizeof(len));
        fout.write(renterName[i].c_str(),len);
        fout.write((char*)&rentedDays[i],sizeof(int));
        fout.write((char*)&currentBill[i],sizeof(double));
    }
    fout.close();
}

void loadBinary(){
    ifstream fin(BACKUP_FILE,ios::binary);
    if(!fin) return;
    int count=0; fin.read((char*)&count,sizeof(count));
    while(capacity<count) resizeArrays(capacity*2);
    carCount=0;
    for(int i=0;i<count;i++){
        int idx=i;
        fin.read((char*)&carID[idx],sizeof(int));
        int len=0; fin.read((char*)&len,sizeof(len));
        char *buf=new char[len+1]; fin.read(buf,len); buf[len]='\0'; carModel[idx]=buf; delete[] buf;
        fin.read((char*)&len,sizeof(len)); buf=new char[len+1]; fin.read(buf,len); buf[len]='\0'; carCategory[idx]=buf; delete[] buf;
        fin.read((char*)&pricePerDay[idx],sizeof(double));
        fin.read((char*)&isAvailable[idx],sizeof(bool));
        fin.read((char*)&len,sizeof(len)); buf=new char[len+1]; fin.read(buf,len); buf[len]='\0'; renterName[idx]=buf; delete[] buf;
        fin.read((char*)&rentedDays[idx],sizeof(int));
        fin.read((char*)&currentBill[idx],sizeof(double));
        carCount++;
    }
    fin.close();
}

void backupAuto(){ saveBinary(); cout<<"Backup done.\n"; }

/* -----------------------
   Extra Reports / Sorting
------------------------*/
void sortCarsByPrice(){
    for(int i=0;i<carCount-1;i++)
        for(int j=i+1;j<carCount;j++)
            if(pricePerDay[i]>pricePerDay[j]){
                swap(carID[i],carID[j]); swap(carModel[i],carModel[j]); swap(carCategory[i],carCategory[j]);
                swap(pricePerDay[i],pricePerDay[j]); swap(isAvailable[i],isAvailable[j]);
                swap(renterName[i],renterName[j]); swap(rentedDays[i],rentedDays[j]); swap(currentBill[i],currentBill[j]);
            }
}

void sortCarsByModel(){
    for(int i=0;i<carCount-1;i++)
        for(int j=i+1;j<carCount;j++)
            if(carModel[i]>carModel[j]){
                swap(carID[i],carID[j]); swap(carModel[i],carModel[j]); swap(carCategory[i],carCategory[j]);
                swap(pricePerDay[i],pricePerDay[j]); swap(isAvailable[i],isAvailable[j]);
                swap(renterName[i],renterName[j]); swap(rentedDays[i],rentedDays[j]); swap(currentBill[i],currentBill[j]);
            }
}

void filterAvailableCars(){
    printCarHeader();
    for(int i=0;i<carCount;i++) if(isAvailable[i]) printCarRow(i);
}

void filterRentedCars(){
    printCarHeader();
    for(int i=0;i<carCount;i++) if(!isAvailable[i]) printCarRow(i);
}

double totalRevenue(){
    double sum=0.0;
    for(int i=0;i<carCount;i++) sum+=currentBill[i];
    return sum;
}

int countAvailableCars(){
    int c=0; for(int i=0;i<carCount;i++) if(isAvailable[i]) c++; return c;
}

int countRentedCars(){
    int c=0; for(int i=0;i<carCount;i++) if(!isAvailable[i]) c++; return c;
}

double averagePrice(){
    if(carCount==0) return 0.0;
    double sum=0.0;
    for(int i=0;i<carCount;i++) sum+=pricePerDay[i];
    return sum/carCount;
}

void highestPricedCar(){
    if(carCount==0){ cout<<"No cars.\n"; return; }
    int idx=0;
    for(int i=1;i<carCount;i++) if(pricePerDay[i]>pricePerDay[idx]) idx=i;
    displayCarByIndex(idx);
}

void lowestPricedCar(){
    if(carCount==0){ cout<<"No cars.\n"; return; }
    int idx=0;
    for(int i=1;i<carCount;i++) if(pricePerDay[i]<pricePerDay[idx]) idx=i;
    displayCarByIndex(idx);
}

/* -----------------------
   UI
------------------------*/
void displayMenu(){
    cout<<"\n=== Car Rental ===\n";
    cout<<"1. Add Car\n2. View Cars\n3. Search Car\n4. Delete Car\n5. Rent Car\n6. Return Car\n";
    cout<<"7. Save Text\n8. Load Text\n9. Save Binary\n10. Load Binary\n11. Backup Now\n";
    cout<<"12. Sort by Price\n13. Sort by Model\n14. Show Available\n15. Show Rented\n";
    cout<<"16. Total Revenue\n17. Count Available\n18. Count Rented\n19. Avg Price\n20. Highest Price\n21. Lowest Price\n";
    cout<<"0. Exit\n";
}

void pause(){ cout<<"\nPress Enter..."; cin.ignore(1000,'\n'); }

/* -----------------------
   Main
------------------------*/
int main(){
    initializeSystem();
    loadFromFile();
    loadBinary();
    int choice;
    while(true){
        displayMenu();
        choice=getInt("Choice: ");
        switch(choice){
            case 1: addCar(); break;
            case 2: viewCars(); break;
            case 3: searchCar(); break;
            case 4: deleteCar(); break;
            case 5: rentCar(); break;
            case 6: returnCar(); break;
            case 7: saveToFile(); break;
            case 8: loadFromFile(); break;
            case 9: saveBinary(); break;
            case 10: loadBinary(); break;
            case 11: backupAuto(); break;
            case 12: sortCarsByPrice(); break;
            case 13: sortCarsByModel(); break;
            case 14: filterAvailableCars(); break;
            case 15: filterRentedCars(); break;
            case 16: cout<<"Revenue: "<<totalRevenue()<<"\n"; break;
            case 17: cout<<"Available: "<<countAvailableCars()<<"\n"; break;
            case 18: cout<<"Rented: "<<countRentedCars()<<"\n"; break;
            case 19: cout<<"Avg Price: "<<averagePrice()<<"\n"; break;
            case 20: highestPricedCar(); break;
            case 21: lowestPricedCar(); break;
            case 0:
                saveToFile(); saveBinary(); shutdownSystem();
                return 0;
            default: cout<<"Invalid choice.\n";
        }
        pause();
    }
}