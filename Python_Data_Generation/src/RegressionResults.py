import numpy as np
import pandas as pd
import matplotlib.pyplot as plt
from typing import Tuple
from sklearn.linear_model import LinearRegression, TheilSenRegressor, HuberRegressor, RANSACRegressor

class RegressionCalculator:

    @staticmethod
    def calculate_Theil_Sen(x: np.array, y: np.array) -> Tuple[np.array, np.array]:
        model = TheilSenRegressor()
        model.fit(x, y)
        x_estimate = np.arange(x.min(), x.max(), 0.1)
        y_estimate = model.predict(x_estimate.reshape(-1, 1))
        return x_estimate, y_estimate, model.coef_[0], model.intercept_
        
    @staticmethod
    def calculate_OLS(x: np.array, y: np.array):
        model = LinearRegression()
        model.fit(x, y)
        x_estimate = np.arange(x.min(), x.max(), 0.1)
        y_estimate = model.predict(x_estimate.reshape(-1, 1))
        return x_estimate, y_estimate, model.coef_[0], model.intercept_
    
    @staticmethod
    def calculate_RANSAC(x: np.array, y: np.array):
        model = RANSACRegressor()
        model.fit(x, y)
        x_estimate = np.arange(x.min(), x.max(), 0.1)
        y_estimate = model.predict(x_estimate.reshape(-1, 1))
        return x_estimate, y_estimate, model.estimator_.coef_[0], model.estimator_.intercept_
    
    @staticmethod
    def calculate_Huber(x: np.array, y: np.array):
        model = HuberRegressor()
        model.fit(x, y)
        x_estimate = np.arange(x.min(), x.max(), 0.1)
        y_estimate = model.predict(x_estimate.reshape(-1, 1))
        return x_estimate, y_estimate, model.coef_[0], model.intercept_

    @staticmethod
    def plot_regression(x, y, x_estimate, y_estimate, regressor):
        plt.figure()
        plt.scatter(x, y)
        plt.plot(x_estimate, y_estimate, color='r')
        plt.title(regressor)
        
    

if __name__ == "__main__":
    df = pd.read_csv("Python_Data_Generation/data/DataLength100_Gauss1.csv")
    x = df[["X"]].to_numpy()
    y = df["Y"].to_numpy()

    x_TheilSen, y_TheilSen, slope_TheilSen, intercept_TheilSen  = RegressionCalculator.calculate_Theil_Sen(x, y)
    x_RANSAC, y_RANSAC, slope_RANSAC, intercept_RANSAC = RegressionCalculator.calculate_RANSAC(x, y)
    x_Huber, y_Huber, slope_Huber, intercept_Huber = RegressionCalculator.calculate_Huber(x, y)
    x_OLS, y_OLS, slope_OLS, intercept_OLS = RegressionCalculator.calculate_OLS(x, y)

    print("========= Theil-Sen Regressor ===========")
    print(f"Slope: {slope_TheilSen}, Intercept: {intercept_TheilSen}\n")

    print("========= RANSAC Regressor ===========")
    print(f"Slope: {slope_RANSAC}, Intercept: {intercept_RANSAC}\n")

    print("========= Huber Regressor ===========")
    print(f"Slope: {slope_Huber}, Intercept: {intercept_Huber}\n")

    print("========= OLS Regressor ===========")
    print(f"Slope: {slope_OLS}, Intercept: {intercept_OLS}\n")


    RegressionCalculator.plot_regression(x,y, x_TheilSen, y_TheilSen, "Theil-Sen Regressor")
    RegressionCalculator.plot_regression(x,y, x_RANSAC, y_RANSAC, "RANSAC Regressor")
    RegressionCalculator.plot_regression(x,y, x_Huber, x_Huber, "Huber Regressor")
    RegressionCalculator.plot_regression(x,y, x_OLS, y_OLS, "OLS Regressor")
    plt.show()