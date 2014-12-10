BlackJack
=========

BlackJack machine learning program, meant to somewhat emulate a neural learning system, ends up running at about 48% win rate, pretty much expected, only takes ~90-100k runs to reach that high. Fairly Efficient. If you want to see some previous weighting results, you can view here 
https://docs.google.com/spreadsheet/ccc?key=0ApPDNFarOvUGdGhxZEtackdVWk0wcGVocHdFdWdCX0E&usp=sharing
1=stand, 2=hit, 4=double, 3=split
17stand, no double splits, 6 decks, pretty basic stuff. 
As you can see by seeing from the previous runs, I had a bit of a hard time getting the lower half to be clean in regards to the stands and doubles, mostly to pslit on two 8's, but im happy with the results. 

Interesting tidbit. If you clone the repo and change the decksize global from 6 to 1,2, or 3, the algorithm actually accounts for what is shown on the board. Its not huge, but there is a very noticeable increase in percentage, which shows that there is a very very small card counting sort of deal here, since it knows that there isnt a few cards in the deck since they are on the board, pretty cool in my opinion.
