import numpy as np
import pandas as pd
import matplotlib.pyplot as plt
from sklearn import datasets

from typing import List, Tuple, Union


class RegressionDataGenerator:

    @staticmethod
    def generate_data(number: int, number_outliers: int) -> Tuple[np.array]:
        x, y = datasets.make_regression(n_samples=number, n_features=1, n_targets=1, tail_strength=0.3, effective_rank=1, n_informative=1, noise=0.95, bias=100, random_state=5)
        
        for i in range(number_outliers):
            x[-i] = np.random.randint(2, 8) * x[-i]

        x = x * 20000

        y_min, y_max = np.min(y), np.max(y)
        y_normalized = (y - y_min) / (y_max - y_min)

        y = y_normalized * (2000 + 500) - 500

        outliers = np.random.choice(number, number_outliers, replace=False)

        return np.array(x), np.array(y)
    

    @staticmethod
    def save_data(x: np.array, y: np.array, filename: str):
        df = pd.DataFrame({"X": x.flatten(), "Y": y})
        df.to_csv(f"Python_Data_Generation/data/{filename}.csv", index=False)
            

if __name__ == "__main__":
    x, y = RegressionDataGenerator.generate_data(1000, 20)
    RegressionDataGenerator.save_data(x, y, "DataLength1000")