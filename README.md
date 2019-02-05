# Recommendation-System-based-on-Lsh-and-Clustering-Projects
Recommendation System Based On LSH and Clustering

This is a 3-part project.

-->At the first part I implement an LSH data structure for eucledian and cosine metric which is designed for 
efficient range search and nn (nearest neighbors) search in high dimensions. Another hybrid lsh alternative has been implemented.
It is based on random-projection theory as its stores similar items to the same tops of a hypercube. It requires
less space and remains as effective as classic lsh. For the evaluation of the models maximum fraction of true and
approximate nn is being computed.

-->At the second Ι implement different assumptions of high dimension vectors clustering. There are different choices
for initialization(random,K-means++),assignment(K-means,LSH) and update(K-means,PAM). Models have been evaluated using
silhouette metric. Lsh (implemented in part1) has been used for assigning to a new cluster centroid or medoid other elements
taking advantage of the fact that similar elements are possible to exist in the same bucket of lsh tables within a given radius.

-->The last part concerns the implementation of a collaborative recommendation system for cryptocurrnecies based on
tweeter's users tweets. Firstly given a dataset of tokenized tweets and a sentiment analysis score lexicon Ι  create 
every users vector based on cryptocoins that has been mentioned and his tweets content. As the result of this preproccessing i
create for every user a single vector in each position of it it stored his total sentiment for the corresponding coin. As far as the application is concerned it predicts top unknown coins (cryptocoins that a given user has never comment before on his tweets) using clustering or lsh-range search. These two methods have been used in order to find fast and efficient users nearest neighbors. 10-fold cross validation has been also implmented in order to test the accuracy of the model.
