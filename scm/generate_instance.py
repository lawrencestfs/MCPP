##!/usr/bin/python
# -*- coding: utf-8 -*-

# File Name: generate_instance.py
# Author: Lawrence Fernandes
# Copyright (C) 2020 Lawrence Fernandes

import glob
import json
import math
from time import time

import matplotlib.pyplot as plt
import numpy as np
import pandas as pd
import seaborn as sns
from tqdm import tqdm

# Global variables
base_dir = '/home/lawrence.fernandes/Documents/Workspace/stocks/S&P500 datasets'
data_dir = f'{base_dir}/S&P500_500'
stock_files = sorted(glob.glob(f'{data_dir}/*.csv'))  # Using glob to get the path
instance_path = f'{base_dir}/instance.txt'
stock_tickers_path = f'{base_dir}/stock_tickers.json'


def get_instance(plot_heatmap=False):
    df_list = []
    tickers_dict = {}
    rename_dict = {}
    tickers_counter = 0
    stocks_counter = 0

    print('\nReading quotation data...')
    for file in tqdm(enumerate(sorted(stock_files))):
        stocks_counter += 1
        ticker = file[1].replace(f'{data_dir}/', '').replace('.csv', '')
        df = pd.read_csv(file[1])
        df = df[['Date', 'Close']]
        df.columns = ['Date', tickers_counter]
        tickers_dict[ticker] = tickers_counter

        rename_dict[tickers_counter] = ticker

        tickers_counter += 1
        df.set_index('Date', inplace=True)
        df_list.append(df)

    # Concatenate the dataframes and calculate Pearson's correlation coefficient
    df_corr = pd.concat(df_list, axis=1).rename_axis('Symbols', axis=1)
    df_corr_ticker = df_corr.rename(columns=rename_dict)
    df_corr = df_corr.corr(method='pearson')

    if plot_heatmap:
        # Plot heatmap graph
        df_corr_ticker = df_corr_ticker.corr(method='pearson')
        sns.heatmap(df_corr_ticker, cmap="RdBu")
        plt.show()

    '''
    df_corr.reset_index(inplace=True)
    del df_corr.index.name

    # Set values on upper triangular correlation matrix to NaN
    df_corr = df_corr.where(np.tril(np.ones(df_corr.shape)).astype(np.bool))

    # Unpivoting the correlation matrix and removing NaN values
    df_unpivoted = df_corr.melt(id_vars=['Symbols'], var_name='Ticker', value_name='Corr')
    df_unpivoted.dropna(inplace=True)

    print('\nWriting instance file...')
    with open(instance_path, 'w') as f:
        f.write(f'{stocks_counter}\n')
        for row in df_unpivoted.itertuples():
            # f.write(f'{row[2]} {row[1]} {row[3]}\n')
            normalized_corr = math.sqrt(2 * (1 - row[3]))
            f.write(f'{row[2]} {row[1]} {normalized_corr}\n')

    print('\nWriting tickers file...')
    with open(stock_tickers_path, 'w') as fp:
        json.dump(tickers_dict, fp)
    '''


def main():
    start_time = time()
    get_instance(plot_heatmap=True)
    elapsed = time()-start_time
    print("\n--- Task done in %.2f seconds ---" % elapsed)


if __name__ == '__main__':
    main()
