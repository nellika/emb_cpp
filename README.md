# 2021/2 C++ for embedded systems
>Labs... labs... labs

## 01 Introduction
> Getting familiar with the tools

- [x] Creating a histogram from .txt file
- [x] Recall hash map
- [x] Experiment with different containers (unordered/ordered map, multimap, vector)
- [x] Optimize search with STL functions 
- [x] Generate a set of random numbers with a predefined distribution

## 02 Class design
> Getting familiar with external libraries, design a class in our own namespace

- [x] Understand the math background behind PW linear and cubic spline interpolation algos
- [x] Install and use external library Eigen
- [x] Design simple class for both pw linear and cubic spline
- [x] Structure code into separate namespace, header file
- [x] using gnuplot to visualize data points

## 03 Qt basics
> Discover Qt and the Creator IDE

- [x] Understand what is a Barycentric coordinate system and its relation to the Cartesian coordinates
- [x] Discover some interesting drawing related QT methods
- [x] Understand how buttons > actions > functions are connected
- [x] draw pretty, colorful triangle instead of a dull chess board in 'pixel space'

![Colorful triangle](/03/triangle.png?raw=true "Lab 03 - Triangle")


## 04 QT - Derived Class
> Derive our own img class based on QImage

- [x] Write code for a derived class
- [x] Reusing code from Lab2 and Lab3
- [x] Use methods of the base class
- [x] Discover function objects
- [x] Draw pretty spline lines and color background based on f(x)

![Colorful splines](/04/derived_spline.png?raw=true "Lab 04 - Derived spline")

## 05 QT - Mandelbrot set
> Draw Mandelbrot set, learn about the usage of complex numbers and multi-threaded programming

- [x] Create a 2-color Mandelbrot set
- [x] Use the Spline class from lab 2 to color the background
- [x] Improving execution time with multi-threaded computing
- [x] Add user interactions to zoom and pane (yes, it is ugly to create a new class every time...)
- [x] Extend calculation to toggle Julia set

![Colorful Mandelbrot](/05/mandelbrot.png?raw=true "Lab 05 - Mandelbrot set")
