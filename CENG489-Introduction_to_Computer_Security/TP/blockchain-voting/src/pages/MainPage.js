import React from "react";

import voteLogo from "../assets/images/vote.png"
import resultsLogo from "../assets/images/results.png"

import "../App.css";

export default class MainPage extends React.Component{

  voteClickHandler(){
    this.props.history.push({pathname:"/vote"});
  }

  render(){
    return (
        <div className="App">
          <header className="App-header">
            <div style={{display:"flex"}}>
              <div style={{margin:100, display:"flex", flexDirection:"column"}} >
                <img src={voteLogo} width={300} style={{backgroundColor:"white", borderRadius:300, marginBottom:30}} alt="voteLogo"/>
                <a className="App-link" href="/vote">Vote</a>
              </div>
              <div style={{margin:100, display:"flex", flexDirection:"column"}} >
                <img src={resultsLogo} width={300} style={{backgroundColor:"white", borderRadius:300, marginBottom:30}} alt="resultsLogo"/>
                <a className="App-link" href="/results">Results</a>
              </div>
            </div>
          </header>
        </div>
    );
  }
}