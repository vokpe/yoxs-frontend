import React from 'react';
import './App.css';
import { BrowserRouter as Router, Routes, Route } from 'react-router-dom'; 
import Navbar from './Components/Navbar';
import Interpreter from './Interpreter/Interpreter';
import About from './Components/About/About';
// import CodeRunner from './CodeRunner';

function App() {
  return (
    <Router> {/* Wrap the Routes component with Router */}
      <div className="App">
        <Navbar />
        <Routes> {/* Updates for React Router v6 */}
            <Route exact path="/" element={<Interpreter />} />
            <Route path="/about" element={<About />} />
            {/* Add more routes for additional components below */}
        </Routes>
        <div className="interpreter-container">
          <div className="button-area">
            {/* Adding aria-label for better accessibility */}
            <button aria-label="Import Code">IMPORT</button>
            <button aria-label="Save Code">SAVE</button>
            <button aria-label="Parse Code">PARSE</button>
            <button aria-label="Debug Code">DEBUG</button>
          </div>
          USER INPUT HERE:
          <div className="input-area">
            <textarea 
              placeholder="Enter your code here" 
              style={{ width: '100%', height: '150px', marginTop: '10px', marginBottom: '10px' }}
            ></textarea>
          </div>
          OUTPUT:
          <div className="output-area">
            <Interpreter />
          </div>
        </div>
      </div>
    </Router>
  );
}

export default App;
