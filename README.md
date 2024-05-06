# YOXS Frontend

Frontend for our C++ interpreter. The interpreter utilizes a 3 stage (Lexical Analysis, Abstract-Syntax-Tree Construction & Parsing, and Evaluation) process to translate plaintext into evaluated code.

See our server-API code here
+ [YOXS Server Repo](https://github.com/DubbleA/yoxs)

## Get Started

In the project directory 
```bash 
cd frontend/src
```
you can run:

### `npm start`

Runs the app in the development mode.\
Open [http://localhost:3000](http://localhost:3000) to view it in your browser.

The page will reload when you make changes.\
You may also see any lint errors in the console.

### `npm test`

and then run `a` to "run all tests" in watch usage

Launches the test runner in the interactive watch mode.\
See the section about [running tests](https://facebook.github.io/create-react-app/docs/running-tests) for more information.

## Interpreter
The Interpreter feature enables you to write, parse, and debug custom code within the application.

### Using the Interpreter
+ Access the Interpreter tab via the navbar.
+ In the text area labeled "USER INPUT HERE", type or paste your code.
+ Click "PARSE" to execute your code using the backend interpreter.
+ The output will appear in the "OUTPUT" section below the input field.

## Code Samples

Users can access the Code Samples feature via the Code Samples tab in the navbar.

### Adding A Code Sample

Add a code sample to the database by inputting a template name and a code template into the "Add a Code Sample" form and clicking the submit button. 

### Using A Code Sample

You can test our interpreter by copying code samples from the provided templates and pasting them into the user input section of our interpreter.
