import React from "react";
import "../App.css";

export default class ResultsPage extends React.Component{
    constructor(props){
        super(props);
        this.state={
            JDP: localStorage.getItem("JDP") || "0",
            RPP: localStorage.getItem("RPP") || "0",
            DPP: localStorage.getItem("DPP") || "0",
            FP: localStorage.getItem("FP") || "0",
            total: 0
        }
        this.state.total = parseInt(this.state.JDP)+
        parseInt(this.state.RPP)+
        parseInt(this.state.DPP)+
        parseInt(this.state.FP);
    }

    render(){
        return(
            <div className="App-header">
                <div style={{alignItems:"flex-start"}}>
                    <div style={{display:"flex"}}>
                        JDP <div style={{
                            width:(parseInt(this.state.JDP)/this.state.total).toFixed(4)*1000,
                            backgroundColor:"yellow",
                            color:"black",
                            marginLeft:10
                            }}>
                                {(parseInt(this.state.JDP)/this.state.total).toFixed(4)*100}</div>
                    </div>
                    <br />
                    <div style={{display:"flex"}}>
                        RPP <div style={{
                            width:(parseInt(this.state.RPP)/this.state.total).toFixed(4)*1000,
                            backgroundColor:"red",
                            color:"white",
                            marginLeft:10
                            }}>
                                {(parseInt(this.state.RPP)/this.state.total).toFixed(4)*100}
                            </div>
                    </div>
                    <br />
                    <div style={{display:"flex"}}>
                        DPP <div style={{
                            width:(parseInt(this.state.DPP)/this.state.total).toFixed(4)*1000,
                            backgroundColor:"blue",
                            color:"white",
                            marginLeft:10
                            }}>
                                {(parseInt(this.state.DPP)/this.state.total).toFixed(4)*100}
                            </div>
                    </div>
                    <br />
                    <div style={{display:"flex"}}>
                        FP <div style={{
                            width:(parseInt(this.state.FP)/this.state.total).toFixed(4)*1000,
                            backgroundColor:"green",
                            color:"white",
                            marginLeft:10
                            }}>
                                {(parseInt(this.state.FP)/this.state.total).toFixed(4)*100}
                            </div>
                    </div>
                </div>
            </div>
        );
    }
}