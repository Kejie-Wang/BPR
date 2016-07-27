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

## How to run this program
First you should download the project `git clone https://github.com/JackWang822/BPR.git`
### windows (IDE)
take the visual studio as example  
first new an empty project and then add all source code into the project, including the file in the MF, BPR, GenaralStruct and Evaluation directory and the main.c in the root directory and build the program
### linux
after downloading the project, enter the root directory of this project `cd BPR`  
use make command to build the project(***makefile is not in the directory***) `make`  

### extend
In the main program, it use the amazon dataset to test, for the consideration of the size of the dataset, the source dataset is not in the amazon_dataset directtory. And thus, if you want to run this program, you can download the amazon dataset from 
http://jmcauley.ucsd.edu/data/amazon/ by yourself. 
In the amazon dataset directory, there are some script to handle the source dataset.   
- ***amazon_parse.py:*** this script can parse the json.gz file into json file. Run the amazon_parse.py as `python amazon_parse.py xxx.json.jz`  
- ***amazon_pre_simple:*** this script can extract the useful information for the program which simplily extracts the user identify, product identify and rating information and saves as the .data file. Run it as `python amazon_pre_simple.py xxx.json`  
- ***amazon_pre_mixture:*** this script can mix the many source json dataset into a file which can run by the program. The script will read 10000 users (can change in the script) for each input file and all their related rating record. Run it as `python amazon_pre_mixture.py xxx1.json xxx2.json xxx3.json`   

If you want to use another dataset, you should save the dataset as the format *`uid vid rating`* and one line per item and the dataset name must end with _rating.data. And then in the main program you should change the dataname path.  

If you want to test the different parameters, you can change the parameter in the function ***setMFParams*** and ***setBPRParams*** in the ***main.c*** program. You can change the value of each parameter and run the program.
- MF Parameters: (***M = U * V***):
    - **alpha**: learning rate of the Gradient descent algorithms
	- **Learn_loop**: maximum learning loop of the Gradient descent
	- **lambdaUV**: the coefficient of the regularization of the matrix U and V 
	- **lambdaBias**: the coefficient of the regularization of the Bias 
	- **sw_reg**: whether use regularization 0->not, else-> yes
	- **sw_bias**: whether use bias 0->not, else-> yes
- BPR Parameters:
    - **alpha**: learning rate of the Gradient descent algorithms
    - ***lambdaUV**: the coefficient of the regularization    

The program use top-N recommendation to recommd and it will run as top-2, 5, 10, 20, 40, 80, 100 and if you want to change the N value, you can change in the main.c program.







  
    
      