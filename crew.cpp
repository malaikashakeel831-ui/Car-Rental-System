#include "airport.h"

void addCrew() 
{
    cout << "\n--- ADD NEW CREW MEMBER ---\n";
    
    int crewID;
    while (true) 
    {
        crewID = getValidatedInt("Enter Crew ID (3-digit number 100-999): ", 100, 999);
        if (crewIDExists(crewID))
        {
            cout << "Crew ID " << crewID << " already exists! Please use a different ID.\n";
        } 
        else
        {
            break;
        }
    }

    string crewName = getValidatedString("Enter Crew Name: ", 1, MAX_NAME - 1);
    char **newCrewNames = new char*[crewCount + 1];
    int *newCrewIDs = new int[crewCount + 1];
    bool *newCrewAvailable = new bool[crewCount + 1];
    for (int i = 0; i < crewCount; i++) 
    {
        newCrewNames[i] = crewNames[i];
        newCrewIDs[i] = crewIDs[i];
        newCrewAvailable[i] = crewAvailable[i];
    }

    newCrewNames[crewCount] = new char[crewName.length() + 1];
    strcpy(newCrewNames[crewCount], crewName.c_str());
    newCrewIDs[crewCount] = crewID;
    newCrewAvailable[crewCount] = true;

    delete[] crewNames;
    delete[] crewIDs;
    delete[] crewAvailable;

    crewNames = newCrewNames;
    crewIDs = newCrewIDs;
    crewAvailable = newCrewAvailable;

    crewCount++;
    cout << " Crew member '" << crewName << "' (ID: " << crewID << ") added successfully and marked as Available.\n";
}

void displayCrew() 
{
    if (crewCount == 0)
    {
        cout << " No crew members registered.\n";
        return;
    }
    
    int availableCount = 0;
    for (int i = 0; i < crewCount; i++) 
    {
        if (crewAvailable[i]) availableCount++;
    }
    
    cout << "\n--- CREW MEMBERS (" << crewCount << " total, " << availableCount << " available) ---\n";
    line();
    cout << setw(5) << left << "Idx" << setw(10) << "Crew ID" << setw(25) << "Name" << setw(12) << "Available" << "\n";
    line();
    
    for (int i = 0; i < crewCount; i++)
     {
        cout << setw(5) << left << i 
             << setw(10) << crewIDs[i] 
             << setw(25) << crewNames[i] 
             << setw(12) << (crewAvailable[i] ? "Yes" : " No") << "\n";
    }
    line();
}

void markCrewAvailable() 
{
    if (crewCount == 0)
    {
        cout << " No crew members available.\n";
        return;
    }

    displayCrew();
    int idx = getValidatedInt("Enter Crew index to mark as AVAILABLE: ", 0, crewCount - 1);
    
    if (crewAvailable[idx]) 
    {
        cout << "Crew ID " << crewIDs[idx] << " is already marked as available.\n";
    } else 
    {
        crewAvailable[idx] = true;
        cout << "Crew ID " << crewIDs[idx] << " (" << crewNames[idx] << ") marked as available.\n";
    }
}

void markCrewUnavailable() 
{
    if (crewCount == 0) 
    {
        cout << " No crew members available.\n";
        return;
    }

    displayCrew();
    int idx = getValidatedInt("Enter Crew index to mark as UNAVAILABLE: ", 0, crewCount - 1);
    
    if (!crewAvailable[idx]) 
    {
        cout << " Crew ID " << crewIDs[idx] << " is already marked as unavailable.\n";
    } else {
        crewAvailable[idx] = false;
        cout << "Crew ID " << crewIDs[idx] << " (" << crewNames[idx] << ") marked as unavailable.\n";
    }
}

void countAvailableCrew() 
{
    int availableCount = 0;
    for (int i = 0; i < crewCount; i++) 
    {
        if (crewAvailable[i]) availableCount++;
    }
    
    cout << "\n--- CREW AVAILABILITY SUMMARY ---\n";
    cout << "Total Crew Members: " << crewCount << "\n";
    cout << "Available Crew: " << availableCount << "\n";
    cout << "Unavailable Crew: " << (crewCount - availableCount) << "\n";
    cout << "Availability Rate: " << fixed << setprecision(1) 
         << (crewCount > 0 ? (availableCount * 100.0 / crewCount) : 0) << "%\n";
}

void searchCrewByID() 
{
    if (crewCount == 0) 
    {
        cout << "No crew members available to search.\n";
        return;
    }

    int searchID = getValidatedInt("Enter Crew ID to search: ", 0, 999);
    
    bool found = false;
    for (int i = 0; i < crewCount; i++) 
    {
        if (crewIDs[i] == searchID) {
            cout << "\n✓ Crew Member Found!\n";
            line();
            cout << "Crew ID: " << crewIDs[i] << "\n";
            cout << "Name: " << crewNames[i] << "\n";
            cout << "Available: " << (crewAvailable[i] ? "Yes" : "No") << "\n";
            cout << "Index: " << i << "\n";
            line();
            found = true;
            break;
        }
    }
    
    if (!found) 
    {
        cout << " Crew ID " << searchID << " not found in the system.\n";
    }
}

void crewMenu() {
    int ch;
    do {
        clearScreen();
        printTitle("CREW MANAGEMENT");
        cout << "[1] Add Crew\n";
        cout << "[2] Display Crew\n";
        cout << "[3] Mark Crew Available\n";
        cout << "[4] Mark Crew Unavailable\n";
        cout << "[5] Count Available Crew\n";
        cout << "[6] Search Crew by ID\n";
        cout << "[7] Back to Main Menu\n";
        line();
        ch = getValidatedInt("Enter choice (1-7): ", 1, 7);

        switch(ch) {
            case 1: addCrew(); break;
            case 2: displayCrew(); break;
            case 3: markCrewAvailable(); break;
            case 4: markCrewUnavailable(); break;
            case 5: countAvailableCrew(); break;
            case 6: searchCrewByID(); break;
            case 7: break;
        }
        if (ch != 7) { 
            cout << "\nPress Enter to continue..."; 
            cin.get(); 
        }

    } while(ch != 7);
}