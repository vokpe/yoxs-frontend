import React, { useEffect, useState } from 'react';
import propTypes from 'prop-types';
import axios from 'axios';
import { BACKEND_URL } from '../../constants';

// Helper function to fetch code content by name
function getCodeContentByName(name) {
    return axios.get(`${BACKEND_URL}/code/id_by_name`, { params: { name } })
        .then(response => {
            const codeSampleID = response.data; // Assuming the response contains the ID directly
            return axios.get(`${BACKEND_URL}/code/${codeSampleID}`);
        })
        .then(response => {
            return response.data.code; // Assuming 'code' contains the actual code
        })
        .catch(error => {
            console.error("Error fetching code content:", error);
            throw error;
        });
}

function AddCodeSampleForm({ visible, setError, fetchCodeSamples, cancel }) {
    const [name, setName] = useState('');
    const [code, setCode] = useState('');

    const changeName = (event) => {
        setName(event.target.value);
    };

    const changeCode = (event) => {
        setCode(event.target.value);
    };

    const addCodeSample = (event) => {
        event.preventDefault();
        axios.post(`${BACKEND_URL}/code/db_content`, { name, code })
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

function CodeSample({ name }) {
    const [codeContent, setCodeContent] = useState('');
    const [isLoading, setIsLoading] = useState(false);
    const [error, setError] = useState('');

    useEffect(() => {
        setIsLoading(true);
        getCodeContentByName(name)
            .then(code => {
                setCodeContent(code);
                setIsLoading(false);
            })
            .catch(error => {
                setError('Failed to fetch code sample');
                setIsLoading(false);
            });
    }, [name]);

    if (isLoading) return <p>Loading...</p>;
    if (error) return <p>{error}</p>;

    return (
        <div className="card">
            <div className="card-content">
                <h4>{name}</h4>
                <pre>{codeContent}</pre>
            </div>
        </div>
    );
}
CodeSample.propTypes = {
    name: propTypes.string.isRequired,
};

function CodeSamples() {
    const [samples, setSamples] = useState([]);
    const [error, setError] = useState('');
    const [addingCodeSample, setAddingCodeSample] = useState(false);

    const fetchCodeSamples = () => {
        axios.get(`${BACKEND_URL}/code/db_content`)
            .then(({ data }) => setSamples(data))
            .catch(() => setError('Something went wrong when retrieving the code samples'));
    };

    useEffect(() => {
        fetchCodeSamples();
    }, []);

    return (
        <div className="wrapper">
            <h1>Code Examples For Interpreter</h1>
            <div className="button-area">
                <button type="button" onClick={() => setAddingCodeSample(true)}>Add a Code Sample</button>
            </div>
            {error && <ErrorMessage message={error} />}
            <AddCodeSampleForm
                visible={addingCodeSample}
                setError={setError}
                cancel={() => setAddingCodeSample(false)}
                fetchCodeSamples={fetchCodeSamples}
            />
            <div className="card-container">
                {samples.map((sample) => (
                    <CodeSample key={sample.id} name={sample.name} />
                ))}
            </div>
        </div>
    );
}

function ErrorMessage({ message }) {
    return <div className="error-message">{message}</div>;
}
ErrorMessage.propTypes = {
    message: propTypes.string.isRequired,
};

export default CodeSamples;
