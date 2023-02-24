##!/usr/bin/python
# -*- coding: utf-8 -*-

# File Name: download_stocks.py
# Author: Lawrence Fernandes
# Copyright (C) 2017 Lawrence Fernandes

""" This module donwloads historical prices of stocks listed at the S&P500 index from Google Finance.
"""

import io
import datetime as dt

import pandas_datareader.data as web

save_path = "/home/lawrence.fernandes/Documents/Workspace/stocks/S&P500 datasets/S&P500_full/todo"
ticker_file = "/home/lawrence.fernandes/Documents/Workspace/stocks/S&P500 datasets/stock_tickers_todo.txt"
start_date = dt.datetime(2017, 1, 3)
end_date = dt.datetime(2020, 10, 3)


def get_data(source='yahoo'):
    """ Get the S&P500 stock's historical prices from Google Finance."""
    tickers = []
    with io.open(ticker_file, mode="r", encoding="utf-8") as f:
        for line in f:
            tickers.append(line.split()[1])

    for ticker in tickers:
        try:
            df = web.DataReader(
                name=ticker,
                data_source=source,
                start=start_date,
                end=end_date
            )
            df.to_csv(path_or_buf=f'{save_path}/{ticker}.csv')
            print("Success in retreiving data for: ", ticker)
        except:
            print("Failed in retreiving data for: ", ticker)
            continue


get_data()
