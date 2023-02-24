##!/usr/bin/python
# -*- coding: utf-8 -*-

import glob
import math
from time import time
import pandas as pd
import numpy
import scipy.stats
import matplotlib.pyplot as plt
from tqdm import tqdm

# Global variables
data_dir = '/home/lawrence.fernandes/Documents/Workspace/stocks/S&P500 datasets/S&P500_500/'
path = data_dir + '*.csv'
instance_path = '/home/lawrence.fernandes/Documents/Workspace/stocks/S&P500 datasets/'
cvs_stocks = sorted(glob.glob(path))  # Using glob to get the path
stock_returns = []  # Stores the daily returns of all stocks


def read_files():
    """ Read the cvs files and calculate daily returns of the stocks """
    with open(instance_path + 'stock_tickers.txt', 'w+') as f:
        for i, stock in enumerate(cvs_stocks):
            # Save stock tickers dictionary
            f.write("%d %s\n" % (i, stock.split(data_dir)[1].replace('.csv', '')))
            df = pd.read_csv(stock)
            # Reads the Close column of the csv file
            close_column = df.Close
            # Daily return = close_price - previous_day_close_price
            # Enumerate with conditional inside the list comprehension:
            # daily_returns = [(x - close_column[i - 1])/close_column[i - 1] for i, x in enumerate(close_column) if i > 0]
            daily_returns = [x - close_column[i - 1] for i, x in enumerate(close_column) if i > 0]
            # Add the daily returns of a stock to the list of stock returns
            stock_returns.append(daily_returns)


def compute_correlation():
    """ Calculate the Pearson correlation between the stocks """
    with open(instance_path + 's&p500_500.txt', 'w+') as f:
        f.write("%d\n" % (len(stock_returns)))
        for i in tqdm(range(len(stock_returns))):
            for j in range(i + 1, len(stock_returns)):
                # Using SciPy to calculate Pearson's correlation coefficient:
                p = scipy.stats.pearsonr(stock_returns[i], stock_returns[j])[0]
                # Calculating the Pearson's Distance
                d = 1 - p
                # Calculating the so-called "ultra-metric"
                um = math.sqrt(2 * d)
                f.write("%d %d %s\n" % (i, j, um))


def correlation_matrix():
    """ Creates a correlation matrix of the stocks read """
    # Using NumPy to calculate Pearson's correlation coefficient:
    corr_matrix = numpy.corrcoef(stock_returns)
    # Using matplotlib to draw the correlation matrix
    plt.imshow(corr_matrix, interpolation='nearest')
    plt.colorbar()
    plt.show()


def main():
    start_time = time()
    print("Reading files...")
    read_files()
    print("Calculating the correlations...")
    compute_correlation()
    print("Printing the correlation matrix...")
    correlation_matrix()
    elapsed = time()-start_time
    print("\n--- Task done in %.2f seconds ---" % elapsed)


if __name__ == '__main__':
    main()
