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

        return np.array(x).astype(np.float32), np.array(y).astype(np.float32)
    
    # @staticmethod
    # def generate_data_with_noise(number: int, number_outliers: int, noise_type: str) -> Tuple[np.array]:
    #     # x, y = datasets.make_regression(n_samples=number, n_features=1, n_targets=1, tail_strength=0.3, effective_rank=1, n_informative=1, noise=0.95, bias=100, random_state=5)
        
    #     x = np.linspace(-2000, 2000, number)
    #     y= np.random.randint(10, 9999)*x / 100

    #     if noise_type == "gauss":
    #         noise = np.random.normal(0, np.random.randint(10,100), size=y.shape)  # Szum Gaussa (normalny)
    #     elif noise_type == "poisson":
    #         noise = np.random.poisson(lam=np.random.randint(10,100), size=y.shape)  # Szum Poissona
    #     elif noise_type == "uniform":
    #         noise = np.random.uniform(-np.random.randint(10,100), np.random.randint(10,100), size=y.shape)  # Szum jednostajny (uniform)
    #     elif noise_type == "laplace":
    #         noise = np.random.laplace(0, np.random.randint(10,100), size=y.shape)  # Szum Laplace'a
    #     else:
    #         raise ValueError("Nieznany typ szumu. Dostępne: gauss, poisson, uniform, laplace")
        
    #     noise = noise * 1000
    #     y = y + noise
    #     y = y /10000

    #     for i in range(number_outliers):
    #         x[-i] = np.random.randint(0, 80)/100 * x[-i]
    #         y[-i] = np.random.randint(0, 80)/50 * y[-i]

    #     return np.array(x).astype(np.float32), np.array(y).astype(np.float32)
    
    @staticmethod
    def generate_data_with_noise(number: int, num_noisy_samples: int, noise_type: str) -> Tuple[np.array]:
        # x, y = datasets.make_regression(n_samples=number, n_features=1, n_targets=1, tail_strength=0.3, effective_rank=1, n_informative=1, noise=0.95, bias=100, random_state=5)
        
        x = np.linspace(-2000, 2000, number)
        y = np.random.randint(10, 9999) * x

        # Wybieramy losowe indeksy próbek, do których będzie dodany szum
        noisy_indices = np.random.choice(number, size=num_noisy_samples, replace=False)

        # Generowanie szumu tylko dla losowo wybranych próbek
        noise = np.zeros(y.shape)  # Inicjalizacja szumu zerami
        
        if noise_type == "gauss":
            noise_values = np.random.normal(0, np.random.randint(10, 10000), size=num_noisy_samples)  # Szum Gaussa
        elif noise_type == "poisson":
            noise_values = np.random.poisson(lam=np.random.randint(10, 10000), size=num_noisy_samples)  # Szum Poissona
        elif noise_type == "uniform":
            noise_values = np.random.uniform(-np.random.randint(10, 10000), np.random.randint(10, 10000), size=num_noisy_samples)  # Szum jednostajny
        elif noise_type == "laplace":
            noise_values = np.random.laplace(0, np.random.randint(10, 10000), size=num_noisy_samples)  # Szum Laplace'a
        else:
            raise ValueError("Nieznany typ szumu. Dostępne: gauss, poisson, uniform, laplace")
        
        # Dodaj szum tylko do wybranych próbek
        noise[noisy_indices] = noise_values * 1000
        y = y + noise
        y = y / 10000

        # # Dodawanie outlierów (punktów odstających)
        # for i in range(number_outliers):
        #     x[-i] = np.random.randint(0, 80) / 100 * x[-i]
        #     y[-i] = np.random.randint(0, 80) / 50 * y[-i]

        return np.array(x).astype(np.float32), np.array(y).astype(np.float32)

    @staticmethod
    def save_data(x: np.array, y: np.array, filename: str):
        df = pd.DataFrame({"X": x.flatten(), "Y": y})
        df.to_csv(f"Python_Data_Generation/data/{filename}.csv", index=False)
            

if __name__ == "__main__":
    x, y = RegressionDataGenerator.generate_data_with_noise(200, 200, "gauss")
    RegressionDataGenerator.save_data(x, y, "DataLength_200_200_Gauss")
    x, y = RegressionDataGenerator.generate_data_with_noise(200, 200, "poisson")
    RegressionDataGenerator.save_data(x, y, "DataLength_200_200_Poisson")
    x, y = RegressionDataGenerator.generate_data_with_noise(200, 200, "uniform")
    RegressionDataGenerator.save_data(x, y, "DataLength_200_200_Uniform")
    x, y = RegressionDataGenerator.generate_data_with_noise(200, 200, "laplace")
    RegressionDataGenerator.save_data(x, y, "DataLength_200_200_Laplace")