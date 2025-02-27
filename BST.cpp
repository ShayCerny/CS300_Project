#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>

using namespace std;

//Course Class to represent a course with its number, title, and prerequisites 
class Course {
public:
	int number; 
	string title;
	vector<int> prerequisites;

	//constructor to initialize course object with a number, title, and optional prerequisites
	Course(int num, string t, vector<int> prereqs = {}) :number(num), title(t), prerequisites(prereqs) {}
};


//Node structure to represent a node in the Binary Search Tree
struct Node {
	Course data;
	Node* left;
	Node* right;

	//Constructor to initialize a node with course data and null child pointers
	Node(Course course) :data(course), left(nullptr), right(nullptr) {}
};

//BST Class to manage the courses in a binary tree structure
class BST {
public:
	Node* root; // pointer to the root of the BST

	//Constructor to initialize BST with a null root
	BST() { root = nullptr; }


	//Recursive insert function to insert a course into the BST
	Node* insert(Node* node, Course* course) {

		//if the current node is null, create a new node and return it
		if (node == nullptr) {
			return new Node(*course);
		}

		// Recursively insert the course into the left or right subtree based on the course number
		if (course->number < node->data.number) {
			node->left = insert(node->left, course);
		}
		else if (course->number > node->data.number) {
			node->right = insert(node->right, course);
		}

		//return the current node after insertion
		return node;
	}

	//public insert function that starts the insertion process from the root
	void insert(Course* course) {
		root = insert(root, course);
	}

	//Recursive search function to find a course by its number
	Course* search(Node* node, int number) {
		if (node == nullptr) return nullptr; // if the root node is null, return null
		
		if (node->data.number == number) return &node->data; // if the course number matches return the course data
		
		//recursively search in the left or right subtree
		return (number < node->data.number) ? search(node->left, number) : search(node->right, number); 
	}
	
	//public search function that starts the search process from the root
	Course* search(int number) {
		return search(root, number);
	}

	//Recursive in-order traversal to print all courses in numerical order
	void inorder(Node* node) {
		if (node == nullptr) return; // Base case: if root node is null, do nothing
		inorder(node->left); // travel the left subtree
		cout << "CSCI" << node->data.number << ", " << node->data.title << "\n";// print course information
		inorder(node->right);//travel the right subtree
	}

	//public in-order traversal function to start the traversal from root node
	void inorder() {
		inorder(root);
		cout << endl;
	}
};

//Function to simplify conversion to int
int stripAndConvert(const string& str) {
	string numberStr = str.substr(4); // Remove the "CSCI" prefix
	return stoi(numberStr); // convert to int and return
}

// Function to load courses from CSV file
void loadCoursesFromCSV(const string& filename, BST& bst) {
	ifstream file(filename); // open the file for reading
	string line;

	//check if the file was opened successfully
	if (file.is_open()) {
		while (getline(file, line)) { //Read each line of the file
			stringstream ss(line); //Create a stringstream to parse the line
			string courseNumberStr, title; 
			getline(ss, courseNumberStr, ','); //read the course number
			getline(ss, title, ','); // read the course title

			//Strip the "CSCI" prefixx and convert to int
			int courseNumber = stripAndConvert(courseNumberStr);

			//Vector to hold the prerequisites for the current course
			vector<int> prerequisites;
			string prerequisite;

			//Read the prerequisites for the course
			while (getline(ss, prerequisite, ',')) {
				if (!prerequisite.empty()) {
					//Strip the "CSCI" prefix and convert to int
					prerequisites.push_back(stripAndConvert(prerequisite));
				}
			}
			//Create a new Course object and insert it into the BST
			Course* course = new Course(courseNumber, title, prerequisites);
			bst.insert(course);
		}
		file.close(); //Close the file after reading
		cout << "Data structure loaded successfully.\n\n";
	}
	else {
		//Error message if the file couldnt be opened
		cout << "Failed to open file." << endl;
	}
	
}


//Main Function to interact with the user and perform operations on the course planner
int main() {
	BST tree; //Create a BST to hold the courses
	string filename;

	int choice;
	do {
		//Display menu options to the user
		cout << "Welcome to the course planner.\n\n";
		cout << "1. Load Data Structure.\n";
		cout << "2. Print Course List.\n";
		cout << "3. Print Course.\n";
		cout << "9. Exit\n\n";
		cout << "What would you like to do? ";
		cin >> choice;
		cout << "\n";

		switch (choice) {
		case 1:
			//Load Courses from a CSV
			cout << "Which file do you want to load from?\n";
			cin >> filename;
			cout << "\n";
			loadCoursesFromCSV(filename, tree); // Load courses into the BST
			break;
		case 2:
			// Print all courses in numerical order
			tree.inorder();
			break;
		case 3: {
			//Search and print details of a specific course
			cout << "What course do you want to know about? ";
			string courseNumber;
			cin >> courseNumber;
			cout << "\n";

			if (courseNumber.substr(0, 4) == "CSCI") {
				int num = stoi(courseNumber.substr(4)); // Extract numeric part
				Course* foundCourse = tree.search(num); // search for course in the BST
				if (foundCourse) {
					//Print the course details if found
					cout << "CSCI" << foundCourse->number << ", " << foundCourse->title << "\n";
					if (!foundCourse->prerequisites.empty()) {
						cout << "Prerequisites: ";
						for (int prereq : foundCourse->prerequisites) {
							cout << "CSCI" << prereq << " ";
						}
						cout << "\n\n";
					}
				}
				else {
					cout << "Course not found.\n\n";
				}
			}
			else {
				//Handle invalid course format input
				cout << "Invalid course format. Use CSCI### format.\n\n";
			}
			break;
		}
		case 9:
			//Exit the program
			cout << "Thank you for using the course planner!\n\n";
			break;
		default:
			//handle invalid menu option
			cout << choice << " is not a valid option.\n\n";
			break;
		}
	} while (choice != 9);

	return 0;
}