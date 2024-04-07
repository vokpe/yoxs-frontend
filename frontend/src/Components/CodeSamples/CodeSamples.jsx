import React, { useEffect, useState } from 'react';
import propTypes from 'prop-types';
import axios from 'axios';

function AddCodeSampleForm({visible, setError, fetchCodeSamples, cancel}) {
    const [name, setName] = useState();
    const [number, setNumber] = useState(0);

    const changeName = (event) => { setName(event.target.value);};
    const changeNumber = (event) => { setNumber(event.target.value);};

    const addCodeSample = () => {
        event.preventDefault();
        axios.post('http://localhost:8000/codesamples', { name: name, codeSampleID: number })
            .then(fetchCodeSamples)
            .catch(() => { setError('There was a problem adding the code form.'); });
    };

    if (!visible) return null;
    return (
        <form>
            <label htmlFor="name">
                Name
            </label>
            <input type="text" id="name" value={name} onChange={changeName} />
            <label htmlFor="number">
                ID
            </label>
            <input type="number" id="number" value={name} onChange={changeNumber} />
            <button type="button" onClick={cancel}>Cancel</button>
            <button type="submit" onClick={addCodeSample}>Submit</button>
        </form>
    );
}
AddGameForm.propTypes = {
    visible: propTypes.bool.isRequired,
    cancel: propTypes.func.isRequired,
    fetchCodeSamples: propTypes.func.isRequired,
    setError: propTypes.func.isRequired,
};

function FetchCodeSampleViaID() {
    const [number, setNumber] = useState({
        IDNumber: "",
        errors: {},
        loading: false,
});

    const changeNumber = (event) => { const {value} = (event.target); };
  
    const IDSubmit = (event) => {
        event.preventDefault();

        setNumber({
            ...number,
            loading: true,
        });

        setTimeout(() => {
            console.log(number);
            setNumber({
                ...number,
                loading: false,
            });
        }, 2000);
        

        if (validateForm()) {
            console.log(setNumber);
        } else {
            
        }
    };

    const validateForm = () => {
        const errors = {};
    
        if (!setNumber.IDNumber) {
          errors.setNumber = "ID Input is required";
        }
    
        setNumber((prevState) => ({ ...prevState, errors }));
    
        return Object.keys(errors).length === 0;
    };
    
    if (!visible) return null;
    return (
      <form onSubmit={IDSubmit}>
        <label>
            Enter Code Sample ID:
        </label>
        <input type="number" id="number" value={setNumber.IDNumber} onChange={changeNumber} />
        <button type="button" onClick={cancel}>Cancel</button>
        <input type="submit" value="Submit" disabled={setNumber.loading}/>
        {setNumber.loading && (
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

function FetchCodeSampleViaName() {
    const [name, setName] = useState({
        SampleName: "",
        errors: {},
        loading: false,
});

    const handleChange = (event) => {
        const { name, value } = event.target;
        setFormData((prevState) => ({ ...prevState, [name]: value }));
    };
  
    const NameSubmit = (event) => {
        event.preventDefault();

        setName({
            ...name,
            loading: true,
        });

        setTimeout(() => {
            console.log(name);
            setName({
                ...name,
                loading: false,
            });
        }, 2000);
        

        if (validateForm()) {
            console.log(setName);
        } else {
            
        }
    };

    const validateForm = () => {
        const errors = {};
    
        if (!setName.SampleName) {
          errors.setName = "Sample Name Input is required";
        }
    
        setNumber((prevState) => ({ ...prevState, errors }));
    
        return Object.keys(errors).length === 0;
    };
    
    
}
FetchCodeSampleViaID.propTypes = {
    visible: propTypes.bool.isRequired,
    cancel: propTypes.func.isRequired,
    setError: propTypes.func.isRequired,
};

function CodeSample({ codesample }) {
    const { name, codeSampleID } = codesample;
    return (
      <Link to={name}>
        <div className="codesample-container">
          <h2>{name}</h2>
          <p>
            Code Sample ID: {codeSampleID}
          </p>
        </div>
      </Link>
    );
  }
CodeSample.propTypes = {
    codesample: propTypes.shape({
        name: propTypes.string.isRequired,
        codeSampleID: propTypes.number.isRequired,
    }).isRequired,
};

function codesamplesObjectToArray({ Data }) {
    const keys = Object.keys(Data);
    const codesamples = keys.map((key) => Data[key]);
    return codesamples;
}

function CodeSamples() {
    const [error, setError] = useState('');
    const [samples, setSamples] = useState([]);
    const [addingCodeSample, setAddingCodeSample] = useState(false);

    const fetchCodeSamples = () => {
        axios.get('http://localhost:8000/codesamples')
                .then(({ data }) => setSamples(codesamplesObjectToArray(data)))
                .catch(() => setError('Something went wrong when retrieving the code samples'));
    };

    const showAddCodeSample = () => { setAddingCodeSample(true); };
    const hideAddCodeSample = () => { setAddingCodeSample(false); };

    useEffect (fetchCodeSamples, []);

    return (
        <div className="wrapper">
            <h1>
                Code Examples For Interpreter
            </h1>
            <button type="button" onClick={showAddCodeSample}>
                Add a Code Sample
            </button>
            {error && (
                <div className="error message">
                {error}
                </div>
            )}
            <AddCodeSampleForm visible={addingCodeSample} setError={setError} cancel={hideAddCodeSample} fetchCodeSamples={fetchCodeSamples}/>
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