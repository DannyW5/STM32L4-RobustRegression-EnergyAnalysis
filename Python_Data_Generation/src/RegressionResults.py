import numpy as np
import pandas as pd
import matplotlib.pyplot as plt
from typing import Tuple
import seaborn as sns
from sklearn.linear_model import LinearRegression, TheilSenRegressor, HuberRegressor, RANSACRegressor
from sklearn.metrics import mean_absolute_error

class RegressionCalculator:

    @staticmethod
    def calculate_Theil_Sen(x: np.array, y: np.array) -> Tuple[np.array, np.array]:
        model = TheilSenRegressor()
        model.fit(x, y)
        y_estimate = model.predict(x.reshape(-1, 1))
        mae=mean_absolute_error(y, y_estimate)
        return x, y_estimate, model.coef_[0], model.intercept_, mae
        
    @staticmethod
    def calculate_OLS(x: np.array, y: np.array):
        model = LinearRegression()
        model.fit(x, y)
        y_estimate = model.predict(x.reshape(-1, 1))
        mae=mean_absolute_error(y, y_estimate)
        return x, y_estimate, model.coef_[0], model.intercept_, mae
    
    @staticmethod
    def calculate_RANSAC(x: np.array, y: np.array):
        model = RANSACRegressor()
        model.fit(x, y)
        y_estimate = model.predict(x.reshape(-1, 1))
        mae=mean_absolute_error(y, y_estimate)
        return x, y_estimate, model.estimator_.coef_[0], model.estimator_.intercept_, mae
    
    @staticmethod
    def calculate_Huber(x: np.array, y: np.array):
        model = HuberRegressor()
        model.fit(x, y)
        y_estimate = model.predict(x.reshape(-1, 1))
        mae=mean_absolute_error(y, y_estimate)
        return x, y_estimate, model.coef_[0], model.intercept_, mae

    @staticmethod
    def plot_regression(x, y, x_estimate, y_estimate, regressor):
        plt.figure()
        plt.scatter(x, y)
        plt.plot(x_estimate, y_estimate, color='r')
        plt.title(regressor)
        
def new_main():
    df = pd.read_csv("Python_Data_Generation/data/DataLength_200_50_Gauss.csv")
    x = df[["X"]].to_numpy()
    y = df["Y"].to_numpy()

    estimators = {
        "Theil-Sen": RegressionCalculator.calculate_Theil_Sen,
        "RANSAC": RegressionCalculator.calculate_RANSAC,
        "Huber": RegressionCalculator.calculate_Huber,
        "OLS": RegressionCalculator.calculate_OLS
    }

    # Typy szumów i liczba punktów z szumem
    noise_types = ["Gauss", "Poisson", "Uniform", "Laplace"]
    noise_points = [10, 20, 35, 50, 75, 100]
    results = np.zeros((len(noise_types), len(noise_points), len(estimators)))

    # Obliczamy MAE dla każdej kombinacji typu szumu i liczby punktów z szumem
    for i, noise_type in enumerate(noise_types):
        for j, num_noisy_samples in enumerate(noise_points):
            file_name = "DataLength_200_" + str(num_noisy_samples) + "_" + str(noise_type)
            df = pd.read_csv(f"Python_Data_Generation/data/{file_name}.csv")
            x = df[["X"]].to_numpy()
            y = df["Y"].to_numpy()
            
            for k, (name, estimator) in enumerate(estimators.items()):
                x_p, y_p, x_est, y_est, mae = estimator(x, y)
                results[i, j, k] = mae

    # # Rysowanie heatmap dla każdego estymatora
    # for k, (name, estimator) in enumerate(estimators.items()):
    #     plt.figure(figsize=(8, 6))
    #     sns.heatmap(results[:, :, k], annot=True, xticklabels=noise_points, yticklabels=noise_types, cmap="YlGnBu", cbar_kws={'label': 'MAE'})
    #     plt.title(f"Heatmap MAE dla {name}")
    #     plt.xlabel("Liczba punktów z szumem")
    #     plt.ylabel("Rodzaj szumu")
    #     plt.show()

    fig, axes = plt.subplots(2, 2, figsize=(14, 10))  # 2 wiersze, 2 kolumny (dla 4 estymatorów)

    for k, (name, estimator) in enumerate(estimators.items()):
        ax = axes[k//2, k%2]  # Umiejscowienie subplotu
        sns.heatmap(results[:, :, k], annot=True, xticklabels=noise_points, yticklabels=noise_types, cmap="YlGnBu", cbar_kws={'label': 'MAE'}, ax=ax)
        ax.set_title(f"Heatmap MAE dla {name}")
        ax.set_xlabel("Liczba punktów z szumem")
        ax.set_ylabel("Rodzaj szumu")

    plt.tight_layout()
    plt.show()

    # # Rysowanie wykresów liniowych
    # for i, noise_type in enumerate(noise_types):
    #     plt.figure(figsize=(10, 6))
    #     for k, (name, _) in enumerate(estimators.items()):
    #         plt.plot(noise_points, results[i, :, k], label=name, marker='o')

    #     plt.title(f"Porównanie estymatorów dla szumu: {noise_type}")
    #     plt.xlabel("Liczba punktów z szumem")
    #     plt.ylabel("MAE")
    #     plt.legend()
    #     plt.grid(True)
    #     plt.show()

    # # Rysowanie wszystkich wykresów liniowych w jednym figure jako subploty
    # fig, axes = plt.subplots(2, 2, figsize=(14, 10))  # 2 wiersze, 2 kolumny (dla 4 typów szumów)
    
    # for i, noise_type in enumerate(noise_types):
    #     ax = axes[i//2, i%2]  # Umiejscowienie subplotu
    #     for k, (name, _) in enumerate(estimators.items()):
    #         ax.plot(noise_points, results[i, :, k], label=name, marker='o')
        
    #     ax.set_title(f"Porównanie estymatorów dla szumu: {noise_type}")
    #     ax.set_xlabel("Liczba punktów z szumem")
    #     ax.set_ylabel("MAE")
    #     ax.legend()
    #     ax.grid(True)
    
    # plt.tight_layout()
    # plt.show()

    # Rysowanie wszystkich wykresów słupkowych w jednym figure jako subploty
    fig, axes = plt.subplots(2, 2, figsize=(14, 10))  # 2 wiersze, 2 kolumny (dla 4 typów szumów)
    width = 0.2  # Szerokość słupków

    # Kolory dla estymatorów

    for i, noise_type in enumerate(noise_types):
        ax = axes[i//2, i%2]  # Umiejscowienie subplotu
        x = np.arange(len(noise_points))  # Indeksy dla osi X (liczba punktów z szumem)
        
        for k, (name, _) in enumerate(estimators.items()):
            # Rysowanie słupków dla każdego estymatora, przesuwamy położenie słupka dla każdego estymatora
            ax.bar(x + k * width, results[i, :, k], width=width, label=name)
        
        ax.set_title(f"Porównanie estymatorów dla szumu: {noise_type}")
        ax.set_xlabel("Liczba punktów z szumem")
        ax.set_ylabel("MAE")
        ax.set_xticks(x + width * 1.5)  # Środek grupy słupków
        ax.set_xticklabels(noise_points)
        ax.grid()
        ax.legend()

    plt.tight_layout()
    plt.show()
        

if __name__ == "__main__":
    new_main()
    # df = pd.read_csv("Python_Data_Generation/data/DataLength_200_200_Gauss.csv")
    # x = df[["X"]].to_numpy()
    # y = df["Y"].to_numpy()

    # x_TheilSen, y_TheilSen, slope_TheilSen, intercept_TheilSen, mae  = RegressionCalculator.calculate_Theil_Sen(x, y)
    # x_RANSAC, y_RANSAC, slope_RANSAC, intercept_RANSAC, mae = RegressionCalculator.calculate_RANSAC(x, y)
    # x_Huber, y_Huber, slope_Huber, intercept_Huber, mae = RegressionCalculator.calculate_Huber(x, y)
    # x_OLS, y_OLS, slope_OLS, intercept_OLS, mae = RegressionCalculator.calculate_OLS(x, y)

    # print("========= Theil-Sen Regressor ===========")
    # print(f"Slope: {slope_TheilSen}, Intercept: {intercept_TheilSen}\n")

    # print("========= RANSAC Regressor ===========")
    # print(f"Slope: {slope_RANSAC}, Intercept: {intercept_RANSAC}\n")

    # print("========= Huber Regressor ===========")
    # print(f"Slope: {slope_Huber}, Intercept: {intercept_Huber}\n")

    # print("========= OLS Regressor ===========")
    # print(f"Slope: {slope_OLS}, Intercept: {intercept_OLS}\n")


    # RegressionCalculator.plot_regression(x,y, x_TheilSen, y_TheilSen, "Theil-Sen Regressor")
    # RegressionCalculator.plot_regression(x,y, x_RANSAC, y_RANSAC, "RANSAC Regressor")
    # RegressionCalculator.plot_regression(x,y, x_Huber, x_Huber, "Huber Regressor")
    # RegressionCalculator.plot_regression(x,y, x_OLS, y_OLS, "OLS Regressor")
    # plt.show()