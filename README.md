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
- **main.c**: the testing main program

## how to run this program
First you should download the project `git clone https://github.com/JackWang822/BPR.git`
### windows (IDE)
take the visual studio as example  
first new an empty project and then add all source code into the project, including the file in the MF, BPR, GenaralStruct and Evaluation directory and the main.c in the root directory and build the program
### linux
after downloading the project, enter the root directory of this project `cd BPR`  
use make command to build the project(*make file is not in the directory*) `make`  

## extend
In the main program, it use the amazon dataset to test, for the consideration of the size of the dataset, the source dataset is not in the amazon_dataset directtory. And thus, if you want to run this program, you can download the amazon dataset from 
http://jmcauley.ucsd.edu/data/amazon/ by yourself. In the amazon dataset directory, there are some script to handle the source dataset. The *amazon_parse.py* script can 







  
    
      