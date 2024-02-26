import React from 'react';
import './App.css';
import Navbar from './Components/Navbar';
import Interpreter from './Interpreter/Interpreter';
// import CodeRunner from './CodeRunner';

function App() {
  return (
    <div className="App">
      <Navbar />
      <div className="interpreter-container">
        <div className="button-area">
          <button>IMPORT</button>
          <button>SAVE</button>
          <button>PARSE</button>
          <button>DEBUG</button>
        </div>
        USER INPUT HERE:
        <div className="input-area">
          {/* Area for user input now implemented with a textarea */}
          <textarea 
            placeholder="Enter your code here" 
            style={{ width: '100%', height: '150px', marginTop: '10px', marginBottom: '10px' }}
          ></textarea>
        </div>
        OUTPUT:
        <div className="output-area">
          {/* Area to display output */}
        </div>
      </div>
      <Interpreter />
    </div>
  );
}

export default App;
