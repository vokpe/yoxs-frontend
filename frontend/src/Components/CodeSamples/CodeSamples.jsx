import React, { useEffect, useState } from 'react';
import propTypes from 'prop-types';
import axios from 'axios';

function AddCodeSampleForm(fetchCodeSamples) {
    const [name, setName] = useState();
    const [number, setNumber] = useState(0);

    const changeName = (event) => { setName(event.target.value);};
    const changeNumber = (event) => { setNumber(event.target.value);};

    const addCodeSample = () => {
        event.preventDefault();
        axios.post('http://localhost:8000/codesamples', { name: name, codeSampleID: number })
            .then(() => {
                fetchCodeSamples();
            })
            .catch(() => { setError('There was a problem adding the code form.'); });
    };

    return (
        <form>
            <label htmlFor="name">
                Name
            </label>
            <input type="text" id="name" value={name} onChange={changeName} />
            <label htmlFor="number">
                Name
            </label>
            <input type="number" id="number" value={name} onChange={changeName} />
            <button type="submit" onClick={addCodeSample}>Submit</button>
        </form>
    );
}


function CodeSamples() {
    const [error, setError] = useState('');
    const [samples, setSamples] = useState([]);

    const fetchCodeSamples = () => {
        axios.get('http://localhost:8000/codesamples')
                .then((response) => {
                    const samplesObject = reponse.data.Data;
                    const keys = Object.keys(samplesObject);
                    const samplesArray = keys.map((key) => samplesObject[key]);
                    setSamples(samplesArray);
                })
                .catch(() => {setError('Something went wrong'); });
    };

    useEffect (
        fetchCodeSamples,
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
            <AddCodeSampleForm setError={setError} fetchCodeSamples={fetchCodeSamples}/>
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