import React, { useEffect, useState } from 'react';
import axios from 'axios';
import { BACKEND_URL } from '../constants';

function Interpreter() {
    const [helloMessage, setHelloMessage] = useState(null);

    useEffect(() => {
      // Fetch data from the backend on component mount
      axios.get(`${process.env.REACT_APP_BACKEND_URL}/code/hello`).then((response) => {
        setHelloMessage(response.data);
      }).catch((error) => {
        // Handling errors here
        console.error("There was an error fetching the data:", error);
        setHelloMessage({ error: "Failed to fetch data." }); 
      });
    }, []); // The empty array ensures this effect runs once on mount

    return (
      <div>
        {/* Conditional rendering based on fetched data */}
        {helloMessage ? (
          <div>
            <pre style={{ whiteSpace: 'pre-wrap', wordBreak: 'break-word' }}>
              {JSON.stringify(helloMessage, null, 2)}
            </pre>
          </div>
        ) : (
          <p>Loading data...</p>
        )}
      </div>
    );
}

export default Interpreter;
