// src/Interpreter/InterpreterUI.jsx
import React from 'react';
import Interpreter from './Interpreter';

const InterpreterUI = () => {
  // put UI state and other functions here

  return (
    <div className="interpreter-container">
      <div className="button-area">
        <button>IMPORT</button>
        <button>SAVE</button>
        <button>PARSE</button>
        <button>DEBUG</button>
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
  );
};

export default InterpreterUI;
