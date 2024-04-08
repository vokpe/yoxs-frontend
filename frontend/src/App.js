import React from 'react';
import './App.css';
import { Route, Routes } from 'react-router-dom'; 
import Navbar from './Components/Navbar';
import InterpreterUI from './Interpreter/InterpreterUI';
import About from './Components/About/About';
import Home from './Components/Home/Home';
import CodeSamples from './Components/CodeSamples/CodeSamples.jsx';
//import CodeRunner from './CodeRunner';


function App() {
  return (
    <div className="App">
      <Navbar />
      <Routes>
        <Route path="/" element={<Home />} /> 
        <Route path="/interpreter" element={<InterpreterUI />} /> 
        <Route path="/about" element={<About />} />
        <Route path="/codesamples" element={<CodeSamples />} />
        {/* add more routes below */}
      </Routes>
    </div>
  );
}

export default App;
