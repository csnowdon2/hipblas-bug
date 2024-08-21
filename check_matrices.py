import pandas as pd 

A = pd.read_csv("A.csv", header=None)
B = pd.read_csv("B.csv", header=None)
C = pd.read_csv("C.csv", header=None)

diff = abs(C - (A @ B.T))
print("norm(C - AB^T) =", diff.max().max())
