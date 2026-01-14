# ✈️ Smart Travel and Tourism Planner

**Smart Travel and Tourism Planner** is a console-based C++ application designed to simplify and organize the entire process of trip planning.  
The project demonstrates the **practical implementation of Data Structures and Algorithms (DSA)** by solving real-world travel management problems such as flight searching, cost comparison, booking management, passenger check-ins, and hotel recommendations.

This project was developed as part of the **Data Structures Lab (15B17CI371)** under Project-Based Learning.

---

## 📌 Problem Statement
Planning a trip in real life involves checking flights, comparing prices, managing bookings, handling check-ins, and finding suitable hotels. Performing all these tasks manually is time-consuming and confusing.

This project aims to provide a **single, efficient, and user-friendly system** that organizes travel data and helps users make quick and informed travel decisions.

---

## 🎯 Objectives
- To create a user-friendly travel planning system using Data Structures  
- To store and manage flight information efficiently using graphs  
- To allow users to quickly find the cheapest direct flight  
- To maintain booking history with undo functionality  
- To manage passenger check-ins using a queue  
- To recommend top-rated hotels using a Binary Search Tree  
- To implement file handling for permanent data storage  

---

## 🚀 Features
- Add and store flights with distance, time, and cost  
- Display all direct flights between two cities  
- Automatically find the cheapest direct flight  
- Book flight tickets with undo support  
- Display all bookings made by the user  
- Passenger check-in and processing system  
- Add hotels and list top-rated hotels  
- Automatic saving and loading of data using files  

---

## 🛠️ Technologies Used
- **Language:** C++  
- **Paradigm:** Data Structures and Algorithms  
- **Interface:** Command Line Interface (CLI)  

---

## 🧩 Data Structures Used

| Data Structure | Purpose |
|---------------|---------|
| Graph (Adjacency List) | Store cities and flight routes |
| Priority Queue | Find the cheapest flight (best deal) |
| Hash Map | Fast flight lookup by source–destination |
| Stack | Undo the most recent booking |
| Queue | Passenger check-in (FIFO order) |
| Binary Search Tree (BST) | Store and sort hotels by rating |
| File Handling | Persistent storage of data |

---

## 🏗️ System Design

### 🔹 Flight Management
- Flights are stored using a **Graph (Adjacency List)**  
- Direct flights are searched efficiently  
- A **Priority Queue** identifies the cheapest available flight  

### 🔹 Booking Management
- Bookings are stored in a list  
- A **Stack** is used to undo the most recent booking  

### 🔹 Passenger Check-In
- Implemented using a **Queue**  
- Passengers are processed in the order of arrival (FIFO)  

### 🔹 Hotel Management
- Hotels are stored in a **Binary Search Tree**  
- Inorder traversal displays hotels sorted by rating  

---

## 🔄 Flow of the Application
1. Load saved flights, bookings, and hotels from files  
2. Display the main menu to the user  
3. Perform selected operations (flight search, booking, check-in, hotel management)  
4. Update data structures dynamically  
5. Save all updated data back to files before exit  

---

## 💾 Data Persistence
The application uses **file handling** to permanently store:
- Flight details  
- Booking records  
- Hotel information  

This ensures that all data is retained even after the program is closed.

---

## ▶️ How to Run the Project
1. Clone the repository:
```bash
git clone https://github.com/your-username/Smart-Travel-And-Tourism-Planner.git
