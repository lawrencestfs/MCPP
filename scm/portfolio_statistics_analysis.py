##!/usr/bin/python
# -*- coding: utf-8 -*-

import glob

import matplotlib
import matplotlib.pyplot as plt
import numpy as np
import pandas as pd
import pandas_datareader as web
import seaborn as sns
from scipy import stats

from risk_metrics import *

matplotlib.use("TkAgg")
start_date = '2017-01-03'
end_date = '2020-10-02'
# solution_dir = '/home/lawrence.fernandes/Documents/Workspace/stocks/WhatIf/'
solution_dir = '/home/lawrence.fernandes/Documents/Workspace/stocks/WhatIf/results_exact/'
solutions = sorted(glob.glob(solution_dir + '*.txt'))  # Using glob to get the path
random_solutions = '/home/lawrence.fernandes/Documents/Workspace/stocks/random_portfolios/solutions_final.xlsx'
benchmark_file = '/home/lawrence.fernandes/Documents/Workspace/stocks/benchmark.csv'
data_dir = '/home/lawrence.fernandes/Documents/Workspace/stocks/S&P500_full/'
path = data_dir + '*.csv'

blacklist_tickers = [
    'AET', 'RTN', 'XL', 'BCR', 'UTX', 'DLPH',
    'RHT', 'PCLN', 'PX', 'MON', 'TSS', 'HCN', 'TWX'
]

blacklist_tickers_full = [
    'AET', 'AGN', 'ANDV', 'APC', 'BBT', 'BCR', 'BHGE', 'CA', 'CBG', 'CBS', 'CELG', 'CSRA',
    'DLPH', 'DPS', 'DWDP', 'ESRX', 'EVHC', 'FOX', 'FOXA', 'GGP', 'HCN', 'HCP', 'HRS', 'JEC',
    'KORS', 'LLL', 'LUK', 'MON', 'NFX', 'PCLN', 'PX', 'RHT', 'RTN', 'SCG', 'SNI', 'STI', 'SYMC',
    'TMK', 'TSS', 'TWX', 'UTX', 'VIAB', 'WYN', 'XL', 'BHF', 'Q', 'LEN-B'
]


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


def read_solutions_exact():
    """ Read the portfolio solution of the exact method """
    solutions_dict = {}  # Create empty dictionary

    for file in enumerate(sorted(solutions)):
        solution_name = file[1].replace(solution_dir, '').replace('.txt', '')
        tickers = []
        with open(file=file[1], mode='r') as f:
            lines = f.readlines()[4:]
            for line in lines:
                if line not in blacklist_tickers:
                    ticker = line.replace('\n', '')
                    tickers.append(ticker)
        print(solution_name, len(tickers))
        solutions_dict.update({solution_name: tickers})
        print(solutions_dict)
    return solutions_dict


def read_solutions_new(portfolio_size, compare_random=False):
    """ Read the portfolio solution and return the tickers as a list """
    solutions_dict = {}  # Create empty dictionary
    tickers = []
    # filename_pattern = f'P_{str(portfolio_size).zfill(3)}'
    filename_pattern = f'P_{str(portfolio_size)}'

    for file in enumerate(sorted(solutions)):
        solution_name = file[1].replace(solution_dir, '').replace('.txt', '')
        if solution_name == filename_pattern:
            # Read MCIP solution
            with open(file=file[1], mode='r') as f:
                lines = f.readlines()[11:]
                for line in lines:
                    line = line.replace('[', '').replace(']', '').replace(' ', '')
                    values = line.split(',')
                    # if values[3].startswith('1') and values[0] not in blacklist_tickers:
                    if values[3].startswith('1'):
                        tickers.append(values[0])
            solutions_dict.update({solution_name: tickers})

            if compare_random:
                # Read random portfolios
                df = pd.read_excel(random_solutions)
                df.query(expr=f'portfolio_size=={portfolio_size}', inplace=True)
                for index, row in df.iterrows():
                    solution_name = f"R_{row['portfolio_size']}_{row['portfolio_id']}"
                    tickers = row['stocks'].split(',')
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
    df = pd.concat(df_list, axis=1, sort=True).rename_axis('Symbols', axis=1)
    return df


def get_naive_weights(portfolio_size):
    """ Get the weight of each stock in the portfolio by the naive investment approach """
    weight = 1 / portfolio_size
    weights = [weight] * portfolio_size
    return weights


def get_stats(portfolio_name, benchmark_ret, port_ret):
    """ Get portfolio statistics """

    # convert the daily returns to geometric average annual returns
    geometric_port_return = np.prod(port_ret + 1) ** (252 / port_ret.shape[0]) - 1
    # print("Geometric Average Annual Return:", geometric_port_return)

    # calculate the annualized portfolio standard deviation or volatility
    annual_std = np.std(port_ret) * np.sqrt(252)

    # portfolio Sharpe ratio
    port_sharpe_ratio = geometric_port_return / annual_std

    beta, alpha, r_value, p_value, std_err = stats.linregress(benchmark_ret.values, port_ret.values)
    r_squared = r_value ** 2

    print(portfolio_name)
    print("Beta:", round(beta, 4))
    print("Alpha:", round(alpha, 5))
    print("R-squared:", round(r_squared, 4))
    print("Standard Deviation (annualized):", round(annual_std, 4))
    # print("Sharpe ratio:", round(port_sharpe_ratio, 4))
    print("Sharpe ratio:", round((sharpe_ratio(er=np.mean(port_ret), returns=port_ret, rf=0) * (252**0.5)), 4))
    print("Information Ratio:", information_ratio(port_ret, benchmark_ret))
    print("Annualized Return: {:.2%}".format(geometric_port_return))
    print("\n")


def get_scatter_plots(portfolio_returns, benchmark_ret):
    fig, (ax1, ax2, ax3) = plt.subplots(ncols=3, sharey=True)
    sns.regplot(x=portfolio_returns[0], y=benchmark_ret, ax=ax1)
    sns.regplot(x=portfolio_returns[1], y=benchmark_ret, ax=ax2)
    sns.regplot(x=portfolio_returns[2], y=benchmark_ret, ax=ax3)
    ax1.set(xlabel='Benchmark Returns', ylabel='P_010 Returns')
    ax2.set(xlabel='Benchmark Returns', ylabel='P_020 Returns')
    ax3.set(xlabel='Benchmark Returns', ylabel='P_030 Returns')

    fig, (ax1, ax2, ax3) = plt.subplots(ncols=3, sharey=True)
    sns.regplot(x=portfolio_returns[3], y=benchmark_ret, ax=ax1)
    sns.regplot(x=portfolio_returns[4], y=benchmark_ret, ax=ax2)
    sns.regplot(x=portfolio_returns[5], y=benchmark_ret, ax=ax3)
    ax1.set(xlabel='Benchmark Returns', ylabel='P_060 Returns')
    ax2.set(xlabel='Benchmark Returns', ylabel='P_100 Returns')
    ax3.set(xlabel='Benchmark Returns', ylabel='P_200 Returns')


def main(portfolio_size=10, get_price_data=False, scatter_plots=True):
    # solutions_dict = read_solutions()
    solutions_dict = read_solutions_exact()
    # solutions_dict = read_solutions_new(portfolio_size)
    portfolio_returns = []

    df_list = []
    df_columns = ['Benchmark']

    if get_price_data:
        # benchmark_price = web.get_data_yahoo('SPY', start=start_date, end=end_date)
        benchmark_price = web.DataReader(
            name='SPY',
            data_source='yahoo',
            start=start_date,
            end=end_date
        )
        benchmark_price.to_csv(benchmark_file)
    else:
        benchmark_price = pd.read_csv(benchmark_file)
        benchmark_price.set_index('Date', inplace=True)
        benchmark_price.index = pd.to_datetime(benchmark_price.index)

    benchmark_ret = benchmark_price['Close'].pct_change()[1:]
    benchmark_cumulative_ret = (benchmark_ret + 1).cumprod()
    df_list.append(benchmark_cumulative_ret)

    for portfolio, tickers in solutions_dict.items():
        df_columns.append(portfolio)

        if get_price_data:
            price_data = web.DataReader(
                name=tickers,
                data_source='yahoo',
                start=start_date,
                end=end_date
            )
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

        portfolio_returns.append(port_ret)

    get_stats('benckmark', benchmark_ret, benchmark_ret)

    if scatter_plots:
        get_scatter_plots(portfolio_returns, benchmark_ret)

    # Plot cumulative returns
    df = pd.concat(df_list, axis=1)
    df.columns = df_columns
    df.columns.name = 'Portfolio'
    plot = df.plot(y=df_columns, use_index=True, kind="line", title='MCPP portfolios vs Benchmark')
    plot.set_ylabel("Cumulative returns")
    plt.show()


main()
