import React, { useEffect, useState } from 'react';
import axios from 'axios';
import { BACKEND_URL } from '../constants';

function Interpreter() {
    const [data, setData] = useState(null);

    useEffect(() => {
      // Fetch data from the backend on component mount
      axios.get(BACKEND_URL).then((response) => {
        setData(response.data);
      }).catch((error) => {
        // Handling errors here
        console.error("There was an error fetching the data:", error);
        setData({ error: "Failed to fetch data." }); 
      });
    }, []); // The empty array ensures this effect runs once on mount

    return (
      <div>
        {/* Conditional rendering based on fetched data */}
        {data ? (
          <div>
            <pre style={{ whiteSpace: 'pre-wrap', wordBreak: 'break-word' }}>
              {JSON.stringify(data, null, 2)}
            </pre>
          </div>
        ) : (
          <p>Loading data...</p>
        )}
      </div>
    );
}

export default Interpreter;
