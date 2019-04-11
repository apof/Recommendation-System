# Recommendation-System based-on LSH-Clustering

This is a 3-part project.

-->In the first part, I implemented an LSH data structure for eucledian and cosine metric, which is designed for 
efficient range search and nn (nearest neighbors) search in high dimensions. Another hybrid LSH alternative has been implemented. It is based on random-projection theory, as it stores similar items at the same tops of a hypercube. It requires
less space and remains as effective as the classic LSH. For the evaluation of the models, maximum fraction of true and
approximate nn is being computed.

-->In the second part, Ι implemented some different assumptions of high dimension vectors clustering. There are different choices for initialization(random,K-means++),assignment(K-means,LSH) and update(K-means,PAM) procedures. Models have been evaluated using silhouette metric. LSH (implemented in part1) has been used for assigning other elements to a new cluster's centroid or medoid, taking advantage of the fact that similar elements are likely to exist in the same bucket of LSH tables, within a given radius.

-->The last part concerns the implementation of a collaborative recommendation system for cryptocurrnecies, based on
twitter's users' tweets. Firstly, given a dataset of tokenized tweets and a sentiment analysis score lexicon, Ι  created 
every user's vector, based on cryptocoins that he/she has mentioned and his/her tweets' content. As the result of this preprocessing, I create a single vector for every user, in each position of which his/her total sentiment for the corresponding coin is stored . As far as the application is concerned, it predicts top unknown coins (cryptocoins that a given user has never commented before on his/her tweets) using clustering or LSH-range search. These two methods have been used in order to find quickly and efficiently users' nearest neighbors. 10-fold cross validation has been also implemented in order to test the perforamnce of the model. Mae (mean absolute error) has been utilized.
