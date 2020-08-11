The Krypton Project (Web Development Framework)
Work in progress [demo](https://www.nxte.tech/krypton_test)

![Krypton Badge](https://www.nxte.tech/static/assets/Krypton_Alternate.png)

This project is aimed at providing a web user with a native experience. By that I mean, the user should be able to load a Krypton application on a website and feel as if they
are using an application that has been installed to their phone. An application made with Krypton should be able to utilise the camera, file system, accelerometer, orientation
etc.

The target platform for this project is the web, but specifically the mobile subset of web browsers.


Section 1.0.
Glossary of Terms:
1. Event - “As in event driven architecture; An event can be defined as "a significant change in state".
2. Application - “The compiled C/C++ code that is in charge of displaying and managing the state of graphical UI elements and processing events.

Section 2.0
Functional Requirements:
1. Events are to be captured by a top layer invisible HTML element then passed to C++ using an event callback that calls a C++ function to append the new event to an event queue for processing by the application code. 
2. When an application is compiled with Krypton development tools there should be the accompanying HTML + Javascript code that sets up the environment for the application in the web browser.
3. The system must target Web GL 2.0 which is the C++/C equivalent to Open GL ES 2.0.


Section 2.1
Non-Functional Requirements:
1. Applications made with the Krypton framework should be as lightweight as possible.
2. The Krypton framework should be as lightweight as possible.
