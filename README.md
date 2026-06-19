# UDRMS — University Dormitory & Restaurant Management System

## ENSIA OOP Project 2025-2026

## Description
A C++ Qt Widgets application for managing university dormitories,
rooms, students, and restaurant menus.

## Features
- Student management (add, remove, assign to rooms)
- Dormitory and room management with capacity tracking
- Restaurant menu management per dormitory
- Data persistence via text files
- Modern dark gold GUI with background images

## OOP Concepts Used
- Inheritance: Student inherits from Person
- Composition: Dormitory owns Room and Restaurant
- Polymorphism: virtual display() overridden in Student
- Dynamic Memory Allocation: new/delete for Student objects
- Encapsulation: private attributes with public getters/setters

## Classes
Person, Student, Room, Restaurant, Dormitory, University, MainWindow

## Technologies
- Language: C++17
- GUI Framework: Qt 6 Widgets
- Storage: Text file (CSV format)

## How to Run
1. Open CMakeLists.txt or system.pro in Qt Creator
2. Press the green Run button
