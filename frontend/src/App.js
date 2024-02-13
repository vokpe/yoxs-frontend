// App.js

import React from 'react';
import './App.css';
import Navbar from './Components/Navbar';
import Interpreter from './Interpreter/Interpreter';

function App() {
  return (
    <div className="App">
      <Navbar />
      <Interpreter />
    </div>
  );
}

export default App;
