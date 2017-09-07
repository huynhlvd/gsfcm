# gsfcm
**A Generalized Spatial Fuzzy C-Means Clustering Algorithm (GSFCM)**

    Version 1.1,  Jan. 24, 2009
    Implementations by Huynh Van Luong, Email: huynhlvd@gmail.com,
    Embedded System Lab, University of Ulsan, Korea
    
   `Please see the file` [LICENSE](https://github.com/huynhlvd/gsfcm/blob/master/LICENSE.md) `for the full text of the license.`

Please cite this publication:

`Huynh Van Luong and Jong Myon Kim, "`[A Generalized Spatial Fuzzy C-Means Algorithm for Medical Image Segmentation](http://ieeexplore.ieee.org/xpls/abs_all.jsp?arnumber=5276878)`," in IEEE International Conference on Fuzzy Systems (FUZZ-IEEE 2009), Jeju, Korea, Aug. 2009.`
             
**_Solving the problem_**

<img src="https://latex.codecogs.com/svg.latex?\small&space;J_{m}(U,&space;V)=\sum_{i=1}^{c}\sum_{k=1}^{n}u_{ik}^{m}d^{2}(x_{k},v_{i})," title="\small J_{m}(U, V)=\sum_{i=1}^{c}\sum_{k=1}^{n}u_{ik}^{m}d^{2}(x_{k},v_{i})," />

where
- <img src="https://latex.codecogs.com/svg.latex?d(x_{k},v_{i})" title="d(x_{k},v_{i})" />: The distance between the pixel <img src="https://latex.codecogs.com/svg.latex?x_{k}" title="x_{k}" /> and centroid <img src="https://latex.codecogs.com/svg.latex?v_{i}" title="v_{i}" />, 
- <img src="https://latex.codecogs.com/svg.latex?\sum_{i=1}^{c}u_{ik}=1" title="\sum_{i=1}^{c}u_{ik}=1" />: The constraint and the degree of fuzzification _m ≥ 1_.

**_Source code files:_** 
- [gsfcm.c](https://github.com/huynhlvd/gsfcm/blob/master/gsfcm.c): The function for GSFCM

**_Example usage:_** 
- [exampleUsage](https://github.com/huynhlvd/gsfcm/blob/master/exampleUsage): A folder consists of one example of GSFCM usage
