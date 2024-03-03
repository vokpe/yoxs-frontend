import React from 'react';
import './About.css';

const About = () => {
  return (
    <div className="about-container">
      <h1>About YOXS</h1>
      <p>YOXS is a fully functional interpreter of the Monkey language, created with a focus on demonstrating the inner workings of an interpreter. It utilizes a 3-stage process consisting of Lexical Analysis, AST Construction & Parsing, and Evaluation to translate plaintext into evaluated code.</p>
      <p>This project aims to provide an educational tool for understanding the fundamentals of programming language interpreters. It includes an API server with over 10 REST endpoints and a front-end visualizer, allowing users to input their own code or select from sample programs hosted in MongoDB.</p>
      <p>For more information on our project and to view our progress and goals, please visit our project repository at <a href="https://github.com/DubbleA/yoxs" target="_blank" rel="noopener noreferrer">https://github.com/DubbleA/yoxs</a>.</p>
    </div>
  );
};

export default About;
