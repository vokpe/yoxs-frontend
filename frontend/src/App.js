import React from 'react';
import './App.css';
import { Route, Routes } from 'react-router-dom'; 
import Navbar from './Components/Navbar';
import Interpreter from './Interpreter/Interpreter';
import About from './Components/About/About';
//import CodeRunner from './CodeRunner';

function TestPage() {
  return (
    <div>
      <h1>This is a Test Page</h1>
      <p>Welcome to the test page. If you're seeing this, routing is working!</p>
    </div>
  );
}


function App() {
  return (
    <div className="App">
      <Navbar />
      <Routes> {/* renders only the first Route that matches the location */}
          <Route path="/" element={<Interpreter />} />
          <Route path="/about" element={<About />} />
          <Route path="/test" element={<TestPage />} />
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
