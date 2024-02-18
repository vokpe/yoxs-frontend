import React, { useEffect, useState } from 'react';
import propTypes from 'prop-types';
import axios from 'axios';

function CodeSamples() {
    const [error, setError] = useState();

    useEffect (
        () => {
            axios.get('http://localhost:8000/codesamples')
                .then()
                .catch(() => {setError('Something went wrong'); });
        },
        [],
    );

    return (
        <div className="wrapper">
            <h1>
                Code Examples For Interpreter
            </h1>
            {error && (
                <div className="error message">
                {error}
                </div>
            )}
        </div>
    )
}

export default CodeSamples;