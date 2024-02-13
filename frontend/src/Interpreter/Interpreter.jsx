import React, { useEffect, useState } from 'react';
import propTypes from 'prop-types';
import axios from 'axios';
import { BACKEND_URL } from '../constants';

function Interpreter(props) {
    const [data, setData] = useState(null);
  
    useEffect(() => {
      axios.get(BACKEND_URL).then((response) => {
        setData(response.data);
      });
    }, []); // empty array ensures this effect runs once on mount
  
    return (
      <div>
        {/* Render UI here */}
        {data && <div>{/* Render data here */}</div>}
      </div>
    );
  }

export default Interpreter;