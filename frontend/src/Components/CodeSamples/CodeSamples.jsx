import React, { useEffect, useState } from 'react';
import propTypes from 'prop-types';
import axios from 'axios';

function CodeSamples() {
    const [error, setError] = useState('');
    const [samples, setSamples] = useState([]);

    useEffect (
        () => {
            axios.get('http://localhost:8000/codesamples')
                .then((response) => {
                    const samplesObject = reponse.data.Data;
                    const keys = Object.keys(samplesObject);
                    const samplesArray = keys.map((key) => samplesObject[key]);
                    setSamples(samplesArray);
                })
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

function ErrorMessage({ message }) {
    return (
        <div className="error-message">
            {message}
        </div>
    );
}
ErrorMessage.propTypes = {
    message: propTypes.string.isRequired,
};

export default CodeSamples;