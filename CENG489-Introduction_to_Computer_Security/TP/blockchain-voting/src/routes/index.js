import React from "react";
import { Switch, Route } from "react-router-dom";

import ResultsPage from "../pages/ResultsPage";
import VotePage from "../pages/VotePage";
import MainPage from "../pages/MainPage";

export default function Routes() {
  return (
    <Switch>
      <Route path="/" exact render={(props) => <MainPage {...props} /> } />
      <Route path="/vote" exact render={(props) => <VotePage {...props} /> } />
      <Route path="/results" exact render={(props) => <ResultsPage {...props} /> } />
    </Switch>
  );
}
