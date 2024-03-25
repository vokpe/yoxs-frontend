import React, { useEffect, useState } from 'react';
import axios from 'axios';
import { BACKEND_URL } from '../constants';

function Interpreter() {
    const [data, setData] = useState(null);
    const [isLoading, setIsLoading] = useState(false);
    const [error, setError] = useState(null);

    useEffect(() => {
        const fetchData = async () => {
            setIsLoading(true); // Start loading before fetching data
            try {
                const response = await axios.get(BACKEND_URL+ "/code/db_content");
                setData(response.data); // Set data from response
                setError(null); // Reset error state
            } catch (error) {
                console.error("There was an error fetching the data:", error);
                setData(null); // Reset data state
                setError("Failed to fetch data. Please try again later."); // Set error message
            } finally {
                setIsLoading(false); // Stop loading regardless of the outcome
            }
        };

        fetchData();
    }, []); // The empty array ensures this effect runs once on mount

    return (
        <div>
            {/* Loading state */}
            {isLoading && <p>Loading data...</p>}
            {/* Error state */}
            {!isLoading && error && <p>{error}</p>}
            {/* Data state */}
            {!isLoading && !error && data && (
                <div>
                    <h2>Output:</h2>
                    <pre style={{ whiteSpace: 'pre-wrap', wordBreak: 'break-word' }}>
                        {JSON.stringify(data, null, 2)}
                    </pre>
                </div>
            )}
            {/* No data or error */}
            {!isLoading && !error && !data && <p>No data available.</p>}
        </div>
    );
}

export default Interpreter;
