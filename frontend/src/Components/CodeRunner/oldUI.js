// CodeRunner.js
import React, { useState, useEffect } from 'react';
// Import PrismJS for syntax highlighting
import Prism from 'prismjs';
import 'prismjs/themes/prism-okaidia.css'; // 

const CodeRunner = () => {
    const [codeSamples, setCodeSamples] = useState([]); 
    const [selectedCode, setSelectedCode] = useState('');

    useEffect(() => {
        // Initialize Prism for syntax highlighting
        Prism.highlightAll();
    }, [selectedCode]); // Rerun Prism highlight when selectedCode changes

    useEffect(() => {
        // Initialize Ace Editor
        const editor = ace.edit("editor");
        editor.setTheme("ace/theme/monokai");
        editor.session.setMode("ace/mode/javascript");
        editor.setValue(selectedCode, -1); // Update the editor with the selected code

        // Update hidden input on editor change
        editor.session.on('change', () => {
            const code = editor.getValue();
            setSelectedCode(code); // Update selectedCode state with the editor's content
        });
    }, []); // This effect should run once on component mount

    // Handler for changing selected code sample
    const handleSelectExample = (e) => {
        const code = e.target.value;
        setSelectedCode(code); // Update the selected code
    };

    // Render component
    return (
        <div className="module">
            <h1>Run Your Code</h1>
            
            <label htmlFor="exampleCode">Choose an example program:</label>
            <select id="exampleCode" name="exampleCode" onChange={handleSelectExample}>
                <option value="none" selected>Select a sample</option>
                {codeSamples.map(sample => (
                    <option key={sample.name} value={sample.code}>{sample.name}</option>
                ))}
            </select>

            <div id="editor" className="language-javascript"></div>

            <form method="POST">
                <input type="hidden" name="code" value={selectedCode} />
                <button type="submit">Run Code</button>
            </form>

            <div className="output">
                <h2>Output</h2>
                <pre className="language-javascript">{/* Output here */}</pre>
            </div>
        </div>
    );
};

export default CodeRunner;

