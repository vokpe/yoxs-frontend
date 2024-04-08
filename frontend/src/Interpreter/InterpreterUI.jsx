import React, { useState } from 'react';
import axios from 'axios';
import { BACKEND_URL } from '../constants';

const InterpreterUI = () => {
  const [userCode, setUserCode] = useState('');
  const [executionResult, setExecutionResult] = useState('');
  const [isLoading, setIsLoading] = useState(false);

  const handleUserCodeChange = (event) => {
    setUserCode(event.target.value);
    // Clear the executionResult as soon as the user starts typing again
    if(executionResult !== '') {
      setExecutionResult('');
    }
  };

  const executeUserCode = async () => {
    setIsLoading(true);
    try {
      const response = await axios.post(`${BACKEND_URL}/code/compile`, { code: userCode });
      console.log(response.data); // Logging the response for debugging
      setExecutionResult(response.data.output); // Adjust based on how your backend sends back the result
    } catch (error) {
      console.error("There was an error executing the code:", error);
      setExecutionResult("Error executing code. Please try again.");
    }
    setIsLoading(false);
  };

  return (
    <div className="interpreter-container">
      <div className="button-area">
        {/* Future functionality for these buttons */}
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
