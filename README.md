# BPR algorithm
The procedure implements the essay [***BPR: Bayesian Personalized Ranking from Implicit Feedback*** ](https://www.ismll.uni-hildesheim.de/pub/pdfs/Rendle_et_al2009-Bayesian_Personalized_Ranking.pdf)
and uses [***amazon dataset***](http://jmcauley.ucsd.edu/data/amazon/) to test

## Project structure
- **Evaluation**: some evaluation function, such as the precision, recall, AUC calculation
- **GeneralSturct**: general data structure and some constant paramters defination
- **MF**: the implenmentation of matrix factorization algorithms
- **BPR**: the implenmentation of Bayesian Personalized Ranking algorithms
- **amazon_dataset**: the directory of amazon dataset and some python script to handle the source amazon dataset
- **yelp_dataset**: the directory of yelp dataset and some python script to handle the source amazon dataset
- **main.c**: the testing main program

## How to run this program
- **Download the source code**

  `git clone https://github.com/Kejie-Wang/BPR.git & cd BPR ` 

- **Build**

  `mkdir build & cd build`

  `cmake ..`

  `make`

- **Generate the dataset and test**

  `cd ../yelp_dataset`

  `python yelp_choose.py yelp`

  `cd ../build`

  `./bpr ../yelp_dataset/yelp`

### Extend
The main program in the code is test by yelp dataset and you can simply follow the instructions above to test.

I also use the Amazon dataset to test and offer some sricpts. For the consideration of the size of the dataset, the source dataset is not in the amazon_dataset directtory. And thus, if you want to run this program, you can download the amazon dataset from 
http://jmcauley.ucsd.edu/data/amazon/ by yourself. 
In the amazon dataset directory, there are some script to handle the source dataset.   

- ***amazon_parse.py:*** this script can parse the json.gz file into json file. Run the amazon_parse.py as `python amazon_parse.py xxx.json.jz`  
- ***amazon_pre_simple:*** this script can extract the useful information for the program which simplily extracts the user identify, product identify and rating information and saves as the .data file. Run it as `python amazon_pre_simple.py xxx.json`  
- ***amazon_pre_mixture:*** this script can mix the many source json dataset into a file which can run by the program. The script will read 10000 users (can change in the script) for each input file and all their related rating record. Run it as `python amazon_pre_mixture.py xxx1.json xxx2.json xxx3.json` 

And run `python amazon_choose.py amazon` to generate the dataset and run bpr to test.

If you want to use another dataset, you should save the dataset as the format *`uid vid rating`* and one line per item and the dataset name must end with _rating.data and provide the prefix of dataname to test bpr program. (e.g. `./bpr ../yelp_dataset/yelp`)

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
    - **lambdaUV**: the coefficient of the regularization    

The program use top-N recommendation to recommd and it will run as top-2, 5, 10, 20, 40, 80, 100 and if you want to change the N value, you can change in the main.c program.

## Contact
This project is maintained by WANG Kejie <wang_kejie@foxmail.com> and if you have some problems or find some bugs in the procedure, please send me the email.








​      