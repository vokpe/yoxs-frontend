import React, { useEffect, useState } from 'react';
import axios from 'axios';

function Interpreter() {
    const [helloMessage, setHelloMessage] = useState(null);
    const [isLoading, setIsLoading] = useState(false);
    const [error, setError] = useState('');

    useEffect(() => {
        setIsLoading(true); // Start loading
        axios.get(`${BACKEND_URL}/code/hello`)
            .then((response) => {
                setHelloMessage(response.data);
                setIsLoading(false); // Stop loading once data is received
            })
            .catch((error) => {
                console.error("There was an error fetching the data:", error);
                setError("Failed to fetch data. Please try again later.");
                setIsLoading(false); // Stop loading on error
            });
    }, []); // The empty array ensures this effect runs once on mount

    return (
        <div>
            {isLoading ? (
                <p>Loading data...</p>
            ) : error ? (
                <p>{error}</p>
            ) : helloMessage ? (
                <div>
                    <pre style={{ whiteSpace: 'pre-wrap', wordBreak: 'break-word' }}>
                        {JSON.stringify(helloMessage, null, 2)}
                    </pre>
                </div>
            ) : (
                <p>No data to display.</p>
            )}
        </div>
    );
}

export default Interpreter;
