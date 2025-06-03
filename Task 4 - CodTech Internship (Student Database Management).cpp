#include <iostream>
#include <fstream> //for using file based functions and classes
#include <string> //for using getline()
#include <sstream> //for using stringstream()
using namespace std;

struct Student
{ //each student's data record consists of it's roll, name and average marks obtained
    int roll;
    string name;
    double marks;
} stud[100]; //stores data of upto a 100 students
/*we want to store the data into the file likeso
101 Maulik 92 (<roll> <name> <marks>)
*/
int i; //reflects the count of all students - global counter
int searchroll(int r, string filename) //now checks uniqueness by searching directly in the file
{
    string line;
    int roll;
    
    ifstream file(filename); //open file in read mode
    
    if(!file) 
    {
        cout << "Cannot open file to search roll number.\n";
        return 1; //fail-safe: allow addition to proceed
    }

    while(getline(file, line)) 
    {
        stringstream ss(line);
        ss >> roll;
        if(roll == r) 
        {
            file.close();
            return 0; // roll already exists
        }
    }

    file.close();
    return 1; // roll is unique
}


void copypaste(string source, string dest) 
{
    string line;
    ifstream in(source); //opens the source file in read mode
    ofstream out(dest); //opens the destination file in write mode

    if (!in || !out) //when either file can't be opened, we won't proceed
    {
        cout << "Error copying files.\n";
        return;
    }

    while (getline(in, line)) 
    {
        out << line << endl;
    }

    in.close();
    out.close();
}

void addrecord(string filename)
{   
    fstream file(filename, ios::app); //opens the file in append mode
    
    if(!file) //when file can't be opened, we won't proceed
    {
        cout << "Cannot open file!";
        return;
    }
    
    cout << "Enter the roll number, name and average marks: ";
    cin >> stud[i].roll >> stud[i].name >> stud[i].marks; //user input

    if (stud[i].roll < 0) //checks if the roll number is negative
    {
        cout << "Roll number cannot be negative! Cannot Add a record!" << endl;
        file.close();
        return;
    }
    
    if(searchroll(stud[i].roll, filename)) //when the roll is unique
    {
        //now we write these values into the file
        file << stud[i].roll << " " << stud[i].name << " " << stud[i].marks << endl; //file input
        i++; //increments the value of i to ensure that each time no value is overwritten and that
    //the previous value(s) are retained, also because the student count increases
    }
    else
        cout << "The same roll number already exists! Cannot Add a record!" << endl; //count doesn't change
    
    file.close();
}

void deleterecord(string filename, int r)
{
    string line; //holds each line of the file
    int flag = 0; //checks whether the roll number exists
    int rol; //to extract the roll number from each line
    
    fstream file(filename, ios::in); //the main file is opened in read mode
    fstream outfile("studentoutput.txt", ios::out); //the resultant file is opened in write (overwrite) mode
    
    if(!file || !outfile) //when either files can't be opened, we won't proceed
    {
        cout << "Cannot open file(s)!";
        return;
    }
    
    while (getline(file, line)) //reads the file line-by-line until the end
    {
        stringstream ss(line); //breaks down each line into 3 parts - roll, name, and marks
        ss >> rol; //holds the roll number in each line, one at a time as an integer

        if (rol == r)
        {   //checks for matching roll number
            flag = 1; //ensures that the roll number exists
            i--; //student count decreases as data will be deleted
            continue; //avoids copying of the student's data which needs to be deleted
        }

        //copies all the student info except the one which has to be deleted
        outfile << line << endl;
    }
    
    if(flag)
        cout << "Student data has been deleted! Please check the 'studentoutput.txt' file" << endl;
    else
        cout << "Student's roll number doesn't match with the list! Cannot delete!" << endl;
        
    file.close();
    outfile.close();
}

void displayrecords(string filename)
{
    string line;
    
    fstream file(filename, ios::in); //opens the file in read mode
    
    if(!file) //when file can't be opened, we won't proceed
    {
        cout << "Cannot open file!";
        return;
    }    
    
    while(!file.eof()) //loop runs to the end of the file
    {
        getline(file, line); //copy each line from the file into the string 'line'
        cout << line << endl; //prints each line of the file
    }
    
    file.close();
}

void modifyrecord(string filename, int r)
{
    string line; //holds each line of the file
    int flag = 0, flag2 = 0; //checks whether the roll number exists
    int rol; //to extract the roll number from each line
    string nme; //to extract the name from each line
    double mrks; //to extract the average marks from each line
    int choice;
    
    fstream file(filename, ios::in); //the main file is opened in read mode
    fstream outfile("studentoutput.txt", ios::out); //the resultant file is opened in write (overwrite) mode

    if(!file || !outfile) //when either files can't be opened, we won't proceed
    {
        cout << "Cannot open file(s)!";
        return;
    }    
    
    while (getline(file, line)) //reads the file line-by-line until the end
    {
        stringstream ss(line); //breaks down each line into 3 parts - roll, name, and marks
        ss >> rol >> nme >> mrks; //holds the roll number, name and average marks
        //in each line, one at a time as an integer, string and double type

        if (rol == r)
        {   //checks for matching roll number
            cout << "Enter the modified record: (Roll, Name, Marks): " << endl;
            cin >> rol >> nme >> mrks;

            if (rol < 0) //checks if the modified roll number is negative
            {
                cout << "Roll number cannot be negative! Modification aborted.\n";
                flag2 = 1;
            }
            else if(searchroll(rol, "student.txt"))
                flag = 1; //ensures that the roll number exists
            else
                flag2 = 1; //roll not unique
        }
        if(flag2 == 0) 
            outfile << rol << " " << nme << " " << mrks << endl; //manually incorporates the new changed line for all cases but when
            //the newly entered data's roll is not unique or is negative
        else 
            outfile << line << endl; //when the modified data's roll isn't valid, we simply don't modify it
    }
    
    if(flag)
        cout << "Student data has been modified! Please check the 'studentoutput.txt' file" << endl;
    else if(flag == 0 && flag2 == 0)
        cout << "Student's roll number doesn't match with the list! Cannot modify!" << endl;
    else if(flag == 0 && flag2 == 1)
        cout << "You need to enter a valid and unique roll number! Please return back to the menu and re-try!" << endl;
        
    file.close();
    outfile.close();
}

int main()
{
    int choice, i = 0, r;
    string filename = "student.txt";
    while(1) //always true - infinite loop
    {
        cout << "Enter your choice:\n";
        cout << "1. Add a Record to the file:\n";
        cout << "2. Delete a Record from the file:\n";
        cout << "3. Modify a Record from the file:\n";
        cout << "4. Display all the Records from the file:\n";
        cout << "5. Exit the menu:\n";
        cin >> choice;
        switch(choice)
        {
            case 1: addrecord(filename); 
            break;
            
            case 2:
            cout << "Enter the roll number of the student whose data needs to be deleted: ";
            cin >> r;
            deleterecord(filename, r);
            copypaste("studentoutput.txt", filename); //reflects the changes in the original file
            break;
            
            case 3: 
            cout << "Enter the roll number of the student whose data needs to be modified: ";
            cin >> r;
            modifyrecord(filename, r);
            copypaste("studentoutput.txt", filename); //reflects the changes in the original file
            break;
            
            case 4: displayrecords(filename);
            break;
            
            case 5: return 1;
            
            default: cout << "Please re-enter between 1 and 5: ";
        }
    }
}