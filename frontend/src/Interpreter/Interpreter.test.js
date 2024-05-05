// Interpreter.test.js
import React from 'react';
import { render, screen, waitFor } from '@testing-library/react';
import axios from 'axios';
import Interpreter from './Interpreter';

jest.mock('axios');

describe('Interpreter', () => {
  test('displays loading message on mount', () => {
    axios.get.mockResolvedValueOnce({ data: {} });
    render(<Interpreter />);
    expect(screen.getByText(/loading data.../i)).toBeInTheDocument();
  });

  test('displays data when fetch is successful', async () => {
    const mockData = { message: 'Success' };
    axios.get.mockResolvedValueOnce({ data: mockData });
    render(<Interpreter />);
    await waitFor(() => expect(screen.getByText(/output:/i)).toBeInTheDocument());
    expect(screen.getByText(JSON.stringify(mockData, null, 2))).toBeInTheDocument();
  });

  test('displays error message when fetch fails', async () => {
    axios.get.mockRejectedValueOnce(new Error('Failed to fetch'));
    render(<Interpreter />);
    await waitFor(() => expect(screen.getByText(/failed to fetch data/i)).toBeInTheDocument());
  });
});
