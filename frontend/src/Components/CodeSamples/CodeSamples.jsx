import React, { useEffect, useState } from 'react';
import propTypes from 'prop-types';
import axios from 'axios';
import { Link } from 'react-router-dom';
import { BACKEND_URL } from '../../constants';


function AddCodeSampleForm({visible, setError, fetchCodeSamples, cancel}) {
    const [name, setName] = useState('');  // Initialize name as an empty string
    const [code, setCode] = useState('');  // Initialize code as an empty string

    const changeName = (event) => {
        setName(event.target.value);
    };

    const changeCode = (event) => {
        setCode(event.target.value);
    };

    const addCodeSample = (event) => {
        event.preventDefault();
        axios.post(`${BACKEND_URL}/code/db_content`, { name: name, code: code })
            .then(fetchCodeSamples)
            .catch(() => { setError('There was a problem adding the code sample.'); });
    };

    if (!visible) return null;

    return (
        <div className="interpreter-container">
            <form className="input-area" onSubmit={addCodeSample}>
                <div className="form-group">
                    <label htmlFor="name" className="form-label">
                        Name:
                    </label>
                    <input type="text" id="name" className="form-control" value={name} onChange={changeName} required />
                </div>
                <div className="form-group">
                    <label htmlFor="code" className="form-label">
                        Code:
                    </label>
                    <textarea id="code" className="form-control" value={code} onChange={changeCode} required rows="4" />
                </div>
                <div className="button-area">
                    <button type="button" className="btn" onClick={cancel}>Cancel</button>
                    <button type="submit" className="btn">Submit</button>
                </div>
            </form>
        </div>
    );
}
AddCodeSampleForm.propTypes = {
    visible: propTypes.bool.isRequired,
    cancel: propTypes.func.isRequired,
    fetchCodeSamples: propTypes.func.isRequired,
    setError: propTypes.func.isRequired,
};


function FetchCodeSampleViaID() {
    const [number, setID] = useState({
        IDNumber: 0,
        errors: {},
        loading: false,
});

    const changeID = (event) => {
        // Parse the value as a number before setting the state
        const newValue = parseInt(event.target.value, 10); 
        setID((prevState) => ({
            ...prevState,
            value: newValue, // Update the value of id
        }));
    };
  
    const IDSubmit = (event) => {
        event.preventDefault();

        setID((prevState) => ({
            ...prevState,
            loading: true,
        }));

        setTimeout(() => {
            console.log(number.value); // Log the value of id
            setID((prevState) => ({
                ...prevState,
                loading: false,
            }));
        }, 2000);
        

        if (validateForm()) {
            console.log(setID);
        }
    };

    const validateForm = () => {
        const errors = {};
    
        if (!setID.IDNumber) {
          errors.setID = "ID Input is required";
        }
    
        setID((prevState) => ({ ...prevState, errors }));
    
        return Object.keys(errors).length === 0;
    };

    const [visible, setVisible] = useState(true);

    const cancel = () => {
        setVisible(false);
    }
    
    if (!visible) return null;
    return (
      <form onSubmit={IDSubmit}>
        <label>
            Enter Code Sample ID:
        </label>
        <input type="number" id="number" value={setID.IDNumber} onChange={changeID} />
        <button type="button" onClick={cancel}>Cancel</button>
        <input type="submit" value="Submit" disabled={setID.loading}/>
        {setID.loading && (
            <div style={{ marginTop: 5, fontWeight: "bold" }}>Loading...</div>
        )}
      </form>
    );
}
FetchCodeSampleViaID.propTypes = {
    visible: propTypes.bool.isRequired,
    cancel: propTypes.func.isRequired,
    setError: propTypes.func.isRequired,
};

function CodeSample({ codesample }) {
    const { name, id: codeSampleID } = codesample;
    return (
      <div className="card">
        <div className="card-content">
          <Link to={`/code-samples/${codeSampleID}`} className="card-name">{name}</Link>
          <p>Code Sample ID: {codeSampleID}</p>
        </div>
      </div>
    );
  }
CodeSample.propTypes = {
    codesample: propTypes.shape({
        name: propTypes.string.isRequired,
        codeSampleID: propTypes.number.isRequired,
    }).isRequired,
};

function codesamplesObjectToArray(data) {
    return Object.keys(data).map((key) => ({
        ...data[key],
        id: key // is each key an ID?
    }));
}

function CodeSamples() {
    const [error, setError] = useState('');
    const [samples, setSamples] = useState([]);
    const [addingCodeSample, setAddingCodeSample] = useState(false);

    const fetchCodeSamples = () => {
        axios.get(`${BACKEND_URL}/code/db_content`)
                .then(({ data }) => setSamples(codesamplesObjectToArray(data)))
                .catch(() => setError('Something went wrong when retrieving the code samples'));
    };

    const showAddCodeSample = () => { setAddingCodeSample(true); };
    const hideAddCodeSample = () => { setAddingCodeSample(false); };

    useEffect(() => {
        fetchCodeSamples();
    }, []);

    return (
        <div className="wrapper">
            <h1>Code Examples For Interpreter</h1>
            <div className="button-area">
                <button type="button" onClick={showAddCodeSample}>
                Add a Code Sample
                </button>
            </div>
            {error && <ErrorMessage message={error} />}
            <AddCodeSampleForm
                visible={addingCodeSample}
                setError={setError}
                cancel={hideAddCodeSample}
                fetchCodeSamples={fetchCodeSamples}
            />
            <div className="card-container">
                {samples.map((sample, index) => (
                    <CodeSample key={index} codesample={sample} />
                ))}
            </div>
        </div>
    );
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