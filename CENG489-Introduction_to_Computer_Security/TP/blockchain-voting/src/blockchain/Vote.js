const SHA256 = require("crypto-js/sha256");
const EC = require('elliptic').ec;
const ec = new EC('secp256k1');

export default class Vote{
    constructor(fromVoter, toCandidate){
        this.fromVoter = fromVoter;
        this.toCandidate = toCandidate;
        this.timestamp = Date.now();
    }

    calculateHash(){
        return SHA256(this.fromVoter+this.toCandidate+this.timestamp).toString();
    }

    signVote(signingKey){
        console.log("vote signed");
        if(signingKey.getPublic('hex') !== this.fromVoter){
            throw new Error('You cannot vote for others!');
        }
        const hashVote = this.calculateHash();
        const sig = signingKey.sign(hashVote, 'base64');
        this.signature = sig.toDER('hex');
    }

    isValid(){
        if (!this.signature || this.signature.length === 0) {
            throw new Error('No signature in this vote');
        }

        const publicKey = ec.keyFromPublic(this.fromVoter, 'hex');
        return publicKey.verify(this.calculateHash(), this.signature);
        
    }

}