##!/usr/bin/python
# -*- coding: utf-8 -*-

from scipy import stats
import glob
import numpy as np
import pandas as pd
import seaborn as sns
import pandas_datareader as web
import matplotlib.pyplot as plt
import matplotlib
from datetime import datetime, timedelta

matplotlib.use("TkAgg")
start_date = '2017-01-03'
end_date = '2020-10-02'  # '2017-06-30'
solution_dir = '/home/lawrence.fernandes/Documents/Workspace/stocks/WhatIf/'
solutions = sorted(glob.glob(solution_dir+'*.txt'))  # Using glob to get the path
# data_dir = '/home/lawrence.fernandes/Documents/Workspace/stocks/S&P500 datasets/S&P500_500/'
data_dir = '/home/lawrence.fernandes/Documents/Workspace/stocks/S&P500_full/'
path = data_dir + '*.csv'
blacklist_tickers = [
    'AET', 'AGN', 'ANDV', 'APC', 'BBT', 'BCR', 'BHGE', 'CA', 'CBG', 'CBS', 'CELG', 'CSRA',
    'DLPH', 'DPS', 'DWDP', 'ESRX', 'EVHC', 'FOX', 'FOXA', 'GGP', 'HCN', 'HCP', 'HRS', 'JEC',
    'KORS', 'LLL', 'LUK', 'MON', 'NFX', 'PCLN', 'PX', 'RHT', 'RTN', 'SCG', 'SNI', 'STI', 'SYMC',
    'TMK', 'TSS', 'TWX', 'UTX', 'VIAB', 'WYN', 'XL', 'BHF', 'Q', 'LEN-B'
]


def read_solution(filename):
    """ Read the portfolio solution and return the tickers as a list """
    tickers = []
    with open(file=f'{solution_dir}{filename}', mode='r') as f:
        lines = f.readlines()[11:]
        for line in lines:
            line = line.replace('[', '').replace(']', '').replace(' ', '')
            values = line.split(',')
            if values[3].startswith('1'):
                tickers.append(values[0])
    return tickers


def read_solutions():
    """ Read the portfolio solution and return the tickers as a list """
    solutions_dict = {}  # Create empty dictionary

    for file in enumerate(sorted(solutions)):
        solution_name = file[1].replace(solution_dir, '').replace('.txt', '')
        tickers = []
        with open(file=file[1], mode='r') as f:
            lines = f.readlines()[11:]
            for line in lines:
                line = line.replace('[', '').replace(']', '').replace(' ', '')
                values = line.split(',')
                if values[3].startswith('1') and values[0] not in blacklist_tickers:
                    tickers.append(values[0])
        solutions_dict.update({solution_name: tickers})
    return solutions_dict


def read_stocks(stock_tickers):
    """ Read the cvs files and return their close column as a dataframe """
    df_list = []
    for stock in stock_tickers:
        df = pd.read_csv(data_dir + stock + '.csv')
        df = df[['Date', 'Close']]
        df.columns = ['Date', stock]
        df.set_index('Date', inplace=True)
        df_list.append(df)
    df = pd.concat(df_list, axis=1).rename_axis('Symbols', axis=1)
    return df


def get_naive_weights(portfolio_size):
    """ Get the weight of each stock in the portfolio by the naive investment approach """
    weight = 1 / portfolio_size
    weights = [weight] * portfolio_size
    return weights


def get_stats(portfolio_name, benchmark_ret, port_ret, scatter_plot=False):
    """ Get portfolio statistics """

    # convert the daily returns to geometric average annual returns
    geometric_port_return = np.prod(port_ret + 1) ** (252 / port_ret.shape[0]) - 1
    # print("Geometric Average Annual Return:", geometric_port_return)

    # calculate the annualized portfolio standard deviation or volatility
    annual_std = np.std(port_ret) * np.sqrt(252)

    # portfolio Sharpe ratio
    port_sharpe_ratio = geometric_port_return / annual_std

    if scatter_plot:
        # scatter plot portfolio vs benchmark returns
        """
        fig = plt.figure()
        ax1 = fig.add_axes([0.1, 0.1, 0.8, 0.8])
        ax1.plot(port_ret, label="Portfolio")
        ax2 = fig.add_axes([0.1, 0.1, 0.8, 0.8])
        ax2.plot(benchmark_ret, label="Benchmark")
        plt.xlabel('Date')
        plt.ylabel("Cumulative Returns")
        plt.title("Portfolio Cumulative Returns")
        plt.legend()
        plt.show()
        """
        sns.regplot(benchmark_ret.values, port_ret.values)
        plt.xlabel("Benchmark Returns")
        plt.ylabel("Portfolio Returns")
        plt.title("Portfolio Returns vs Benchmark Returns")
        plt.show()

    beta, alpha, r_value, p_value, std_err = stats.linregress(benchmark_ret.values, port_ret.values)
    r_squared = r_value ** 2

    print(portfolio_name)
    print("Beta:", round(beta, 4))
    print("Alpha:", round(alpha, 5))
    print("R-squared:", round(r_squared, 4))
    print("Standard Deviation (annualized):", round(annual_std, 4))
    print("Sharpe ratio:", round(port_sharpe_ratio, 4))
    print("\n")


def execute(get_price_date=False):
    solutions_dict = read_solutions()

    df_list = []
    df_columns = ['Benchmark']

    benchmark_price = web.get_data_yahoo('SPY', start=start_date, end=end_date)
    benchmark_ret = benchmark_price['Close'].pct_change()[1:]
    benchmark_cumulative_ret = (benchmark_ret + 1).cumprod()

    df_list.append(benchmark_cumulative_ret)

    for portfolio, tickers in solutions_dict.items():
        df_columns.append(portfolio)

        if get_price_date:
            price_data = web.get_data_yahoo(tickers, start=start_date, end=end_date)
            price_data = price_data['Close']
        else:
            price_data = read_stocks(tickers)

        wts = get_naive_weights(portfolio_size=len(tickers))

        # calculate the daily returns of the assets
        # print('Calculating the daily returns...')
        ret_data = price_data.pct_change()[1:]

        # calculate the portfolio returns
        # print('Calculating the portfolio returns...')
        port_ret = (ret_data * wts).sum(axis=1)
        port_ret.index = pd.to_datetime(port_ret.index)
        portfolio_cumulative_ret = (port_ret + 1).cumprod()

        df_list.append(portfolio_cumulative_ret)

        get_stats(portfolio, benchmark_ret, port_ret)

    # Plot cumulative returns

    df = pd.concat(df_list, axis=1)
    df.columns = df_columns
    df.columns.name = 'Portfolio'
    plot = df.plot(y=df_columns, use_index=True, kind="line", title='MCPP portfolios vs Benchmark')
    plot.set_ylabel("Cumulative returns")
    plt.show()


def main(get_price_date=False):
    tickers = read_solution()
    if get_price_date:
        price_data = web.get_data_yahoo(tickers, start=start_date, end=end_date)
        price_data = price_data['Adj Close']
    else:
        price_data = read_stocks(tickers)

    wts = get_naive_weights(portfolio_size=len(tickers))

    # calculate the daily returns of the assets
    print('Calculating the daily returns...')
    ret_data = price_data.pct_change()[1:]

    # calculate the portfolio returns
    port_ret = (ret_data * wts).sum(axis=1)
    port_ret.index = pd.to_datetime(port_ret.index)

    benchmark_price = web.get_data_yahoo('SPY', start=start_date, end=end_date)
    benchmark_ret = benchmark_price['Close'].pct_change()[1:]

    get_stats(benchmark_ret, port_ret)


# main()
execute()
