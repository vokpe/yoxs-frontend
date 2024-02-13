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
        <div className="input-area">
          {/* Area for user input */}
        </div>
        <div className="button-area">
          {/* Buttons for import, save, etc. */}
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
