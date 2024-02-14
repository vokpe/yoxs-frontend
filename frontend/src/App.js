// App.js

import React from 'react';
import './App.css';
import Navbar from './Components/Navbar';
import Interpreter from './Interpreter/Interpreter';

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
        <div className="input-area">
          {/* Area for user input */}
        </div>
        <div className="output-area">
          {/* Area to display output */}
        </div>
      </div>
      <Interpreter />
    </div>
  );
}

export default App;
