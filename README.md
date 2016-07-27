# BPR algorithm
The procedure implements the essay [***BPR: Bayesian Personalized Ranking from Implicit Feedback*** ](https://www.ismll.uni-hildesheim.de/pub/pdfs/Rendle_et_al2009-Bayesian_Personalized_Ranking.pdf)
and uses [***amazon dataset***](http://jmcauley.ucsd.edu/data/amazon/)

## Project structure
- **Evaluation**: some evaluation function, such as the precision, recall, AUC calculation
- **GeneralSturct**: general data structure and some constant paramters defination
- **MF**: the implenmentation of matrix factorization algorithms
- **BPR**: the implenmentation of Bayesian Personalized Ranking algorithms
- **amazon_dataset**: the directory of amazon dataset and some python script to handle the source amazon dataset
- **yelp_dataset**: the directory of yelp dataset and some python script to handle the source amazon dataset