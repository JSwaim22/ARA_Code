# -*- coding: utf-8 -*-
"""
Created on Wed Jun 15 16:11:25 2022

@author: Joshua Swaim

"""

import sys
import csv
import os
import calendar
import random

def main():
    if len(sys.argv) >= 2:
        start_year = sys.argv[1]
        if len(sys.argv) >= 3:
            end_year = sys.argv[2]
            if len(sys.argv) >= 4:
                folder_name = sys.argv[3]
            else:
                folder_name = 'AnnualExpenseData'
        else:
            end_year = 2022
            folder_name = 'AnnualExpenseData'
    else:
        start_year = 1980
        end_year = 1998
        folder_name = 'AnnualExpenseData'
    
    try:
        start_year = int(start_year)
        if start_year < 1980 or start_year > 2022:
            print('Ivalid start year: Must be a number between 1980 and 2022')
            exit(-1)
    except (ValueError, RuntimeError, TypeError, NameError):
        print('Invalid start year: Must be a number between 1980 and 2022')
        exit(-1)
        
    try:
        end_year = int(end_year)
        if end_year < 1980 or end_year > 2022 or end_year < start_year:
            print('Ivalid end year: Must be a number between 1980 and 2022 and not before start year')
            exit(-1)
    except (ValueError, RuntimeError, TypeError, NameError):
        print('Invalid end year: Must be a number between 1980 and 2022 and not before start year')
        exit(-1)
    
    try:
        folder_name = str(folder_name)
    except (ValueError, RuntimeError, TypeError, NameError):
        print('Invalid folder name: Must be a string')
        exit(-1)
    
    if not os.path.exists(folder_name):
        os.makedirs(folder_name)
        
    print("Starting at year " + str(start_year))
    print("Ending at year " + str(end_year))
    print("Saving in folder " + folder_name)
    
    for year in range(start_year, end_year + 1):
        file_name = folder_name + '/' + str(year) + "MonthlyExpenses.csv"
        expenses_generated = generateExpenses(year)
        for expense in expenses_generated:
            writeCSVlist(file_name, expense)
    
def writeCSVlist(file_name: str, expense: dict):
    exists = os.path.exists(file_name)
    with open(file_name, 'a', newline = '') as csvfile:
         fieldnames = ['Year', 'Month', 'Expense_Category', 'Expense_Type', 'Amount_Due', 'Due_Date', 'Amount_Paid', 'Payment_Date', 'Payment_Method']
         writer = csv.DictWriter(csvfile, fieldnames=fieldnames)
         if not exists:
             writer.writeheader()
         writer.writerow({'Year':expense['Year'],
                          'Month':expense['Month'],
                          'Expense_Category':expense['Expense_Category'],
                          'Expense_Type':expense['Expense_Type'],
                          'Amount_Due':expense['Amount_Due'],
                          'Due_Date':expense['Due_Date'],
                          'Amount_Paid':expense['Amount_Paid'],
                          'Payment_Date':expense['Payment_Date'],
                          'Payment_Method':expense['Payment_Method']})

def generateExpenses(year: int):
    expenses_generated = []
    
    expense_types = getExpenseTypes()
    payment_methods = getPaymentMethods()
    
    for month in range(1,13):
        for value in set(expense_types.values()):
            while True:
                key, val = random.choice(list(expense_types.items()))
                if val == value:
                    break
            if value == 'Mortgage' or value == 'Car' or value == 'Credit Card':
                amount_due = round(random.uniform(500,3000), 2)
                amount_paid = round(random.uniform(250,1500), 2)
            else:
                amount_due = round(random.uniform(5,200), 2)
                amount_paid = round(random.uniform(2.5,100), 2)
            dates = []
            for day in calendar.Calendar().itermonthdays(year,month):
                if day > 0:
                    dates.append(day)
            due_date = random.choice(dates)
            dates.append(0)
            payment_date = random.choice(dates)
            payment_method = random.choice(payment_methods)
            
            expenses_generated.append({'Year':year,
                                       'Month':calendar.month_name[month],
                                       'Expense_Category':value,
                                       'Expense_Type':key,
                                       'Amount_Due':amount_due,
                                       'Due_Date':due_date,
                                       'Amount_Paid':amount_paid,
                                       'Payment_Date':payment_date,
                                       'Payment_Method':payment_method})
    return expenses_generated

def getExpenseTypes():
    # Reading Expense Types
    with open('ExpenseTypes.csv', 'r', newline='') as csvfile:
        reader = csv.DictReader(csvfile)
        expense_types = {}
        for row in reader:
            expense_types[row['Expense_Type']] = row['Expense_Category']
    return expense_types

def getPaymentMethods():
    # Reading Payment Methods
    with open('PaymentMethods.csv', 'r', newline='') as csvfile:
        reader = csv.reader(csvfile, delimiter=',', quotechar= '"', quoting=csv.QUOTE_MINIMAL)
        payment_methods = []
        for row in reader:
            payment_methods.append(str(row).strip('[').strip(']').strip('\''))
    return payment_methods
            

if __name__ == "__main__":
    main()