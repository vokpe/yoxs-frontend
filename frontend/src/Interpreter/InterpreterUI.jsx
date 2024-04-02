import React, { useState } from 'react';
import axios from 'axios';
import { BACKEND_URL } from '../constants'; // Ensure this import is correct

const InterpreterUI = () => {
  const [userCode, setUserCode] = useState('');
  const [executionResult, setExecutionResult] = useState('');
  const [isLoading, setIsLoading] = useState(false);

  const handleUserCodeChange = (event) => {
    setUserCode(event.target.value);
  };

  const executeUserCode = async () => {
    setIsLoading(true);
    try {
      // Assuming the backend expects a POST request with the user code
      const response = await axios.post(`${BACKEND_URL}/code/compile`, { code: userCode });
      console.log(response.data)
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
        {/* Assuming these buttons will have functionality added later */}
        <button>IMPORT</button>
        <button>SAVE</button>
        <button onClick={executeUserCode}>PARSE</button> {/* Updated to execute code */}
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
        {isLoading ? (
          <p>Loading...</p>
        ) : (
          <pre style={{ whiteSpace: 'pre-wrap', wordBreak: 'break-word' }}>{executionResult}</pre>
        )}
      </div>
    </div>
  );
};

export default InterpreterUI;
