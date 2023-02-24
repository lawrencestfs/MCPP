##!/usr/bin/python
# -*- coding: utf-8 -*-

import glob

import pandas as pd
from google.cloud import bigquery
from google.oauth2 import service_account

service_account_path = '/home/lawrence.fernandes/Documents/Workspace/data_integration/srv-compliance-cylon-key-prd.json'
credentials = service_account.Credentials.from_service_account_file(service_account_path)
result_path = '/home/lawrence.fernandes/Documents/Workspace/stocks/random_portfolios'
project_id = 'compliance-marketplace-prd'
dataset_ref = 'cylon'
bqclient = bigquery.Client(credentials=credentials, project=project_id)
stocks_path = '/home/lawrence.fernandes/Documents/Workspace/stocks/S&P500_full/'
stocks = sorted(glob.glob(stocks_path + '*.csv'))


def bigquery_export(client, dataset, table, dataframe):
    """ Export data to BigQuery
    """
    print(f'Exporting table {table}...')
    destination = f'{dataset}.{table}'
    job_config = bigquery.LoadJobConfig()

    job = client.load_table_from_dataframe(
        dataframe=dataframe,
        destination=destination,
        job_config=job_config
    )
    job.result()
    print(
        f'Query job {job.job_id} finished with state {job.state} after {str(job.ended - job.started)} s\n'
    )


def generate_dataset():
    print('Generating the dataset...\n')
    df_acoes = pd.read_excel(f'{result_path}/acoes_legais.xlsx')
    df_acoes2 = pd.read_excel(f'{result_path}/acoes_legais2.xlsx')
    df_acoes_full = pd.concat([df_acoes2, df_acoes])
    df_acoes_full = df_acoes_full.drop_duplicates()

    df_tickers_tcc = pd.read_csv(f'{result_path}/stock_tickers_legais.csv')
    df_acoes_final = pd.merge(
        df_tickers_tcc,
        df_acoes_full,
        how='left',
        left_on="TS",
        right_on="Ticker_symbol",
        suffixes=('_TCC', '_Base')
    )
    df_acoes_final = df_acoes_final[['TS', 'Security', 'GICS_Sector', 'GIC_Sub_Industry', 'DATA']]

    tabela_final = "temp_fn_gics_final4"
    bigquery_export(bqclient, dataset_ref, tabela_final, df_acoes_final)

    # filter tickers
    ticker_list = []
    for file in enumerate(sorted(stocks)):
        stock_ticker = file[1].replace(stocks_path, '').replace('.csv', '')
        ticker_list.append(stock_ticker)
    tickers = str(tuple([key for key in ticker_list])).replace(',)', ')')

    num_iter = range(5)
    qtd = [10, 20, 30, 60, 100, 200]
    list_df = []

    for i in qtd:
        for k in num_iter:

            query_stocks_simple = f'''
            SELECT
              ts,
              RAND() AS r,
              {k}+1 AS id_df,
              {i} AS n_carteira
            FROM (
              SELECT TS FROM(
                SELECT ts
                    /*
                    CASE
                    WHEN ts = 'AET'  THEN 'CVS'
                    WHEN ts = 'AGN'  THEN 'ABBV'
                    WHEN ts = 'APC'  THEN 'OXY'
                    WHEN ts = 'BBT'  THEN 'TFC'
                    WHEN ts = 'BCR'  THEN 'BDX'
                    WHEN ts = 'BHGE' THEN 'BKR'
                    WHEN ts = 'CA'   THEN 'AVGO'
                    WHEN ts = 'CBG'  THEN 'CBRE'
                    WHEN ts = 'CELG' THEN 'BMY'
                    WHEN ts = 'CBS'  THEN 'VIAC'
                    WHEN ts = 'CSRA' THEN 'GD'
                    WHEN ts = 'DLPH' THEN 'BWA'
                    WHEN ts = 'DPS'  THEN 'KDP'
                    WHEN ts = 'DWDP' THEN 'DD'
                    WHEN ts = 'GGP'  THEN 'BPY'
                    WHEN ts = 'HCN'  THEN 'WELL'
                    WHEN ts = 'HCP'  THEN 'PEAK'
                    WHEN ts = 'HRS'  THEN 'LHX'
                    WHEN ts = 'JEC'  THEN 'J'
                    WHEN ts = 'KORS' THEN 'CPRI'
                    WHEN ts = 'LLL'  THEN 'LHX'
                    WHEN ts = 'LUK'  THEN 'JEF'
                    WHEN ts = 'PCLN' THEN 'BKNG'
                    WHEN ts = 'PX'   THEN 'LIN'
                    WHEN ts = 'RHT'  THEN 'IBM'
                    WHEN ts = 'RTN'  THEN 'RTX'
                    WHEN ts = 'STI'  THEN 'TFC'
                    WHEN ts = 'SYMC' THEN 'NLOK'
                    WHEN ts = 'TMK'  THEN 'GL'
                    WHEN ts = 'TSS'  THEN 'GPN'
                    WHEN ts = 'UTX'  THEN 'RTX'
                    WHEN ts = 'VIAB' THEN 'VIAC'
                    WHEN ts = 'WYN'  THEN 'WYND'
                    ELSE ts
                    END AS ts
                    */
                FROM {project_id}.{dataset_ref}.temp_fn_gics_final4
                WHERE ts IN {tickers}
                /*
                AND GICS_Sector IN (
                    "Utilities","Materials","Health Care", "Financials",
                    "Telecommunications Services", "Consumer Staples"
                )
                */
                GROUP BY TS
              )
              WHERE RAND() < 2*{i}/459
                -- RAND() < 2*{i}/203   /* filtro de setor */
            )
            ORDER BY r
            LIMIT {i}
            '''
            query_job = bqclient.query(query_stocks_simple, location='US')
            results = query_job.result()
            df_base_stocks_simple = results.to_dataframe()

            list_df.append(df_base_stocks_simple)

    df_carteiras = pd.concat(list_df)
    tabela_final = "temp_fn_gics_final6"
    bigquery_export(bqclient, dataset_ref, tabela_final, df_carteiras)


def export_dataset():
    print(f'Exporting dataset...')
    export_query = '''
    SELECT
        portfolio_size,
        portfolio_id,
        STRING_AGG(DISTINCT ts,",") AS stocks
    FROM (
        SELECT
          DISTINCT A.ts,
          A.n_carteira AS portfolio_size,
          A.id_df AS portfolio_id,
          B.GICS_Sector,
          B.GIC_Sub_Industry,
          IF(MAX(DATA) OVER (PARTITION BY A.ts)="ANTES","DEPOIS",DATA) AS DATA
        FROM
          compliance-marketplace-prd.cylon.temp_fn_gics_final6 AS A
        LEFT JOIN
          compliance-marketplace-prd.cylon.temp_fn_gics_final4 AS B
        ON
          A.ts = B.TS
    )
    GROUP BY portfolio_id, portfolio_size
    ORDER BY 1,2
    '''
    query_job = bqclient.query(query=export_query, location='US')
    results = query_job.result()
    df_base_final = results.to_dataframe()
    df_base_final.to_excel(f'{result_path}/solutions_perenes.xlsx', index=False)
    print(f'OK!')


def main():
    generate_dataset()
    export_dataset()


main()
