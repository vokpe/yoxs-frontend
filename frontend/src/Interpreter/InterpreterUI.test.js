// InterpreterUI.test.js
import React from 'react';
import { render, fireEvent, screen, waitFor } from '@testing-library/react';
import axios from 'axios';
import InterpreterUI from './InterpreterUI';

jest.mock('axios');

describe('InterpreterUI', () => {
  test('allows entering code and executes it', async () => {
    const userCode = 'print("hello world")';
    const mockOutput = 'hello world';
    axios.post.mockResolvedValueOnce({ data: { output: mockOutput } });

    render(<InterpreterUI />);
    const inputArea = screen.getByPlaceholderText(/enter your code here/i);
    const parseButton = screen.getByText(/parse/i);

    fireEvent.change(inputArea, { target: { value: userCode } });
    fireEvent.click(parseButton);

    await waitFor(() => expect(screen.getByText(mockOutput)).toBeInTheDocument());
  });

  test('displays error when code execution fails', async () => {
    axios.post.mockRejectedValueOnce(new Error('Network Error'));
    render(<InterpreterUI />);
    const parseButton = screen.getByText(/parse/i);
  
    fireEvent.click(parseButton);
  
    await waitFor(() => expect(screen.getByText(/Error executing code/i)).toBeInTheDocument());
  });
  
});
