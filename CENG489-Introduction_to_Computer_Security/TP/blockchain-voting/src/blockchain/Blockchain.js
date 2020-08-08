const { default: Block } = require('./Block');

export default class Blockchain{
    constructor(){
        this.chain = [];
        this.chain.push(this.createGenesisBlock());
        this.difficulty = 1;
        this.pendingVotes = [];
    }

    createGenesisBlock(){
        var date = new Date();
        return new Block(date.toISOString(), "Initial Data", "0");
    }

    getLatestBlock(){
        return this.chain[this.chain.length-1];
    }

    minePendingVotes(address){
        var date = new Date();
        let block = new Block(date.toISOString(), this.pendingVotes, this.getLatestBlock().hash);
        block.mineBlock(this.difficulty);
        console.log("Block successfully mined");
        this.chain.push(block);
        this.pendingVotes = [];
    }

    addVote(vote){
        console.log("vote added");
        if(!vote.fromVoter || !vote.toCandidate){
            throw new Error("Vote must include voter and candidate");
        }
        if(!vote.isValid()){
            throw new Error("Cannot add invalid vote to chain");
        }
        this.pendingVotes.push(vote);
    }

    getVotesOfCandidate(candidate){
        let votes = 0;
        for(const block of this.chain){
            console.log(block);
            for(const vote of block.votes){
                if(vote.toCandidate === candidate){
                    votes++;
                }
                if(vote.fromVoter === candidate){
                    votes--;
                }
            }
        }
        return votes;
    }

    isChainValid(){
        for(let i=1; i<this.chain.length; i++){
            const currentBlock = this.chain[i];
            const previousBlock = this.chain[i-1];
            if(!currentBlock.hasValidVotes()){
                return false;
            }
            if(currentBlock.hash !== currentBlock.calculateHash()){
                return false;
            }
            if(currentBlock.previousHash !== previousBlock.hash){
                return false;
            }
        }
        return true;
    }
}