// src/Components/Home/Home.jsx
import React from 'react';
import { useNavigate } from 'react-router-dom';

function Home() {
  const navigate = useNavigate(); // Hook for navigation

  const navigateToInterpreter = () => {
    navigate('/interpreter'); // Navigate to the Interpreter page
  };

  return (
    <div className="welcome-container">
      <h1>Welcome to Our Application</h1>
      <p>This is the place where you can run and test your code. Enjoy using our interpreter!</p>
      <button onClick={navigateToInterpreter} style={buttonStyle}>
        Start coding in Monkey now!
      </button>
    </div>
  );
}

const buttonStyle = {
  padding: '10px 20px',
  backgroundColor: '#4CAF50',
  color: 'white',
  border: 'none',
  borderRadius: '5px',
  cursor: 'pointer',
  fontSize: '16px',
  marginTop: '20px',
};

export default Home;
