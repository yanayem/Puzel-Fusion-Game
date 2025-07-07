#include <iostream>
#include <fstream>
#include <vector>
#include <ctime>
#include <string>
#include <thread> // For adding delays
#include <chrono> // For timing
#include <conio.h>
#include <map>
#include <algorithm> // For sorting

using namespace std;

const string RESET = "\033[0m";
const string RED = "\033[31m";
const string GREEN = "\033[32m";
const string YELLOW = "\033[33m";
const string BLUE = "\033[34m";

int MAX_TRIES = 10;
int MAX_POINTS = 100;

//LeaderboardEntry structure
struct LeaderboardEntry
{
    string name;
    int score;
    string gameType;
};


int letterFill(char, string, string&);
void guessCountry(const string& username);
void mathProblems(const string& username);
void guessPerson(const string& username);
void numberGuessing(const string& username);
void registerUser();
bool loginUser(string& username);
void loadingAnimation(string message, int duration);
void feedbackAnimation(string message, char symbol, int times);
void updateLeaderboard(const string& username, int score, const string& gameType);
void showLeaderboard();

int main()
{
    string username;
    int choice;
    loadingAnimation("Loading Game Hub", 3);

    // Login Interface
    cout << "===== Welcome to the Game Hub =====" << endl;
    cout << "1. Register" << endl;
    cout << "2. Login" << endl;
    cout << "Enter your choice: ";
    cin >> choice;

    if (choice == 1)
    {
        registerUser();
    }

    if (!loginUser(username))
    {
        cout << "Login failed. Exiting the program." << endl;
        return 0;
    }

    // Game Menu
    while (true)
    {
        /*cout << "\n====== Game Menu ======" << endl;
        cout << "1. Guess the Country" << endl;
        cout << "2. Math Problems" << endl;
        cout << "3. Guess the Person" << endl;
        cout << "4. Number Guessing" << endl;
        cout << "5. View Leaderboard" << endl;
        cout << "6. Exit" << endl;*/
        cout << "Enter your choice: ";
        cout << BLUE << "\n====== Game Menu ======" << RESET << endl;
        cout << YELLOW << "1. Guess the Country" << RESET << endl;
        cout << YELLOW << "2. Math Problems" << RESET << endl;
        cout << YELLOW << "3. Guess the Person" << RESET << endl;
        cout << YELLOW << "4. Number Guessing" << RESET << endl;
        cout << YELLOW << "5. View Leaderboard" << RESET << endl;
        cout << YELLOW << "6. Exit" << RESET << endl;
        cout << GREEN << "Enter your choice: " << RESET;

        cin >> choice;

        switch (choice)
        {
        case 1:
            guessCountry(username);
            break;
        case 2:
            mathProblems(username);
            break;
        case 3:
            guessPerson(username);
            break;
        case 4:
            numberGuessing(username);
            break;
        case 5:
            showLeaderboard();
            break;
        case 6:
            feedbackAnimation("Exiting the program. Goodbye!", '*', 3);
            return 0;
        default:
            feedbackAnimation("Invalid choice. Please try again.", '!', 5);

        }
    }
    return 0;
}

void loadingAnimation(string message, int duration)
{
    cout << message;
    for (int i = 0; i < duration; ++i)
    {
        cout << ".";
        this_thread::sleep_for(chrono::milliseconds(500)); // Simulated delay
    }
    cout << endl;
}

void feedbackAnimation(string message, char symbol, int times)
{
    cout << message << " ";
    for (int i = 0; i < times; ++i)
    {
        cout << symbol;
        this_thread::sleep_for(chrono::milliseconds(200)); // Simulated delay
        cout.flush();
    }
    cout << endl;
}
// Function to register a new user
void registerUser()
{
    string username, password;
    ofstream usersFile("users.txt", ios::app);

    cout << "===== Register New User =====" << endl;
    cout << "Register with a username: ";
    cin >> username;
    cout << "Register with a password: ";
    cin >> password;

    // Store the username and password in the file
    usersFile << username << " " << password << endl;
    usersFile.close();
    feedbackAnimation("Registration successful! You can now log in.", '.', 3);

    //cout << "Registration successful! You can now log in." << endl;
}

// Function to log in an existing user
bool loginUser(string& username)
{
    string /*username,*/ password, fileUsername, filePassword;
    ifstream usersFile("users.txt");

    int attempts = 0; // Counter for login attempts
    const int maxAttempts = 3; // Maximum number of login attempts

    while (attempts < maxAttempts)
    {
        cout << "===== User Login =====" << endl;
        cout << "Enter your username: ";
        cin >> username;
        cout << "Enter your password: ";
        cin >> password;

        // Simulate a delay while checking credentials
        loadingAnimation("Checking credentials", 3);

        // Reset file stream to the beginning
        usersFile.clear();
        usersFile.seekg(0, ios::beg);

        // Check credentials from the file
        while (usersFile >> fileUsername >> filePassword)
        {
            if (username == fileUsername && password == filePassword)
            {
                feedbackAnimation("Login successful! Welcome, " + username + "!", '*', 3);
                return true;
            }
        }
        // If login fails
        attempts++;
        if (attempts < maxAttempts)
        {
            feedbackAnimation("Invalid username or password. Try again.", '!', 3);
        }
        else
        {
            feedbackAnimation("Login failed after " + to_string(maxAttempts) + " attempts.", '!', 5);
        }
    }

    usersFile.close();
    return false; // Login unsuccessful after maximum attempts
}

//Function to guess the country
void guessCountry(const string& username)
{
    string word, hint;
    char letter;
    int num_of_wrong_guesses = 0;
    int points = MAX_POINTS;
    vector<string> words;
    vector<string> hints;

    // Read countries and hints from the file
    ifstream file("countries.txt");
    if (!file)
    {
        feedbackAnimation("Error opening countries file!", '!', 5);
        return;
    }

    string line;
    while (getline(file, line))
    {
        size_t commaPos = line.find(',');
        if (commaPos != string::npos)
        {
            string country = line.substr(0, commaPos);
            string countryHint = line.substr(commaPos + 1);
            words.push_back(country);
            hints.push_back(countryHint);
        }
    }
    file.close();

    // Check if the file was empty
    if (words.empty())
    {
        feedbackAnimation("No countries found in the file!", '!', 5);
        return;
    }

    // Randomly select a country
    srand(time(NULL));
    int n = rand() % words.size();
    word = words[n];
    hint = hints[n];

    string unknown(word.length(), '_');

    loadingAnimation("Starting the 'Guess the Country' game", 3);
    cout << "\nYou have " << MAX_TRIES << " tries to guess the word." << endl;

    while (num_of_wrong_guesses < MAX_TRIES)
    {
        cout << endl << hint << endl;
        cout << "\n" << unknown;

        cout << "\nGuess a letter: ";
        cin >> letter;

        // Check if the input is more than one character or invalid
        if (cin.peek() != '\n' || isspace(letter))
        {
            feedbackAnimation("Invalid input! Please enter only one letter at a time.", '!', 3);
            cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Clear the input buffer
            continue;  // Skip the rest of the loop and ask for input again
        }

        if (letterFill(letter, word, unknown) == 0) // Assuming letterFill updates 'unknown'
        {
            feedbackAnimation("Whoops! That letter isn't in there!", '-', 5);
            num_of_wrong_guesses++;
            points -= 10;
        }
        else
        {
            feedbackAnimation("You found a letter!", '+', 3);
        }

        cout << "You have " << MAX_TRIES - num_of_wrong_guesses << " guesses left." << endl;
        cout << "Your current points: " << points << endl;

        if (word == unknown)
        {
            cout << word << endl;
            feedbackAnimation("Yeah! You got it!", '*', 5);
            cout << "Your final points: " << points << endl;

            // Update leaderboard with the final score
            updateLeaderboard(username, points, "Guess the Country");
            return;
        }
    }

    feedbackAnimation("Sorry, you lose!", '-', 5);
    cout << "\nThe word was " << word << endl;
    cout << "Your final points: " << points << endl;

    // Update leaderboard with the final score even if the user loses
    updateLeaderboard(username, points, "Guess the Country");
}

// Function for "Math Problems" game
void mathProblems(const string& username)
{
    int userAnswer, correctAnswer;
    int score = 0;
    const int numQuestions = 5;
    const int pointsPerQuestion = 20;
    string difficulty;
    int maxRange;

    // Choose difficulty level with validation
    while (true)
    {
        cout << "Choose a difficulty level (easy, medium, hard or 1, 2, 3): ";
        cin >> difficulty;

        if (difficulty == "easy" || difficulty == "1")
        {
            maxRange = 10;
            break;
        }
        else if (difficulty == "medium" || difficulty == "2")
        {
            maxRange = 20;
            break;
        }
        else if (difficulty == "hard" || difficulty == "3")
        {
            maxRange = 50;
            break;
        }
        else
        {
            cout << "Invalid choice. Please try again." << endl;
        }
    }

    srand(time(0));
    loadingAnimation("\nWelcome to the 'Math Problems' game!", 3);
    cout << "You will be given " << numQuestions << " random math problems. Let's start!" << endl;

    for (int i = 1; i <= numQuestions; i++)
    {
        int questionType = rand() % 5; // Randomly select question type
        int a = rand() % maxRange + 1; // Random number between 1 and maxRange
        int b = rand() % maxRange + 1;

        cout << "\nQuestion " << i << ": ";

        // Generate different types of math problems
        switch (questionType)
        {
        case 0: // Addition
            cout << "What is " << a << " + " << b << "? ";
            correctAnswer = a + b;
            break;

        case 1: // Subtraction
            cout << "What is " << a << " - " << b << "? ";
            correctAnswer = a - b;
            break;

        case 2: // Multiplication
            cout << "What is " << a << " * " << b << "? ";
            correctAnswer = a * b;
            break;

        case 3: // Division (integer division)
            if (b == 0) b = 1; // Avoid division by zero
            cout << "What is " << a << " / " << b << "? (Round down to nearest integer) ";
            correctAnswer = a / b;
            break;

        case 4: // Simple algebra (solve for x)
            cout << "Solve for x: " << a << "x = " << (a * b) << ". What is x? ";
            correctAnswer = b;
            break;
        }

        // Get user answer with error handling for invalid input
        while (!(cin >> userAnswer))
        {
            feedbackAnimation("Invalid input! Please enter a valid number.", '!', 3);
            cin.clear(); // Clear the error flag
            cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Ignore the rest of the line
            cout << "Try again: ";
        }

        // Check if the answer is correct
        if (userAnswer == correctAnswer)
        {
            cout << "Correct! Well done!" << endl;
            score += pointsPerQuestion;
        }
        else
        {
            cout << "Incorrect. The correct answer is " << correctAnswer << "." << endl;
        }
    }

    // Display final score
    cout << "\nQuiz complete! Your total score is: " << score << " out of " << (numQuestions * pointsPerQuestion) << "." << endl;

    // Give feedback based on performance
    if (score >= 80)
    {
        feedbackAnimation("Great job! You passed the quiz!", '-', 5);
    }
    else
    {
        feedbackAnimation("Better luck next time!", '-', 5);
    }

    // Update leaderboard after the game ends
    updateLeaderboard(username, score, "Math Problems");
}

// Function for "Guess the Person" game
void guessPerson(const string& username)
{
    string person, hint;
    char letter;
    int num_of_wrong_guesses = 0;
    int points = MAX_POINTS;
    vector<string> persons;
    vector<string> hints;

    // Read the names and hints from the file
    ifstream file("persons.txt");
    if (!file)
    {
        cerr << "Error opening persons file!" << endl;
        return;
    }

    string line;
    while (getline(file, line))
    {
        size_t delimiterPos = line.find(',');
        if (delimiterPos != string::npos)
        {
            string name = line.substr(0, delimiterPos);
            string personHint = line.substr(delimiterPos + 1);
            persons.push_back(name);
            hints.push_back(personHint);
        }
    }
    file.close();

    if (persons.empty())
    {
        cout << "No data found in persons file!" << endl;
        return;
    }

    srand(time(NULL));
    int n = rand() % persons.size();
    person = persons[n];
    hint = hints[n];

    string unknown(person.length(), '_');

    loadingAnimation("\n\nWelcome to the 'Guess the Person' game!", 3);
    cout << "\nYou have " << MAX_TRIES << " tries to guess the name." << endl;

    while (num_of_wrong_guesses < MAX_TRIES)
    {
        cout << endl << hint << endl;
        cout << "\n" << unknown;

        cout << "\nGuess a letter: ";

        // Input validation: Ensure the user enters only one valid letter
        while (true)
        {
            cin >> letter;
            if (cin.fail() || !isalpha(letter) || cin.peek() != '\n')  // Check if input is a single alphabet
            {
                cout << "Invalid input! Please enter only one letter at a time: ";
                cin.clear(); // Clear the error flag
                cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Ignore the rest of the line
            }
            else
            {
                break; // Valid input, exit the loop
            }
        }

        // Check if the letter is in the word
        if (letterFill(letter, person, unknown) == 0)
        {
            cout << "\nWhoops! That letter isn't in there!" << endl;
            num_of_wrong_guesses++;
            points -= 10;
        }
        else
        {
            cout << "\nYou found a letter!" << endl;
        }

        cout << "You have " << MAX_TRIES - num_of_wrong_guesses << " guesses left." << endl;
        cout << "Your current points: " << points << endl;

        // Check if the word has been guessed
        if (person == unknown)
        {
            cout << person << endl;
            cout << "Yeah! You got it!" << endl;
            cout << "Your final points: " << points << endl;
            // Update leaderboard with the final score
            updateLeaderboard(username, points, "Guess the Person");
            return;
        }
    }

    cout << "\nSorry, you lose. The person was " << person << endl;
    cout << "Your final points: " << points << endl;

    // Update leaderboard with the final score, even if the user lost
    updateLeaderboard(username, points, "Guess the Person");
}

void numberGuessing(const string& username)
{
    int randomNumber, userGuess, attempts = 0, maxAttempts, rangeStart, rangeEnd;
    int lowerBound, upperBound;
    string difficulty;
    int score = 0;

    // Choose difficulty level
    cout << "Choose a difficulty level (easy, medium, hard): ";
    cin >> difficulty;

    if (difficulty == "easy" || difficulty == "1")
    {
        maxAttempts = 10;
        rangeStart = 1;
        rangeEnd = 50;
    }
    else if (difficulty == "medium" || difficulty == "2")
    {
        maxAttempts = 7;
        rangeStart = 1;
        rangeEnd = 100;
    }
    else if (difficulty == "hard" || difficulty == "3")
    {
        maxAttempts = 5;
        rangeStart = 1;
        rangeEnd = 200;
    }
    else
    {
        cout << "Invalid choice. Defaulting to medium difficulty." << endl;
        maxAttempts = 7;
        rangeStart = 1;
        rangeEnd = 100;
    }

    lowerBound = rangeStart;
    upperBound = rangeEnd;

    srand(time(0));
    randomNumber = rand() % (rangeEnd - rangeStart + 1) + rangeStart;

    loadingAnimation("\nWelcome to the 'Number Guessing' game!", 3);
    cout << "You chose " << difficulty << " difficulty." << endl;
    cout << "I have selected a random number between " << rangeStart << " and " << rangeEnd << "." << endl;
    cout << "You have " << maxAttempts << " attempts to guess it correctly." << endl;

    while (attempts < maxAttempts)
    {
        cout << "Enter your guess: ";
        cin >> userGuess;
        attempts++;

        if (userGuess == randomNumber)
        {
            cout << GREEN << "Congratulations! You guessed the number correctly in " << attempts << " attempts!" << RESET << endl;
            score = max(10 - attempts, 1) * 10;  // Example scoring system
            cout << GREEN << "You earned " << score << " points!" << RESET << endl;
            break;  // Exit the loop since the number was guessed correctly
        }
        else if (userGuess < randomNumber)
        {
            cout << RED << "Too low! Try a higher number." << RESET << endl;
            lowerBound = max(lowerBound, userGuess + 1);
        }
        else
        {
            cout << RED << "Too high! Try a lower number." << RESET << endl;
            upperBound = min(upperBound, userGuess - 1);
        }

        cout << YELLOW << "Hint: The number is between " << lowerBound << " and " << upperBound << "." << RESET << endl;
        cout << "Attempts left: " << maxAttempts - attempts << endl;
    }

    if (userGuess != randomNumber)
    {
        cout << RED << "\nSorry, you've used all your attempts. The correct number was " << randomNumber << "." << RESET << endl;
        score = 0;  // No points if the user fails
    }

    // Update leaderboard with the final score, even if the user lost
    updateLeaderboard(username, score, "Number Guessing");
}

// Function to fill the guessed letter
int letterFill(char guess, string secretword, string &guessword)
{
    int matches = 0;
    for (size_t i = 0; i < secretword.length(); i++)
    {
        if (guess == guessword[i])
            return 0;
        if (guess == secretword[i])
        {
            guessword[i] = guess;
            matches++;
        }
    }
    return matches;
}

// Update leaderboard function
void updateLeaderboard(const string& username, int score, const string& gameType)
{
    // Define the file name based on the game type
    string filename;
    if (gameType == "Guess the Country")
    {
        filename = "guess_the_country_leaderboard.txt";
    }
    else if (gameType == "Math Problems")
    {
        filename = "math_problems_leaderboard.txt";
    }
    else if (gameType == "Guess the Person")
    {
        filename = "guess_the_person_leaderboard.txt";
    }
    else if (gameType == "Number Guessing")
    {
        filename = "number_guessing_leaderboard.txt";
    }
    else
    {
        cout << "Unknown game type!" << endl;
        return;
    }

    // Read existing leaderboard entries
    vector<LeaderboardEntry> leaderboard;
    ifstream inFile(filename);

    if (inFile.is_open())
    {
        string name, type;
        int sc;
        while (inFile >> name >> sc >> type)
        {
            leaderboard.push_back({name, sc, type});
        }
        inFile.close();
    }

    // Add the new score entry
    leaderboard.push_back({username, score, gameType});

    // Sort the leaderboard by score (highest first)
    sort(leaderboard.begin(), leaderboard.end(), [](const LeaderboardEntry& a, const LeaderboardEntry& b)
    {
        return a.score > b.score;
    });

    // Write the updated leaderboard back to the file
    ofstream outFile(filename, ios::trunc);
    if (outFile.is_open())
    {
        for (const auto& entry : leaderboard)
        {
            outFile << entry.name << " " << entry.score << " " << entry.gameType << endl;
        }
        outFile.close();
    }
    else
    {
        cout << "Error: Could not open leaderboard file for writing." << endl;
    }
}


// Show leaderboard function
void showLeaderboard()
{
    int gameChoice;
    cout << "Select the game type:\n";
    cout << "1. Guess the Country\n";
    cout << "2. Math Problems\n";
    cout << "3. Guess the Person\n";
    cout << "4. Number Guessing\n";
    cout << "Enter your choice (1-4): ";
    cin >> gameChoice;

    string gameType;
    switch (gameChoice)
    {
    case 1:
        gameType = "Guess the Country";
        break;
    case 2:
        gameType = "Math Problems";
        break;
    case 3:
        gameType = "Guess the Person";
        break;
    case 4:
        gameType = "Number Guessing";
        break;
    default:
        cout << "Invalid choice.\n";
        return;
    }

    // Define the filename based on the chosen game type
    string filename;
    if (gameType == "Guess the Country")
    {
        filename = "guess_the_country_leaderboard.txt";
    }
    else if (gameType == "Math Problems")
    {
        filename = "math_problems_leaderboard.txt";
    }
    else if (gameType == "Guess the Person")
    {
        filename = "guess_the_person_leaderboard.txt";
    }
    else if (gameType == "Number Guessing")
    {
        filename = "number_guessing_leaderboard.txt";
    }

    // Open the appropriate leaderboard file
    ifstream inFile(filename);
    if (!inFile)
    {
        cout << "Leaderboard is empty or the file could not be found.\n";
        return;
    }

    // Display the leaderboard
    cout << "\n=== Leaderboard for " << gameType << " ===\n";
    string name, type;
    int score;
    int rank = 1;
    bool found = false;

    while (inFile >> name >> score >> type)
    {
        // Display all entries without dates
        cout << rank << ". " << name << " - " << score << " points\n";
        rank++;
        found = true;
    }

    if (!found)
    {
        cout << "No scores available for " << gameType << ".\n";
    }

    inFile.close();
}




