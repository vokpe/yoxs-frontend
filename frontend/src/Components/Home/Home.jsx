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
      <button onClick={navigateToInterpreter} className="start-button">
        Start coding in Monkey now!
      </button>
    </div>
  );
}

export default Home;
