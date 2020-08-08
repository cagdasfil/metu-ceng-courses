import React from "react";
import { Router } from 'react-router-dom';

import history from './blockchain/history';
import Routes from './routes';

function App() {
  return (
    <Router history={history}>
      <Routes/>
    </Router>
  );
}

export default App;
