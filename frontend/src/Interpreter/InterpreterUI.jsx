import React, { useState } from 'react';
import axios from 'axios';
import { BACKEND_URL } from '../constants';

const InterpreterUI = () => {
  const [userCode, setUserCode] = useState('');
  const [executionResult, setExecutionResult] = useState('');
  const [isLoading, setIsLoading] = useState(false);

  const handleUserCodeChange = (event) => {
    setUserCode(event.target.value);
    if (executionResult !== '') {
      setExecutionResult('');
    }
  };

  const logExecutionDetails = (message) => {
    console.log(message);  // This could be enhanced to send logs to a backend
  };

  const executeUserCode = async () => {
    setIsLoading(true);
    logExecutionDetails("Execution started with user code: " + userCode);
    try {
      const response = await axios.post(`${BACKEND_URL}/code/compile`, { code: userCode });
      setExecutionResult(response.data.output);
      logExecutionDetails("Execution successful: " + response.data.output);
    } catch (error) {
      console.error("There was an error executing the code:", error);
      setExecutionResult("Error executing code. Please try again.");
      logExecutionDetails("Execution error: " + error.message);
    }
    setIsLoading(false);
    logExecutionDetails("Execution finished.");
  };

  return (
    <div className="interpreter-container">
      <div className="button-area">
        <button>IMPORT</button>
        <button>SAVE</button>
        <button onClick={executeUserCode}>PARSE</button>
        <button>DEBUG</button>
      </div>
      USER INPUT HERE:
      <div className="input-area">
        <textarea 
          value={userCode}
          onChange={handleUserCodeChange}
          placeholder="Enter your code here" 
          style={{ width: '100%', height: '150px', marginTop: '10px', marginBottom: '10px' }}
        ></textarea>
      </div>
      OUTPUT:
      <div className="output-area">
        {isLoading ? <p>Loading...</p> : <pre style={{ whiteSpace: 'pre-wrap', wordBreak: 'break-word' }}>{executionResult}</pre>}
      </div>
    </div>
  );
};

export default InterpreterUI;
