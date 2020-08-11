<div align="center">
  <p>
    The Krypton Project (Web Development Framework)
    Work in progress demo <a target="_blank" href="https://www.nxte.tech/krypton_test">here</a>
  </p>
  <img src="https://www.nxte.tech/static/assets/Krypton_Alternate.png" width="192" height="192" alt="Krypton Badge" />
  <h1>Krypton</h1>
</div>

<h1>Introduction</h1>
This project is aimed at providing a web user with a native experience. By that I mean, the user should be able to load a Krypton application on a website and feel as if they
are using an application that has been installed to their phone. An application made with Krypton should be able to utilise the camera, file system, accelerometer, orientation
etc.

The target platform for this project is the web, but specifically the mobile subset of web browsers.


<h1>Section 1.0</h1>
<h3>Glossary of Terms:</h3>
1. Event - “As in event driven architecture; An event can be defined as "a significant change in state".
2. Application - “The compiled C/C++ code that is in charge of displaying and managing the state of graphical UI elements and processing events.

<h1>Section 2.0</h1>
<h3>Functional Requirements:</h3>
1. Events are to be captured by a top layer invisible HTML element then passed to C++ using an event callback that calls a C++ function to append the new event to an event queue for processing by the application code. 
2. When an application is compiled with Krypton development tools there should be the accompanying HTML + Javascript code that sets up the environment for the application in the web browser.
3. The system must target Web GL 2.0 which is the C++/C equivalent to Open GL ES 2.0.


<h1>Section 2.1</h1>
<h3>Non-Functional Requirements:</h3>
1. Applications made with the Krypton framework should be as lightweight as possible.
2. The Krypton framework should be as lightweight as possible.
