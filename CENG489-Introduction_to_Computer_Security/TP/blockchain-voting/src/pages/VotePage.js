import React from "react";
import JDPLogo from "../assets/images/1.png";
import RPPLogo from "../assets/images/2.png";
import DPPLogo from "../assets/images/3.png";
import FPLogo from "../assets/images/4.png";
import "../App.css";
import Blockchain from "../blockchain/Blockchain";
import Vote from "../blockchain/Vote";

const EC = require('elliptic').ec;
const ec = new EC('secp256k1');

export default class VotePage extends React.Component{
    constructor(props){
        super(props);
        this.state={
            value:"",
            party:"",
            voteSystem: new Blockchain()
        };
        this.handleKeyChange = this.handleKeyChange.bind(this);
        this.handleOptionChange = this.handleOptionChange.bind(this);
        this.handleSubmit = this.handleSubmit.bind(this);
    }

    handleKeyChange(event) {
        this.setState({value: event.target.value});
    }

    handleOptionChange(event) {
        this.setState({party: event.target.value});
    }
    
    handleSubmit(event) {
        if(this.state.value === ""){
            alert("Please provide a key !");
            return;
        }
        if(this.state.party === ""){
            alert("Please choose a party !");
            return;
        }
        const myKey = ec.keyFromPrivate(this.state.value);
        const myVoterAddress = myKey.getPublic('hex');
        const vote = new Vote(myVoterAddress, this.state.party);
        vote.signVote(myKey);
        this.state.voteSystem.addVote(vote);
        this.state.voteSystem.minePendingVotes();
        const total = localStorage.getItem(this.state.party);
        if(total === null){
            localStorage.setItem(this.state.party,"1");
        }
        else{
            localStorage.setItem(this.state.party, JSON.stringify(parseInt(total)+1));
        }
        event.preventDefault();
    }

    render(){
        return(
            <div className="App-header" style={{textAlign:"center"}}>
                <form onSubmit={this.handleSubmit}>
                    <div style={{marginBottom:50}}>
                        Enter your private key :
                        <br />
                        <br />
                        <input style={{width:600, height:30, fontSize:18, textAlign:"center"}} type="text" value={this.state.value} onChange={this.handleKeyChange} />
                    </div>
                    <br />
                    Choose a party :
                    <div style={{display:"flex", flexDirection:"row"}}>
                        <label style={{display:"flex", flexDirection:"column",alignItems:"center", margin:50}}>
                            <img src={JDPLogo} width={200} style={{borderRadius:50}} alt="JDPLogo"/>
                            JDP
                            <input
                                type="radio"
                                value="JDP"
                                checked={this.state.party === "JDP"}
                                onChange={this.handleOptionChange}
                                style={{width:20}}
                            />
                        </label>
                        <label style={{display:"flex", flexDirection:"column",alignItems:"center", margin:50}}>
                            <img src={RPPLogo} width={200} style={{borderRadius:50}} alt="RPPLogo"/>
                            RPP
                            <input
                                type="radio"
                                value="RPP"
                                checked={this.state.party === "RPP"}
                                onChange={this.handleOptionChange}
                            />
                        </label>
                        <label style={{display:"flex", flexDirection:"column",alignItems:"center", margin:50}}>
                            <img src={DPPLogo} width={200} style={{borderRadius:50}} alt="DPPLogo"/>
                            DPP
                            <input
                                type="radio"
                                value="DPP"
                                checked={this.state.party === "DPP"}
                                onChange={this.handleOptionChange}
                            />
                        </label>
                        <label style={{display:"flex", flexDirection:"column",alignItems:"center", margin:50}}>
                            <img src={FPLogo} width={200} style={{borderRadius:50}} alt="FPLogo"/>
                            FP
                            <input
                                type="radio"
                                value="FP"
                                checked={this.state.party === "FP"}
                                onChange={this.handleOptionChange}
                            />
                        </label>
                    </div>
                    <input type="submit" value="Submit" />
                </form>
            </div>
        );
    }
}