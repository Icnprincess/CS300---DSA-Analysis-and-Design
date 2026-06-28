//============================================================================
// Name        : ABCUAdvisingProgram.cpp
// Author      : Isis Jackson
// Version     : 1.0
// Copyright   : Copyright � 2023 SNHU COCE
// Description : Project 2
//============================================================================

#include <iostream>
#include <vector>
#include <fstream>
#include <sstream>
#include <string>
#include <algorithm>
#include <stdexcept>
using namespace std;

//============================================================================
// Global definitions visible to all methods and classes
//============================================================================

// define a structure to hold information
struct Course {
    string courseId; // unique identifier
    string courseTitle;
	vector<string> prerequisites; // list of prerequisites    
};

// Internal structure for tree node
struct Node {
    Course course;
    Node *left;
    Node *right;

    // default constructor
    Node() {
        left = nullptr;
        right = nullptr;
    }

    // initialize with a course
    Node(Course aCourse) : Node() {
        course = aCourse;
    }
};

//============================================================================
// Binary Search Tree class definition
//============================================================================

/**
 * Define a class containing data members and methods to
 * implement a binary search tree
 */
class BinarySearchTree {

private:
    Node* root;
    void addNode(Node* node, Course course);
    void inOrder(Node* node);
    void deleteNode(Node* node);   

public:
    BinarySearchTree();
    virtual ~BinarySearchTree();
    void InOrder();
    void Insert(Course course);
    Course Search(string courseId);
};

// Default constructor 
BinarySearchTree::BinarySearchTree() {
    root = nullptr;
}

// Destructor 
BinarySearchTree::~BinarySearchTree() {    
    deleteNode(root);
}

// Recursively delete all nodes in the tree
void BinarySearchTree::deleteNode(Node* node) {
    if (node != nullptr) {
        deleteNode(node->left);
        deleteNode(node->right);
        delete node;
    }
}

//Traverse the tree in order
void BinarySearchTree::InOrder() {     
    inOrder(root);
}

//Insert a course
void BinarySearchTree::Insert(Course course) {              
    if (root == nullptr) {
        root = new Node(course);
    }
    else {
        addNode(root, course);
    }    
}

// Add a course to some node (recursive)
void BinarySearchTree::addNode(Node* node, Course course) {
    // if course ID is smaller, add to left subtree
    if (course.courseId < node->course.courseId) {
        if (node->left == nullptr) {
            node->left = new Node(course);
        }
        else {
            addNode(node->left, course);
        }
    }
    else {
        if (node->right == nullptr) {
            node->right = new Node(course);
        }
        else {
            addNode(node->right, course);
        }
    }
}

//Search for a bid
Course BinarySearchTree::Search(string courseId) {
    Node* current = root;
    while (current != nullptr) {
        if (current->course.courseId == courseId) {
            return current->course;
        }
        if (courseId < current->course.courseId) {
            current = current->left;
        }
        else {
            current = current->right;
        }
    }
    Course foundCourse;
    return foundCourse;
}

void BinarySearchTree::inOrder(Node* node) { 
	if (node != nullptr) { //if node is not equal to null ptr
        inOrder(node->left);
        cout << "     " << node->course.courseId << ": " << node->course.courseTitle << endl;
        inOrder(node->right);
    }
}

//============================================================================
// Static methods used for testing
//============================================================================

//Trim leading and trailing whitespace from a string
string trimWhitespace(string str) {
	size_t first = str.find_first_not_of(" \t\r\n"); // Find the first non-whitespace character
	if (first == string::npos) { // If the string is all whitespace, return an empty string
        return "";
    }
	size_t last = str.find_last_not_of(" \t\r\n"); // Find the last non-whitespace character
	return str.substr(first, (last - first + 1)); // Return the trimmed string
}

//Display the course information to the console 
void displayCourse(Course course) {
	cout << "\n     Course ID: " << course.courseId << endl;
	cout << "     Course Title: " << course.courseTitle << endl;
    cout << "     Prerequisites: ";
    
    if (course.prerequisites.empty()) {
        cout << "None" << endl;
    }
    else {
        for (size_t i = 0; i < course.prerequisites.size(); ++i) {
            cout << course.prerequisites[i];
            if (i < course.prerequisites.size() - 1) {
                cout << ", ";
            }
        }
        cout << endl;
    }
    cout << endl;
}

//Load a CSV file into a binary search tree
void loadCourses(string csvPath, BinarySearchTree* bst) {
    cout << "Loading CSV file " << csvPath << endl;
    ifstream file(csvPath);

    if (!file.is_open()) {
        throw runtime_error("Could not open file: " + csvPath);
    }

    string line;
	int rowNumber = 0;

    while (getline(file, line)) {  // Loop through every line in the file
        if (line.empty()) continue;

        stringstream ss(line);
        string token;
        vector<string> rowTokens;

		// Break CSV string apart by comma 
		while (getline(ss, token, ',')) {
			// Trim whitespace
			token = trimWhitespace(token); // Remove leading and trailing whitespace
			rowTokens.push_back(token); // Add the token to the rowTokens vector
		}
            
        // Validate minimum required fields are present (ID and Title)
		if (rowTokens.size() < 2) {  // Error if there are fewer than 2 tokens
            cerr << "File format incorrect. " + line << endl;
            continue;
        }

		Course course;  // Create a new Course object
		course.courseId = rowTokens[0];  // Set the course ID from the first token
		course.courseTitle = rowTokens[1];  // Set the course title from the second token

        // handle prerequisites
        for (size_t i = 2; i < rowTokens.size(); ++i) {
            if (!rowTokens[i].empty()) {
                course.prerequisites.push_back(rowTokens[i]);
            }
        }

        // Insert new course
        bst->Insert(course);
        rowNumber++;
    }

    file.close();
    cout << "Successfully loaded " << rowNumber << " courses." << endl;
}

//The main() method 
int main(int argc, char* argv[]) {

    // process command line arguments
    string csvPath;
    if (argc > 1) {
        csvPath = argv[1];
    }
    else {
        csvPath = "C:\\Users\\icnpr\\OneDrive\\Desktop\\SNHU\\CS 300 DSA Analysis and Design\\Wk7 CS300 ABCU Advising Program Input.csv";
    }
   
	// Define a binary search tree
	BinarySearchTree* bst = new BinarySearchTree();
	int choice = 0;
	string courseId;
	bool dataLoaded = false;  // check if data has been loaded

	// Display menu
	cout << "Welcome to the course planner." << endl;
	cout << "  1. Load Data Structure" << endl;
	cout << "  2. Print Course List" << endl;
	cout << "  3. Print Course" << endl;       
	cout << "  9. Exit" << endl;

	while (choice != 9) {
		cout << "\nWhat would you like to do? ";
		cin >> choice;

        switch (choice) {

        case 1:                 
            loadCourses(csvPath, bst);
            dataLoaded = true; //data loaded
            break;

        case 2:
			if (!dataLoaded) {
				cout << "No courses loaded. Please load the data structure first." << endl;
				break;
			}

			cout << "\nHere is a sample schedule:\n" << endl;
            bst->InOrder();
            break;

		case 3:
            if (!dataLoaded) {
                cout << "No courses loaded. Please load the data structure first." << endl;
                break;
            }

			{
				cout << "\nWhat course would you like to search for? " << endl;
				cin >> courseId;

				// Convert the courseId to uppercase for search
				transform(courseId.begin(), courseId.end(), courseId.begin(), [](unsigned char c) {
					return toupper(c);
				});

				Course course = bst->Search(courseId);
				if (!course.courseId.empty()) {
					displayCourse(course);
				} else {
					cout << "     Course " << courseId << " not found." << endl;
				}
			}
			break;

		case 9:			
			break;

		default:
			cout << choice << " is not a valid option." << endl;
			cout << "Please enter 1, 2, 3, or 9." << endl;
		}
	}

	cout << "\nThank you for using the course planner.  Good bye." << endl;
	delete bst;  // Clean up dynamically allocated tree
	return 0;
}
