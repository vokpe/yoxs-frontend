import React from 'react';
import './App.css';
import { BrowserRouter as Router, Route, Routes } from 'react-router-dom'; 
import Navbar from './Components/Navbar';
import Interpreter from './Interpreter/Interpreter';
import About from './Components/About/About';
//import CodeRunner from './CodeRunner';

function App() {
  return (
    <div className="App">
      <Navbar />
      <Routes> {/* renders only the first Route that matches the location */}
          <Route exact path="/" component={Interpreter} />
          <Route path="/about" component={About} />
          {/* add more routes for additional components below */}
        </Routes>
      <div className="interpreter-container">
        <div className="button-area">
          <button>IMPORT</button>
          <button>SAVE</button>
          <button>PARSE</button>
          <button>DEBUG</button>
        </div>
        USER INPUT HERE:
        <div className="input-area">
          {/* user input area */}
          <textarea 
            placeholder="Enter your code here" 
            style={{ width: '100%', height: '150px', marginTop: '10px', marginBottom: '10px' }}
          ></textarea>
        </div>
        OUTPUT:
        <div className="output-area">
          {/* output area */}
          <Interpreter />
        </div>
      </div>
    </div>
  );
}

export default App;
