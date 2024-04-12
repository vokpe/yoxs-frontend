import React, { useEffect, useState } from 'react';
import propTypes from 'prop-types';
import axios from 'axios';
import { Link } from 'react-router-dom';
import { BACKEND_URL } from '../../constants';


function AddCodeSampleForm({visible, setError, fetchCodeSamples, cancel}) {
    const [name, setName] = useState();
    const [number, setNumber] = useState(0);

    const changeName = (event) => { setName(event.target.value);};
    const changeNumber = (event) => { setNumber(event.target.value);};

    const addCodeSample = (event) => {
        event.preventDefault();
        axios.post(`${BACKEND_URL}/code/db_content`, { name: name, code: number })
            .then(fetchCodeSamples)
            .catch(() => { setError('There was a problem adding the code form.'); });
    };

    if (!visible) return null;
    return (
        <div className="interpreter-container">
          <form className="input-area">
            <div className="form-group">
              <label htmlFor="name" className="form-label">
                Name:
              </label>
              <input type="text" id="name" className="form-control" value={name} onChange={changeName} />
            </div>
            <div className="form-group">
              <label htmlFor="number" className="form-label">
                ID:
              </label>
              <input type="number" id="number" className="form-control" value={number} onChange={changeNumber} />
            </div>
            <div className="button-area">
              <button type="button" className="btn" onClick={cancel}>Cancel</button>
              <button type="submit" className="btn" onClick={addCodeSample}>Submit</button>
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
    const [number, setNumber] = useState({
        IDNumber: "",
        errors: {},
        loading: false,
});

    const changeNumber = (event) => {
        const { value } = event.target;
        setNumber((prevState) => ({
            ...prevState,
            IDNumber: value,
        }));
    };
  
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
            <button type="button" onClick={showAddCodeSample}>
                Add a Code Sample
            </button>
            {error && <ErrorMessage message={error} />}
            <AddCodeSampleForm
                visible={addingCodeSample}
                setError={setError}
                cancel={hideAddCodeSample}
                fetchCodeSamples={fetchCodeSamples}
            />
            <div className="codesample-list">
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