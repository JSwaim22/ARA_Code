# -*- coding: utf-8 -*-
"""
Created on Thu Jun 16 13:55:54 2022

@author: Joshua Swaim

"""

'''
fix
    option for modify when 0 search results
'''


'''---------------------
         Libraries
   ---------------------'''

import sys
import csv
import os
import calendar
import matplotlib.pyplot as plt
import numpy as np


'''---------------------
           Main
   ---------------------'''

def main():
    print('\n     _______________________________________________')
    print('    | Welcome to your Expense Tracking Application! |')
    print('    |_______________________________________________|\n')
    
    if len(sys.argv) >= 2:
        mode = sys.argv[1]
    else:
        mode = '--default'
    
    if mode == '--report':
        print('     _____________')
        print('    | Report Mode |')
        print('    |     Menu    |')
        print('    |_____________|\n')
        reportMode()
    elif mode == '--graph':
        print('     ____________')
        print('    | Graph Mode |')
        print('    |    Menu    |')
        print('    |____________|\n')
        graphMode()
    else:
        print('     ______________')
        print('    | Default Mode |')
        print('    |     Menu     |')
        print('    |______________|\n')
        defaultMode()
        

'''---------------------
       Report Mode
   ---------------------'''
        
def reportMode():
    print('Enter a key from a to d for use of the following functions')
    print('   a.  Generate annual expense total report')
    print('   b.  Generate annual expense type report')
    print('   c.  Generate monthly expense report')
    print('   d.  Close the program (can be pressed anytime)\n')
    close_character = 'd'
    options = ['a', 'b', 'c']
    letter = getLetter(close_character, options)
    if not os.path.exists("ExpenseReports"):
        os.makedirs("ExpenseReports")
    
    if letter == 'a':
        print('     ______________________________________')
        print('    | Generate annual expense total report |')
        print('    |______________________________________|\n')
        genTotalReport()
    elif letter == 'b':
        print('     _____________________________________')
        print('    | Generate annual expense type report |')
        print('    |_____________________________________|\n')
        genTypeReport()
    elif letter == 'c':
        print('     _________________________________')
        print('    | Generate monthly expense report |')
        print('    |_________________________________|\n')
        genMonthReport()
        
def genTotalReport():
    close_character = 'd'
    
    # Entering Years
    start_year = getStartYear(close_character)  
    end_year = getEndYear(close_character, start_year)
            
    print('Enter the t key to choose an expense type or the c key to choose an expense category for the report')
    print('   t.  Choose expense type')
    print('   c.  Choose expense category\n')
    options = ['t', 'c']
    letter = getLetter(close_character, options)
            
    if letter == 't':
        print('     _____________________')
        print('    | Choose expense type |')
        print('    |_____________________|\n')
        chooseExpenseTypeTotalReport(start_year, end_year)
    elif letter == 'c':
        print('     _________________________')
        print('    | Choose expense category |')
        print('    |_________________________|\n')
        chooseExpenseCategoryTotalReport(start_year, end_year)
        
def chooseExpenseTypeTotalReport(start_year, end_year):
    close_character = 'd'
    
    # Reading Expense Types
    expense_types = getExpenseTypes()
    
    # Entering Expense Type
    key = getExpenseType(close_character, expense_types)
    
    # Reading Expense Amounts    
    expense_dictionary = {}
    for year in range(start_year, end_year + 1):
        expense_dictionary[year] = 0
        file_name = "AnnualExpenseData/" + str(year) + "MonthlyExpenses.csv"
        with open(file_name, 'r', newline='') as csvfile:
            reader = csv.DictReader(csvfile)
            for row in reader:
                if row['Expense_Type'] == key:
                    expense_dictionary[year] += float(row['Amount_Due']) 
            
    # Writing Expense Amounts to File
    file_name = "ExpenseReports/" + str(start_year) + "to" + str(end_year) + key.replace(' ', '') + "Totals.csv"
    with open(file_name, 'w', newline = '') as csvfile:
         fieldnames = ['Year', 'Expense']
         writer = csv.DictWriter(csvfile, fieldnames=fieldnames)
         writer.writeheader()
         for year in range(start_year, end_year + 1):
             writer.writerow({'Year':year,
                              'Expense':expense_dictionary[year]})
             
    print('\nReport generated in folder ExpenseReports under name ' + file_name.strip("ExpenseReports/"))
    input('   Press enter to return to menu ')
    print('     ________________')
    print('    | Return to menu |')
    print('    |________________|\n')
    print('|/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\|\n')
    print('     ______________')
    print('    | Report Mode |')
    print('    |    Menu     |')
    print('    |_____________|\n')
    reportMode()
    
def chooseExpenseCategoryTotalReport(start_year, end_year):
    close_character = 'd'
    
    # Reading Expense Types
    expense_types = getExpenseTypes()
            
    # Entering Expense Category
    value = getExpenseCategory(close_character, expense_types)
            
    # Reading Expense Amounts    
    expense_dictionary = {}
    for year in range(start_year, end_year + 1):
        expense_dictionary[year] = 0
        file_name = "AnnualExpenseData/" + str(year) + "MonthlyExpenses.csv"
        with open(file_name, 'r', newline='') as csvfile:
            reader = csv.DictReader(csvfile)
            for row in reader:
                if row['Expense_Category'] == value:
                    expense_dictionary[year] += float(row['Amount_Due']) 
    
    # Writing Expense to File
    file_name = "ExpenseReports/" + str(start_year) + "to" + str(end_year) + value.replace(' ', '') + "Totals.csv"
    with open(file_name, 'w', newline = '') as csvfile:
         fieldnames = ['Year', 'Expense']
         writer = csv.DictWriter(csvfile, fieldnames=fieldnames)
         writer.writeheader()
         for year in range(start_year, end_year + 1):
             writer.writerow({'Year':year,
                              'Expense':expense_dictionary[year]})
             
    print('\nReport generated in folder ExpenseReports under name ' + file_name.strip("ExpenseReports/"))
    input('   Press enter to return to menu ')
    print('     ________________')
    print('    | Return to menu |')
    print('    |________________|\n')
    print('|/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\|\n')
    print('     ______________')
    print('    | Report Mode |')
    print('    |    Menu     |')
    print('    |_____________|\n')
    reportMode()
    
def genTypeReport():
    close_character = 'd'
    
    # Entering Years
    start_year = getStartYear(close_character)
    end_year = getEndYear(close_character, start_year)
            
    print('Enter the t key to choose an expense type or the c key to choose an expense category for the report')
    print('   t.  Choose expense type')
    print('   c.  Choose expense category\n')
    options = ['t', 'c']
    letter = getLetter(close_character, options)
            
    if letter == 't':
        print('     _____________________')
        print('    | Choose expense type |')
        print('    |_____________________|\n')
        chooseExpenseTypeTypeReport(start_year, end_year)
    elif letter == 'c':
        print('     _________________________')
        print('    | Choose expense category |')
        print('    |_________________________|\n')
        chooseExpenseCategoryTypeReport(start_year, end_year)
        
def chooseExpenseTypeTypeReport(start_year, end_year):
    close_character = 'd'
    
    # Reading Expense Types
    expense_types = getExpenseTypes()
    
    # Entering Expense Type
    key = getExpenseType(close_character, expense_types)
    
    # Reading Expense Amounts    
    expense_dictionaries = []
    for year in range(start_year, end_year + 1):
        file_name = "AnnualExpenseData/" + str(year) + "MonthlyExpenses.csv"
        with open(file_name, 'r', newline='') as csvfile:
            reader = csv.DictReader(csvfile)
            for row in reader:
                if row['Expense_Type'] == key:
                    expense_dictionaries.append({'Year':year,
                                                 'Expense_Type':key,
                                                 'Expense_Category':row['Expense_Category'],
                                                 'Expense':row['Amount_Due']})
            
    # Writing Expenses to File
    file_name = "ExpenseReports/" + str(start_year) + "to" + str(end_year) + key.replace(' ', '') + "Expenses.csv"
    with open(file_name, 'w', newline = '') as csvfile:
         fieldnames = ['Year', 'Expense_Type', 'Expense_Category', 'Expense']
         writer = csv.DictWriter(csvfile, fieldnames=fieldnames)
         writer.writeheader()
         for expense_dictionary in expense_dictionaries:
             writer.writerow(expense_dictionary)
             
    print('\nReport generated in folder ExpenseReports under name ' + file_name.strip("ExpenseReports/"))
    input('   Press enter to return to menu ')
    print('     ________________')
    print('    | Return to menu |')
    print('    |________________|\n')
    print('|/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\|\n')
    print('     ______________')
    print('    | Report Mode |')
    print('    |    Menu     |')
    print('    |_____________|\n')
    reportMode()
    
def chooseExpenseCategoryTypeReport(start_year, end_year):
    close_character = 'd'
    
    # Reading Expense Types
    expense_types = getExpenseTypes()
            
    # Entering Expense Category
    value = getExpenseCategory(close_character, expense_types)
            
    # Reading Expense Amounts    
    expense_dictionaries = []
    for year in range(start_year, end_year + 1):
        file_name = "AnnualExpenseData/" + str(year) + "MonthlyExpenses.csv"
        with open(file_name, 'r', newline='') as csvfile:
            reader = csv.DictReader(csvfile)
            for row in reader:
                if row['Expense_Category'] == value:
                    expense_dictionaries.append({'Year':year,
                                                 'Expense_Type':row['Expense_Type'],
                                                 'Expense_Category':value,
                                                 'Expense':row['Amount_Due']})
    
    # Writing Expenses to File
    file_name = "ExpenseReports/" + str(start_year) + "to" + str(end_year) + value.replace(' ', '') + "Expenses.csv"
    with open(file_name, 'w', newline = '') as csvfile:
         fieldnames = ['Year', 'Expense_Type', 'Expense_Category', 'Expense']
         writer = csv.DictWriter(csvfile, fieldnames=fieldnames)
         writer.writeheader()
         for expense_dictionary in expense_dictionaries:
             writer.writerow(expense_dictionary)
             
    print('\nReport generated in folder ExpenseReports under name ' + file_name.strip("ExpenseReports/"))
    input('   Press enter to return to menu ')
    print('     ________________')
    print('    | Return to menu |')
    print('    |________________|\n')
    print('|/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\|\n')
    print('     ______________')
    print('    | Report Mode |')
    print('    |    Menu     |')
    print('    |_____________|\n')
    reportMode()
    
def genMonthReport():
    close_character = 'd'
    
    # Entering Year
    year = getYear(close_character)
            
    # Reading Expense Amounts    
    expense_dictionary = {'January':0, 'February':0, 'March':0, 'April':0, 'May':0, 'June':0,
                          'July':0, 'August':0, 'September':0, 'October':0, 'November':0, 'December':0}
    file_name = "AnnualExpenseData/" + str(year) + "MonthlyExpenses.csv"
    with open(file_name, 'r', newline='') as csvfile:
        reader = csv.DictReader(csvfile)
        for row in reader:
            expense_dictionary[row['Month']] += float(row['Amount_Due'])
    
    # Writing Expenses to File
    file_name = "ExpenseReports/" + str(year) + "MonthlyTotals.csv"
    with open(file_name, 'w', newline = '') as csvfile:
         fieldnames = ['Month', 'Expense']
         writer = csv.DictWriter(csvfile, fieldnames=fieldnames)
         writer.writeheader()
         for k,val in expense_dictionary.items():
             writer.writerow({'Month':k,
                              'Expense':val})
             
    print('\nReport generated in folder ExpenseReports under name ' + file_name.strip("ExpenseReports/"))
    input('   Press enter to return to menu ')
    print('     ________________')
    print('    | Return to menu |')
    print('    |________________|\n')
    print('|/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\|\n')
    print('     ______________')
    print('    | Report Mode |')
    print('    |    Menu     |')
    print('    |_____________|\n')
    reportMode()
    

'''---------------------
        Graph Mode
   ---------------------'''
    
def graphMode():
    print('Enter a key from a to e for use of the following functions')
    print('   a.  Generate annual expense type bar graph')
    print('   b.  Generate annual expense type subbar graph')
    print('   c.  Generate annual expense type pie chart')
    print('   d.  Generate monthly expense total bar graph')
    print('   e.  Close the program (can be pressed anytime)\n')
    close_character = 'e'
    options = ['a', 'b', 'c', 'd']
    letter = getLetter(close_character, options)
    
    if letter == 'a':
        print('     ________________________________________')
        print('    | Generate annual expense type bar graph |')
        print('    |________________________________________|\n')
        genAnnualBarGraph()
    elif letter == 'b':
        print('     ___________________________________________')
        print('    | Generate annual expense type subbar graph |')
        print('    |___________________________________________|\n')
        genAnnualSubBarGraph()
    elif letter == 'c':
        print('     ________________________________________')
        print('    | Generate annual expense type pie chart |')
        print('    |________________________________________|\n')
        genAnnualPieChart()
    elif letter == 'd':
        print('     __________________________________________')
        print('    | Generate monthly expense total bar graph |')
        print('    |__________________________________________|\n')
        genMonthlyBarGraph()
        
def genAnnualBarGraph():
    close_character = 'e'
    
    # Entering Years
    start_year = getStartYear(close_character)
    end_year = getEndYear(close_character, start_year)
    
    print('Enter the t key to choose an expense type or the c key to choose an expense category for the graph')
    print('   t.  Choose expense type')
    print('   c.  Choose expense category\n')
    options = ['t', 'c']
    letter = getLetter(close_character, options)
            
    if letter == 't':
        print('     _____________________')
        print('    | Choose expense type |')
        print('    |_____________________|\n')
        chooseExpenseTypeBarGraph(start_year, end_year)
    elif letter == 'c':
        print('     _________________________')
        print('    | Choose expense category |')
        print('    |_________________________|\n')
        chooseExpenseCategoryBarGraph(start_year, end_year)
        
def chooseExpenseTypeBarGraph(start_year, end_year):
    close_character = 'e'
    
    # Reading Expense Types
    expense_dictionary = getExpenseTypes()
    
    # Entering Expense Type
    key = getExpenseType(close_character, expense_dictionary)
    
    # Reading Expense Amounts    
    expense_dictionary = {}
    years = []
    for year in range(start_year, end_year + 1):
        years.append(year)
        expense_dictionary[year] = 0
        file_name = "AnnualExpenseData/" + str(year) + "MonthlyExpenses.csv"
        with open(file_name, 'r', newline='') as csvfile:
            reader = csv.DictReader(csvfile)
            for row in reader:
                if row['Expense_Type'] == key:
                    expense_dictionary[year] += float(row['Amount_Due'])
            
    # Writing Expense Amounts to Graph
    ax = plt.subplot()
    ax.set_xticks(years)
    plt.bar(list(expense_dictionary.keys()), list(expense_dictionary.values()))
    plt.xlabel('Year')
    plt.ylabel('Expense ($)')
    plt.title(key + ' expenses from ' + str(start_year) + ' to ' + str(end_year))
    for k, val in expense_dictionary.items():
        plt.text(k, val, '$' + str(round(val, 2)), ha = 'center', bbox = dict(facecolor = 'grey', alpha = 0.5))
    plt.show()
             
    print('\nBar graph of expense type ' + str(key) + ' from ' + str(start_year) + ' to ' + str(end_year) + ' generated')
    input('   Press enter to return to menu ')
    print('     ________________')
    print('    | Return to menu |')
    print('    |________________|\n')
    print('|/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\|\n')
    print('     ______________')
    print('    | Graph Mode |')
    print('    |    Menu    |')
    print('    |____________|\n')
    graphMode()
    
    
def chooseExpenseCategoryBarGraph(start_year, end_year):
    close_character = 'e'
    
    # Reading Expense Types
    expense_dictionary = getExpenseTypes()
            
    # Entering Expense Category
    value = getExpenseCategory(close_character, expense_dictionary)
            
    # Reading Expense Amounts    
    expense_dictionary = {}
    years = []
    for year in range(start_year, end_year + 1):
        years.append(year)
        expense_dictionary[year] = 0
        file_name = "AnnualExpenseData/" + str(year) + "MonthlyExpenses.csv"
        with open(file_name, 'r', newline='') as csvfile:
            reader = csv.DictReader(csvfile)
            for row in reader:
                if row['Expense_Category'] == value:
                    expense_dictionary[year] += float(row['Amount_Due']) 
    
    # Writing Expense Amounts to Graph
    ax = plt.subplot()
    ax.set_xticks(years)
    plt.bar(list(expense_dictionary.keys()), list(expense_dictionary.values()))
    plt.xlabel('Year')
    plt.ylabel('Expense ($)')
    plt.title(value + ' expenses from ' + str(start_year) + ' to ' + str(end_year))
    for k, val in expense_dictionary.items():
        plt.text(k, val, '$' + str(round(val, 2)), ha = 'center', bbox = dict(facecolor = 'grey', alpha = 0.5))
    plt.show()
             
    print('\nBar graph of expense category ' + str(value) + ' from ' + str(start_year) + ' to ' + str(end_year )+ ' generated')
    input('   Press enter to return to menu ')
    print('     ________________')
    print('    | Return to menu |')
    print('    |________________|\n')
    print('|/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\|\n')
    print('     ______________')
    print('    | Graph Mode |')
    print('    |    Menu    |')
    print('    |____________|\n')
    graphMode()
    
def genAnnualSubBarGraph():
    close_character = 'e'
    
    # Entering Years
    start_year = getStartYear(close_character)
    end_year = getEndYear(close_character, start_year)
        
    print('Enter the c key to choose an expense category or the a key to use all expense categories for the graph')
    print('   c.  Choose expense category')
    print('   a.  Use all expense categories\n')
    options = ['c', 'a']
    letter = getLetter(close_character, options)
            
    if letter == 'c':
        print('     _________________________')
        print('    | Choose expense category |')
        print('    |_________________________|\n')
        chooseExpenseCategorySubBarGraph(start_year, end_year)
    elif letter == 'a':
        print('     ____________________________')
        print('    | Use all expense categories |')
        print('    |____________________________|\n')
        useAllExpenseCategorySubBarGraph(start_year, end_year)
        
def chooseExpenseCategorySubBarGraph(start_year, end_year):
    close_character = 'e'
    
    # Reading Expense Types
    expense_types = getExpenseTypes()
    
    # Entering Expense Category
    value = getExpenseCategory(close_character, expense_types)
    
    # Reading Expense Amounts  
    keys = []
    for k, val in expense_types.items():
        if val == value:
            keys.append(k)       
            
    expense_dictionaries = {}
    for year in range(start_year, end_year + 1):
        expense_dictionaries[year] = {}
        for k in keys:
            expense_dictionaries[year][k] = 0  

    for year in range(start_year, end_year + 1):
        file_name = "AnnualExpenseData/" + str(year) + "MonthlyExpenses.csv"
        with open(file_name, 'r', newline='') as csvfile:
            reader = csv.DictReader(csvfile)
            for row in reader:
                if row['Expense_Category'] == value:
                    expense_dictionaries[year][row['Expense_Type']] += float(row['Amount_Due'])
    
    expenses = [[]]
    expense_names = []
    for year in range(start_year, end_year + 1):
        i = 0
        for k, val in expense_dictionaries[year].items():
           expenses[i].append(val)
           i = i + 1
           if year == start_year:
               expenses.append([])
               expense_names.append(k)
    expenses.pop()
    
    fig, ax = plt.subplots()
    index = np.arange(start = start_year, stop = end_year + 1)
    bar_width = 0.075
    opacity = 1
    error_config = {'ecolor': '0.3'}
    colors = ['b', 'g', 'r', 'c', 'm', 'y', 'k']
    i = 0
    c = 0
    for expense in expenses:
        ax.bar(index + bar_width * i, expense, bar_width, alpha=opacity, color = colors[c], error_kw=error_config)
        i = i + 1
        if c == len(colors) - 1:
            c = 0
        else:
            c = c + 1
    i = 0
    for expense in expenses:
        year = start_year
        for e in expense:
            plt.text(year + bar_width * i, e, '$' + str(round(e, 2)) + '\n' + expense_names[i], ha = 'center', bbox = dict(facecolor = 'grey', alpha = 0.5), fontsize = 'xx-small')
            year = year + 1
        i = i + 1

    plt.xlabel('Years')
    plt.ylabel('Expense ($)')
    plt.title(value + ' expenses from ' + str(start_year) + ' to ' + str(end_year))
    
    plt.tight_layout()
    plt.show()
             
    print('\nSubbar graph of expense type ' + str(value) + ' from ' + str(start_year) + ' to ' + str(end_year) + ' generated')
    input('   Press enter to return to menu ')
    print('     ________________')
    print('    | Return to menu |')
    print('    |________________|\n')
    print('|/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\|\n')
    print('     ______________')
    print('    | Graph Mode |')
    print('    |    Menu    |')
    print('    |____________|\n')
    graphMode()
    
def useAllExpenseCategorySubBarGraph(start_year, end_year):
    # Reading Expense Types
    expense_types = getExpenseTypes()
    
    # Reading Expense Amounts  
    values = list(set(expense_types.values()))
    expense_dictionaries = {}
    for year in range(start_year, end_year + 1):
        expense_dictionaries[year] = {}
        for val in values:
            expense_dictionaries[year][val] = 0  

    for year in range(start_year, end_year + 1):
        file_name = "AnnualExpenseData/" + str(year) + "MonthlyExpenses.csv"
        with open(file_name, 'r', newline='') as csvfile:
            reader = csv.DictReader(csvfile)
            for row in reader:
                expense_dictionaries[year][row['Expense_Category']] += float(row['Amount_Due'])
    
    expenses = [[]]
    expense_names = []
    for year in range(start_year, end_year + 1):
        i = 0
        for k, val in expense_dictionaries[year].items():
           expenses[i].append(val)
           i = i + 1
           if year == start_year:
               expenses.append([])
               expense_names.append(k)
    expenses.pop()
    
    fig, ax = plt.subplots()
    index = np.arange(start = start_year, stop = end_year + 1)
    bar_width = 0.075
    opacity = 1
    error_config = {'ecolor': '0.3'}
    colors = ['b', 'g', 'r', 'c', 'm', 'y', 'k']
    i = 0
    c = 0
    for expense in expenses:
        ax.bar(index + bar_width * i, expense, bar_width, alpha=opacity, color = colors[c], error_kw=error_config)
        i = i + 1
        if c == len(colors) - 1:
            c = 0
        else:
            c = c + 1
    i = 0
    for expense in expenses:
        year = start_year
        for e in expense:
            plt.text(year + bar_width * i, e, '$' + str(round(e, 2)) + '\n' + expense_names[i], ha = 'center', bbox = dict(facecolor = 'grey', alpha = 0.5), fontsize = 'xx-small')
            year = year + 1
        i = i + 1

    plt.xlabel('Years')
    plt.ylabel('Expense ($)')
    plt.title('Expenses from ' + str(start_year) + ' to ' + str(end_year))
    
    plt.tight_layout()
    plt.show()
             
    print('\nSubbar graph of expenses from ' + str(start_year) + ' to ' + str(end_year) + ' generated')
    input('   Press enter to return to menu ')
    print('     ________________')
    print('    | Return to menu |')
    print('    |________________|\n')
    print('|/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\|\n')
    print('     ______________')
    print('    | Graph Mode |')
    print('    |    Menu    |')
    print('    |____________|\n')
    graphMode()
    
def genAnnualPieChart():
    close_character = 'e'
    
    # Entering Years
    year = getYear(close_character)
    
    print('Enter the c key to choose an expense category or the a key to use all expense categories for the graph')
    print('   c.  Choose expense category')
    print('   a.  Use all expense categories\n')
    options = ['c', 'a']
    letter = getLetter(close_character, options)
            
    if letter == 'c':
        print('     _________________________')
        print('    | Choose expense category |')
        print('    |_________________________|\n')
        chooseExpenseCategoryPieChart(year)
    elif letter == 'a':
        print('     ____________________________')
        print('    | Use all expense categories |')
        print('    |____________________________|\n')
        useAllExpenseCategoryPieChart(year)
        
def chooseExpenseCategoryPieChart(year):
    close_character = 'e'
    
    # Reading Expense Types
    expense_types = getExpenseTypes()
    
    # Entering Expense Category
    value = getExpenseCategory(close_character, expense_types)
    
    expense_dictionary = {}
    for k , val in expense_types.items():
        if val == value:
            expense_dictionary[k] = 0
    
    # Reading Expense Amounts  
    file_name = "AnnualExpenseData/" + str(year) + "MonthlyExpenses.csv"
    with open(file_name, 'r', newline='') as csvfile:
        reader = csv.DictReader(csvfile)
        for row in reader:
            if row['Expense_Category'] == value:
                expense_dictionary[row['Expense_Type']] += float(row['Amount_Due'])
    
    # Removing Expenses of Zero  
    keys = []          
    for k, val in expense_dictionary.items():
        if val == 0:
            keys.append(k)
    for k in keys:
        del expense_dictionary[k]
    
    # Making labels
    label_list = []
    for k, val in expense_dictionary.items():
        label_list.append(str(k) + '\n$' + str(round(float(val), 2)))
        
    # Writing Expense Amounts to Graph
    plt.pie(np.array(list(expense_dictionary.values())), labels = label_list)
    plt.title('Expenses in ' + value + ' for ' + str(year))
    plt.show()
             
    print('\nPie chart of expense catgeory ' + str(value) + ' in ' + str(year) + ' generated')
    input('   Press enter to return to menu ')
    print('     ________________')
    print('    | Return to menu |')
    print('    |________________|\n')
    print('|/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\|\n')
    print('     ______________')
    print('    | Graph Mode |')
    print('    |    Menu    |')
    print('    |____________|\n')
    graphMode()
    
def useAllExpenseCategoryPieChart(year):
    
    # Reading Expense Types
    expense_types = getExpenseTypes()
    
    expense_dictionary = {}
    for val in expense_types.values():
        expense_dictionary[val] = 0
    
    # Reading Expense Amounts  
    file_name = "AnnualExpenseData/" + str(year) + "MonthlyExpenses.csv"
    with open(file_name, 'r', newline='') as csvfile:
        reader = csv.DictReader(csvfile)
        for row in reader:
            expense_dictionary[row['Expense_Category']] += float(row['Amount_Due'])
            
    # Removing Expenses of Zero  
    keys = []          
    for k, val in expense_dictionary.items():
        if val == 0:
            keys.append(k)
    for k in keys:
        del expense_dictionary[k]
            
    # Making labels
    label_list = []
    for k, val in expense_dictionary.items():
        label_list.append(str(k) + '\n$' + str(round(float(val), 2)))
                
    # Writing Expense Amounts to Graph
    plt.pie(np.array(list(expense_dictionary.values())), labels = label_list)
    plt.title('Expenses for ' + str(year))
    plt.show()
             
    print('\nPie chart of expenses in ' + str(year) + ' generated')
    input('   Press enter to return to menu ')
    print('     ________________')
    print('    | Return to menu |')
    print('    |________________|\n')
    print('|/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\|\n')
    print('     ______________')
    print('    | Graph Mode |')
    print('    |    Menu    |')
    print('    |____________|\n')
    graphMode()
    
def genMonthlyBarGraph():
    close_character = 'e'
    
    # Entering Year
    year = getYear(close_character)
    
    # Reading Expense Amounts  
    expense_dictionary = {'January':0, 'February':0, 'March':0, 'April':0, 'May':0, 'June':0,
                          'July':0, 'August':0, 'September':0, 'October':0, 'November':0, 'December':0}
    file_name = "AnnualExpenseData/" + str(year) + "MonthlyExpenses.csv"
    with open(file_name, 'r', newline='') as csvfile:
        reader = csv.DictReader(csvfile)
        for row in reader:
            expense_dictionary[row['Month']] += float(row['Amount_Due'])
            
    # Writing Expense Amounts to Graph
    plt.bar(list(expense_dictionary.keys()), list(expense_dictionary.values()))
    plt.xlabel('Months')
    plt.ylabel('Expense ($)')
    plt.title('Expenses in ' + str(year))
    for k, val in expense_dictionary.items():
        plt.text(k, val, '$' + str(round(val, 2)), ha = 'center', bbox = dict(facecolor = 'grey', alpha = 0.5))
    plt.show()
             
    print('\nBar graph of monthly expenses in ' + str(year) + ' generated')
    input('   Press enter to return to menu ')
    print('     ________________')
    print('    | Return to menu |')
    print('    |________________|\n')
    print('|/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\|\n')
    print('     ______________')
    print('    | Graph Mode |')
    print('    |    Menu    |')
    print('    |____________|\n')
    graphMode()
    

'''---------------------
       Default Mode
   ---------------------'''
    
def defaultMode():
    # Picking function
    print('Enter a key from a to e for use of the following functions')
    print('   a.  Add a new expense')
    print('   b.  Search/Modify an expense')
    print('   c.  Import expense data')
    print('   d.  Add an expense type or payment method')
    print('   e.  Close the program (can be pressed anytime)\n')
    close_character = 'e'
    options = ['a', 'b', 'c', 'd']
    letter = getLetter(close_character, options)
    
    if letter == 'a':
        print('     ___________________')
        print('    | Add a new expense |')
        print('    |___________________|\n')
        addNewExpense()
    elif letter == 'b':
        print('     ______________________________________')
        print('    |       Search/Modify an expense      |')
        print('    | Enter the parameters for the search |')
        print('    |_____________________________________|\n')
        searchModifyExpense()
    elif letter == 'c':
        print('     _____________________')
        print('    | Import expense data |')
        print('    |_____________________|\n')
        importExpense()
    elif letter == 'd':
        print('     _______________________________________')
        print('    | Add an expense type or payment method |')
        print('    |_______________________________________|\n')
        addTypeOrMethod()

def addNewExpense():
    close_character = 'e'
    
    # Reading Expense Types
    expense_types = getExpenseTypes()
    
    # Reading Payment Methods
    payment_methods = getPaymentMethods()
    
    # Entering Year
    year = getYear(close_character)
    
    # Entering Month
    month = getMonth(close_character)
            
    # Entering Expense Category
    value = getExpenseCategory(close_character, expense_types)
    
    # Entering Expense Type From Category
    key = getExpenseTypeFromCategory(close_character, expense_types, value)
 
    # Entering Amount Due
    amount_due = getAmountDue(close_character)
    
    # Entering Due Date
    due_date = getDueDate(close_character, year, month)
            
    # Entering Amount Paid
    amount_paid = getAmountPaid(close_character)
    
    # Entering Payment Date
    payment_date = getPaymentDate(close_character, year, month)
    
    # Entering Payment Method
    payment_method = getPaymentMethod(close_character, payment_methods)
    
    expense_dictionary = {'Year':year, 'Month':calendar.month_name[month], 'Expense_Category':value,
                          'Expense_Type':key, 'Amount_Due':amount_due, 'Due_Date':due_date,
                          'Amount_Paid':amount_paid, 'Payment_Date':payment_date, 'Payment_Method':payment_method}
    
    # Writing Expense to File
    file_name = "AnnualExpenseData/" + str(year) + "MonthlyExpenses.csv"
    with open(file_name, 'a', newline = '') as csvfile:
         fieldnames = ['Year', 'Month', 'Expense_Category', 'Expense_Type', 'Amount_Due', 'Due_Date', 'Amount_Paid', 'Payment_Date', 'Payment_Method']
         writer = csv.DictWriter(csvfile, fieldnames=fieldnames)
         writer.writerow(expense_dictionary)
    
    # Printing messages
    print('\nExpense added to file ' + file_name.strip("AnnualExpenseData/") + ' in folder AnnualExpenseData')
    input('   Press enter to return to menu ')
    print('     ________________')
    print('    | Return to menu |')
    print('    |________________|\n')
    print('|/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\|\n')
    print('     ______________')
    print('    | Default Mode |')
    print('    |     Menu     |')
    print('    |______________|\n')
    defaultMode()
    
def searchModifyExpense():  
    close_character = 'e'
    
    expense_types = getExpenseTypes()
    payment_methods = getPaymentMethods()
    
    while True:
        parameters = []
        parameters = getSearchParameters(close_character)
        parameter_values = {'Year':0, 'Month':0, 'Expense_Category':0, 'Expense_Type':0, 'Amount_Due':0,
                            'Due_Date':0, 'Amount_Paid':0, 'Payment_Date':0, 'Payment_Method':0}
        
        print('     _____________________________________________________')
        print('    | Enter the prompted values for each chosen parameter |')
        print('    |_____________________________________________________|\n')
        for parameter in parameters:
            if parameter == 'Year':
                parameter_values[parameter] = getYear(close_character)
            elif parameter == 'Month':
                parameter_month_num = getMonth(close_character)
                parameter_values[parameter] = calendar.month_name[parameter_month_num]
            elif parameter == 'Expense_Category':
                parameter_values[parameter] = getExpenseCategory(close_character, expense_types)
            elif parameter == 'Expense_Type':
                parameter_values[parameter] = getExpenseTypeFromCategory(close_character, expense_types, parameter_values['Expense_Category'])
            elif parameter == 'Amount_Due':
                parameter_values[parameter] = getAmountDue(close_character) 
            elif parameter == 'Due_Date':
                parameter_values[parameter] = getDueDate(close_character, parameter_values['Year'], parameter_month_num)
            elif parameter == 'Amount_Paid':
                parameter_values[parameter] = getAmountPaid(close_character)
            elif parameter == 'Payment_Date':
                parameter_values[parameter] = getPaymentDate(close_character, parameter_values['Year'], parameter_month_num)
            elif parameter == 'Payment_Method':
                parameter_values[parameter] = getPaymentMethod(close_character, payment_methods)
                
        if parameter_values['Year'] == 0:
            # Reading Expense Amounts    
            expense_dictionaries = []
            for year in range(1980, 2023):
                file_name = "AnnualExpenseData/" + str(year) + "MonthlyExpenses.csv"
                with open(file_name, 'r', newline='') as csvfile:
                    reader = csv.DictReader(csvfile)
                    for row in reader:
                        match = True
                        for parameter in parameters:
                            if not str(row[parameter]) == str(parameter_values[parameter]):
                                match = False
                        if match:
                            expense_dictionaries.append(row)
        else:
            expense_dictionaries = []
            file_name = "AnnualExpenseData/" + str(parameter_values['Year']) + "MonthlyExpenses.csv"
            with open(file_name, 'r', newline='') as csvfile:
                reader = csv.DictReader(csvfile)
                for row in reader:
                    match = True
                    for parameter in parameters:
                        if not str(row[parameter]) == str(parameter_values[parameter]):
                            match = False
                    if match:
                        expense_dictionaries.append(row)
        
        if len(expense_dictionaries) > 10:
            print('\nMatching expense entries exceeds 10. Enter the p key to print the expenses or the c key to change the parameters')
            print('   p.  Print the expenses')
            print('   c.  Change parameters\n')
            close_character = 'e'
            options = ['p', 'c']
            letter = getLetter(close_character, options)
                    
            if letter == 'p':
                print('     ____________________')
                print('    | Print the expenses |')
                print('    |____________________|\n')
                printExpenses(expense_dictionaries)
                break
            elif letter == 'c':
                print('     ___________________')
                print('    | Change parameters |')
                print('    |___________________|\n')
        else:
            print('     ____________________')
            print('    | Print the expenses |')
            print('    |____________________|\n')
            printExpenses(expense_dictionaries)
            break
    
    while True:
        print('Enter the m key to modify one of the printed expenses or the r key to return to the menu')
        print('   m.  Modify an expense')
        print('   r.  Return to menu\n')
        close_character = 'e'
        options = ['m', 'r']
        letter = getLetter(close_character, options)
                
        if letter == 'm':
            print('     _________________________')
            print('    |    Modify an expense    |')
            print('    | Enter the expense index |')
            print('    |_________________________|\n')
            modifyExpense(expense_dictionaries, expense_types, payment_methods)
        elif letter == 'r':
            print('     ________________')
            print('    | Return to menu |')
            print('    |________________|\n')
            print('|/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\|\n')
            print('     ______________')
            print('    | Default Mode |')
            print('    |     Menu     |')
            print('    |______________|\n')
            defaultMode()
        
def printExpenses(expense_dictionaries: list):
    for expense_dictionary in expense_dictionaries:
        index = expense_dictionaries.index(expense_dictionary) + 1
        print('   ' + str(index) + '.\n' + str(expense_dictionary) + '\n')
        
def modifyExpense(expense_dictionaries: list, expense_types: dict, payment_methods: list):
    close_character = 'e'
    
    while True:
        # Entering index
        index_max = len(expense_dictionaries)
        index = getIndex(close_character, index_max)
        
        expense_dictionary = expense_dictionaries[index - 1]
        old_expense_dictionary = expense_dictionary.copy()
        
        while True:
            print('\n' + str(expense_dictionary) + '\n')
            
            print('     ________________________________')
            print('    | Enter the parameters to modify |')
            print('    |________________________________|\n')
            
            parameters = []
            parameters = getModifyParameters(close_character)
            parameter_values = {'Year':-1, 'Month':-1, 'Expense_Category':-1, 'Expense_Type':-1, 'Amount_Due':-1,
                                'Due_Date':-1, 'Amount_Paid':-1, 'Payment_Date':-1, 'Payment_Method':-1}
            
            print('     _____________________________________________________')
            print('    | Enter the prompted values for each chosen parameter |')
            print('    |_____________________________________________________|\n')
            for parameter in parameters:
                if parameter == 'Expense_Type':
                    if 'Expense_Category' in parameters:
                        parameter_values['Expense_Category'] = getExpenseCategory(close_character, expense_types)
                        parameter_values[parameter] = getExpenseTypeFromCategory(close_character, expense_types, parameter_values['Expense_Category'])
                    else:
                        parameter_values[parameter] = getExpenseTypeFromCategory(close_character, expense_types, expense_dictionary['Expense_Category'])
                elif parameter == 'Amount_Due':
                    parameter_values[parameter] = getAmountDue(close_character) 
                elif parameter == 'Due_Date' or parameter == 'Payment_Date':
                    parameter_one = 'Due_Date'
                    parameter_two = 'Payment_Date'
                    if 'Year' in parameters:
                        parameter_values['Year'] = getYear(close_character)
                        if 'Month' in parameters:
                            parameter_month_num = getMonth(close_character)
                            parameter_values['Month'] = calendar.month_name[parameter_month_num]
                            parameter_values[parameter_one] = getDueDate(close_character, parameter_values['Year'], parameter_month_num)
                            parameter_values[parameter_two] = getPaymentDate(close_character, parameter_values['Year'], parameter_month_num)
                            if parameter == 'Due_Date':
                                parameters.remove('Payment_Date')
                            elif parameter == 'Payment_Date':
                                parameters.remove('Due_Date')
                        else:
                            expense_month_num = getMonthNum(expense_dictionary)
                            parameter_values[parameter_one] = getDueDate(close_character, parameter_values['Year'], expense_month_num)
                            parameter_values[parameter_two] = getPaymentDate(close_character, parameter_values['Year'], expense_month_num)
                            if parameter == 'Due_Date':
                                parameters.remove('Payment_Date')
                            elif parameter == 'Payment_Date':
                                parameters.remove('Due_Date')
                    else:
                        if 'Month' in parameters:
                            parameter_month_num = getMonth(close_character)
                            parameter_values['Month'] = calendar.month_name[parameter_month_num]
                            parameter_values[parameter_one] = getDueDate(close_character, int(expense_dictionary['Year']), parameter_month_num)
                            parameter_values[parameter_two] = getPaymentDate(close_character, int(expense_dictionary['Year']), parameter_month_num)
                            if parameter == 'Due_Date':
                                parameters.remove('Payment_Date')
                            elif parameter == 'Payment_Date':
                                parameters.remove('Due_Date')
                        else:
                            expense_month_num = getMonthNum(expense_dictionary)
                            if 'Due_Date' in parameters and 'Payment_Date' in parameters:
                                parameter_values[parameter_one] = getDueDate(close_character, int(expense_dictionary['Year']), expense_month_num)
                                parameter_values[parameter_two] = getPaymentDate(close_character, int(expense_dictionary['Year']), expense_month_num)
                                if parameter == 'Due_Date':
                                    parameters.remove('Payment_Date')
                                elif parameter == 'Payment_Date':
                                    parameters.remove('Due_Date')
                            elif parameter == 'Due_Date':
                                parameter_values[parameter] = getDueDate(close_character, int(expense_dictionary['Year']), expense_month_num)
                            elif parameter == 'Payment_Date':
                                parameter_values[parameter] = getPaymentDate(close_character, int(expense_dictionary['Year']), expense_month_num)      
                elif parameter == 'Amount_Paid':
                    parameter_values[parameter] = getAmountPaid(close_character)
                elif parameter == 'Payment_Method':
                    parameter_values[parameter] = getPaymentMethod(close_character, payment_methods)
                
            for k, val in parameter_values.items():
                if not val == -1:
                    expense_dictionary[k] = val       
            print('\n' + str(expense_dictionary) + '\n')
            print('Enter the s key to save the expense changes or the r key to redo the expense changes')
            print('   s.  Save the expense changes')
            print('   r.  Redo the expense changes\n')
            close_character = 'e'
            options = ['s', 'r']
            letter = getLetter(close_character, options)
                    
            if letter == 's':
                print('     __________________________')
                print('    | Save the expense changes |')
                print('    |__________________________|\n')
                break
            elif letter == 'r':
                print('     __________________________')
                print('    | Redo the expense changes |')
                print('    |__________________________|\n')
        
        new_expense_dictionary = expense_dictionary.copy()
        
        if old_expense_dictionary['Year'] == new_expense_dictionary['Year']:
            expense_dictionaries_file = []
            file_name = "AnnualExpenseData/" + str(old_expense_dictionary['Year']) + "MonthlyExpenses.csv"
            with open(file_name, 'r', newline='') as csvfile:
                reader = csv.DictReader(csvfile)
                for row in reader:
                    if row == old_expense_dictionary:
                        expense_dictionaries_file.append(new_expense_dictionary)
                    else:
                        expense_dictionaries_file.append(row)
            
            # Writing Expense to File
            file_name = "AnnualExpenseData/" + str(new_expense_dictionary['Year']) + "MonthlyExpenses.csv"
            with open(file_name, 'w', newline = '') as csvfile:
                 fieldnames = ['Year', 'Month', 'Expense_Category', 'Expense_Type', 'Amount_Due', 'Due_Date', 'Amount_Paid', 'Payment_Date', 'Payment_Method']
                 writer = csv.DictWriter(csvfile, fieldnames=fieldnames)
                 writer.writeheader()
                 for expense_dictionary in expense_dictionaries_file:
                     writer.writerow(expense_dictionary)
        else:
            expense_dictionaries_file = []
            file_name = "AnnualExpenseData/" + str(old_expense_dictionary['Year']) + "MonthlyExpenses.csv"
            with open(file_name, 'r', newline='') as csvfile:
                reader = csv.DictReader(csvfile)
                for row in reader:
                    if not row == old_expense_dictionary:
                        expense_dictionaries_file.append(row)
            
            # Writing Expense to File
            file_name = "AnnualExpenseData/" + str(old_expense_dictionary['Year']) + "MonthlyExpenses.csv"
            with open(file_name, 'w', newline = '') as csvfile:
                 fieldnames = ['Year', 'Month', 'Expense_Category', 'Expense_Type', 'Amount_Due', 'Due_Date', 'Amount_Paid', 'Payment_Date', 'Payment_Method']
                 writer = csv.DictWriter(csvfile, fieldnames=fieldnames)
                 writer.writeheader()
                 for expense_dictionary in expense_dictionaries_file:
                     writer.writerow(expense_dictionary)
            
            expense_dictionaries_file = []
            file_name = "AnnualExpenseData/" + str(new_expense_dictionary['Year']) + "MonthlyExpenses.csv"
            with open(file_name, 'r', newline='') as csvfile:
                reader = csv.DictReader(csvfile)
                for row in reader:
                    expense_dictionaries_file.append(row)
            expense_dictionaries_file.append(new_expense_dictionary)
                
            # Writing Expense to File
            file_name = "AnnualExpenseData/" + str(new_expense_dictionary['Year']) + "MonthlyExpenses.csv"
            with open(file_name, 'w', newline = '') as csvfile:
                 fieldnames = ['Year', 'Month', 'Expense_Category', 'Expense_Type', 'Amount_Due', 'Due_Date', 'Amount_Paid', 'Payment_Date', 'Payment_Method']
                 writer = csv.DictWriter(csvfile, fieldnames=fieldnames)
                 writer.writeheader()
                 for expense_dictionary in expense_dictionaries_file:
                     writer.writerow(expense_dictionary)
        
        expense_dictionaries.remove(new_expense_dictionary)
        if len(expense_dictionaries) == 0:
            print('     ________________')
            print('    | Return to menu |')
            print('    |________________|\n')
            print('|/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\|\n')
            print('     ______________')
            print('    | Default Mode |')
            print('    |     Menu     |')
            print('    |______________|\n')
            defaultMode()
        else:
            print('Enter the m key to modify another expense or the r key to return to the menu')
            print('   m.  Modify another expense')
            print('   r.  Return to menu\n')
            close_character = 'e'
            options = ['m', 'r']
            letter = getLetter(close_character, options)
                    
            if letter == 'm':
                print('     ________________________')
                print('    | Modify another expense |')
                print('    |________________________|\n')
                printExpenses(expense_dictionaries)
            elif letter == 'r':
                print('     ________________')
                print('    | Return to menu |')
                print('    |________________|\n')
                print('|/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\|\n')
                print('     ______________')
                print('    | Default Mode |')
                print('    |     Menu     |')
                print('    |______________|\n')
                defaultMode()

def importExpense():
    print('Import expense data function not developed')
    input('   Press enter to return to menu ')
    print('     ________________')
    print('    | Return to menu |')
    print('    |________________|\n')
    print('|/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\|\n')
    print('     ______________')
    print('    | Default Mode |')
    print('    |     Menu     |')
    print('    |______________|\n')
    defaultMode()
    
def addTypeOrMethod():
    print('Enter the t key to add an expense type or the m key to add a payment method')
    print('   t.  Add a new expense type')
    print('   m.  Add a new payment method\n')
    close_character = 'e'
    options = ['t', 'm']
    letter = getLetter(close_character, options)
            
    if letter == 't':
        print('     __________________')
        print('    | Add expense type |')
        print('    |__________________|\n')
        addExpenseType()
    elif letter == 'm':
        print('     ____________________')
        print('    | Add payment method |')
        print('    |____________________|\n')
        addPaymentMethod()
        
def addExpenseType():
    close_character = 'e'
    
    # Reading Expense Types
    expense_types = getExpenseTypes()
    
    # Entering new Expense Category
    value = getNewExpenseCategory(close_character, expense_types)
    
    # Entering new Expense Type
    key = getNewExpenseType(close_character, expense_types, value)
            
    # Writing Expense Type to File        
    with open('ExpenseTypes.csv', 'a', newline='') as csvfile:
        fieldnames = ['Expense_Type', 'Expense_Category']
        writer = csv.DictWriter(csvfile, fieldnames=fieldnames)
        writer.writerow({'Expense_Type':key, 'Expense_Category':value})
        
    print('\nExpense type added to file ExpenseTypes.csv')
    input('   Press enter to return to menu ')
    print('     ________________')
    print('    | Return to menu |')
    print('    |________________|\n')
    print('|/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\|\n')
    print('     ______________')
    print('    | Default Mode |')
    print('    |     Menu     |')
    print('    |______________|\n')
    defaultMode()
    
def addPaymentMethod():
    close_character = 'e'
    
    # Reading Payment Methods
    payment_methods = getPaymentMethods()
    
    # Entering new Payment Method
    payment_method = getNewPaymentMethod(close_character, payment_methods)
    
    # Writing Payment Method to File
    payment_method = [payment_method]
    with open('PaymentMethods.csv', 'a', newline='') as csvfile:
        writer = csv.writer(csvfile)
        writer.writerow(payment_method)
        
    print('\nPayment method added to file PaymentMethods.csv')
    input('   Press enter to return to menu ')
    print('     ________________')
    print('    | Return to menu |')
    print('    |________________|\n')
    print('|/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\|\n')
    print('     ______________')
    print('    | Default Mode |')
    print('    |     Menu     |')
    print('    |______________|\n')
    defaultMode()
    

'''---------------------
      Getter Functions
   ---------------------'''
   
def getLetter(close_character: str, options: list):
    while True:
        try:
            letter = str(input('   - '))
            closeCheck(letter, close_character)
            if letter in options:
                return letter
            else:
                print("\nInvalid entry: Submit a letter from the options")
                input('   Press enter to continue ')
                print('')
        except (ValueError, RuntimeError, TypeError, NameError):
            print("\nInvalid entry: Submit a letter from the options")
            input('   Press enter to continue ')
            print('')

def getYear(close_character: str):
    # Entering Year
    while True:
        try:
            year = input('Enter the year\n   - ')
            closeCheck(year, close_character)
            year = int(year)
            if year >= 1980 and year <= 2022:
                return year
            else:
                print('\nInvalid year: Submit a number between 1980 and 2022')
                input('   Press enter to continue ')
                print('')
        except (ValueError, RuntimeError, TypeError, NameError):
            print('\nInvalid year: Submit a number between 1980 and 2022')
            input('   Press enter to continue ')
            print('')
    
def getStartYear(close_character: str):
    # Entering Start Year
    while True:
        try:
            start_year = input('Enter the start year\n   - ')
            closeCheck(start_year, close_character)
            start_year = int(start_year)
            if start_year >= 1980 and start_year <= 2022:
                return start_year
            else:
                print('\nInvalid year: Submit a number between 1980 and 2022')
                input('   Press enter to continue ')
                print('')
        except (ValueError, RuntimeError, TypeError, NameError):
            print('\nInvalid year: Submit a number between 1980 and 2022')
            input('   Press enter to continue ')
            print('')
            
def getEndYear(close_character: str, start_year: int):
    # Entering End Year
    while True:
        try:
            end_year = input('Enter the end year\n   - ')
            closeCheck(end_year, close_character)
            end_year = int(end_year)
            if end_year >= 1980 and end_year <= 2022 and end_year >= start_year:
                return end_year
            else:
                print('\nInvalid year: Submit a number between 1980 and 2022 that is not less than the start year')
                input('   Press enter to continue ')
                print('')
        except (ValueError, RuntimeError, TypeError, NameError):
            print('\nInvalid year: Submit a number between 1980 and 2022 that is not less than the start year')
            input('   Press enter to continue ')
            print('')
            
def getMonth(close_character: str):
    # Entering Month
    while True:
        try:
            month = input('Enter the number of the month of the expense\n   - ')
            closeCheck(month, close_character)
            month = int(month)
            if month >= 1 and month <= 12:
                return month
            else:
                print('\nInvalid month: Submit a number between 1 and 12')
                input('   Press enter to continue ')
                print('')
        except (ValueError, RuntimeError, TypeError, NameError):
            print('\nInvalid month: Submit a number between 1 and 12')
            input('   Press enter to continue ')
            print('')
            
def getMonthNum(expense_dictionary: dict):
    # Getting month number
    if expense_dictionary['Month'] == 'January':
        return 1
    elif expense_dictionary['Month'] == 'February':
        return 2
    elif expense_dictionary['Month'] == 'March':
        return 3
    elif expense_dictionary['Month'] == 'April':
        return 4
    elif expense_dictionary['Month'] == 'May':
        return 5
    elif expense_dictionary['Month'] == 'June':
        return 6
    elif expense_dictionary['Month'] == 'July':
        return 7
    elif expense_dictionary['Month'] == 'August':
        return 8
    elif expense_dictionary['Month'] == 'September':
        return 9
    elif expense_dictionary['Month'] == 'October':
        return 10
    elif expense_dictionary['Month'] == 'November':
        return 11
    elif expense_dictionary['Month'] == 'December':
        return 12
    
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

def getExpenseCategory(close_character: str, expense_types: dict):
    # Entering Expense Category
    while True:
        try:
            print('Enter the category of the expense from the options below\n')
            print(list(set(expense_types.values())))
            value = str(input('\n   - '))
            closeCheck(value, close_character)
            if value in set(expense_types.values()):
                return value
            else:
                print('\nInvalid category: Submit a category from the options')
                input('   Press enter to continue ')
                print('')
        except (ValueError, RuntimeError, TypeError, NameError):
            print('\nInvalid category: Submit a category from the options')
            input('   Press enter to continue ')
            print('')

def getNewExpenseCategory(close_character: str, expense_types: dict):
    # Entering new Expense Category
    while True:
        try:
            print('Enter a new expense category or one of the current categories shown below\n')
            print(list(set(expense_types.values())))
            value = str(input('\n   - '))
            closeCheck(value, close_character)
            return value
        except (ValueError, RuntimeError, TypeError, NameError):
            print('\nInvalid category: Submit a string')
            input('   Press enter to continue ')
            print('')
            
def getExpenseTypeFromCategory(close_character: str, expense_types: dict, value: str):
    # Entering Expense Type From Category
    while True:
        try:
            keys = []
            for k, val in expense_types.items():
                if val == value:
                    keys.append(k)
            print('Enter the type of the expense from the options below\n')
            print(keys)
            key = str(input('\n   - '))
            closeCheck(key, close_character)
            if key in keys:
                return key
            else:
                print('\nInvalid type: Submit a type from the options')
                input('   Press enter to continue ')
                print('')
        except (ValueError, RuntimeError, TypeError, NameError):
            print('\nInvalid type: Submit a type from the options')
            input('   Press enter to continue ')
            print('')
            
def getExpenseType(close_character: str, expense_types: dict):
    # Entering Expense Type
    while True:
        try:
            keys = []
            for k in expense_types.keys():
                keys.append(k)
            print('Enter the type of the expenses from the options below\n')
            print(keys)
            key = str(input('\n   - '))
            closeCheck(key, close_character)
            if key in keys:
                return key
            else:
                print('\nInvalid type: Submit a type from the options')
                input('   Press enter to continue ')
                print('')
        except (ValueError, RuntimeError, TypeError, NameError):
            print('\nInvalid type: Submit a type from the options')
            input('   Press enter to continue ')
            print('')
            
def getNewExpenseType(close_character: str, expense_types: dict, value: str):
    # Entering new Expense Type
    while True:
        try:
            keys = []
            for k, val in expense_types.items():
                if val == value:
                    keys.append(k)
            print('Enter a new expense type (any current types are shown below for reference)\n')
            print(keys)
            key = str(input('\n   - '))
            closeCheck(key, close_character)
            if key in keys:
                print('\nInvalid expense type: Submit a new type not from the options')
                input('   Press enter to continue ')
                print('')
            else:
                return key
        except (ValueError, RuntimeError, TypeError, NameError):
            print('\nInvalid expense type: Submit a new type not from the options')
            input('   Press enter to continue ')
            print('')
            
def getAmountDue(close_character: str):
    # Entering Amount Due
    while True:
        try:
            amount_due = input('Enter the amount due from the expense\n   - ')
            closeCheck(amount_due, close_character)
            amount_due = round(float(amount_due), 2)
            if amount_due >= 0:
                return amount_due
            else:
                print('\nInvalid amount due: Submit a number greater than zero')
                input('   Press enter to continue ')
                print('')
        except (ValueError, RuntimeError, TypeError, NameError):
            print('\nInvalid amount due: Submit a number greater than zero')
            input('   Press enter to continue ')
            print('')
            
def getDueDate(close_character: str, year: int, month: int):
    # Entering Due Date
    while True:
        try:
            due_date = input('Enter the due date of the expense\n   - ')
            closeCheck(due_date, close_character)
            due_date = int(due_date)
            dates = []
            for day in calendar.Calendar().itermonthdays(year,month):
                if day > 0:
                    dates.append(day)
            if due_date in dates:
                return due_date
            else:
                print('\nInvalid due date: Submit a number between 1 and the end of the month')
                input('   Press enter to continue ')
                print('')
        except (ValueError, RuntimeError, TypeError, NameError):
            print('\nInvalid due date: Submit a number between 1 and the end of the month')
            input('   Press enter to continue ')
            print('')
            
def getAmountPaid(close_character: str):
    # Entering Amount Paid
    while True:
        try:
            amount_paid = input('Enter the amount paid\n   - ')
            closeCheck(amount_paid, close_character)
            amount_paid = round(float(amount_paid), 2)
            if amount_paid >= 0:
                return amount_paid
            else:
               print('\nInvalid amount paid: Submit a number greater than zero')
               input('   Press enter to continue ')
               print('') 
        except (ValueError, RuntimeError, TypeError, NameError):
            print('\nInvalid amount paid: Submit a number greater than zero')
            input('   Press enter to continue ')
            print('')
            
def getPaymentDate(close_character: str, year: int, month: int):
    # Entering Payment Date
    while True:
        try:
            payment_date = input('Enter the payment date of the expense or 0 if not applicable\n   - ')
            closeCheck(payment_date, close_character)
            payment_date = int(payment_date)
            dates = []
            for day in calendar.Calendar().itermonthdays(year,month):
                if day > 0:
                    dates.append(day)
            if payment_date in dates or payment_date == 0:
                return payment_date
            else:
                print('\nInvalid payment date: Submit a number between 1 and the end of the month')
                input('   Press enter to continue ')
                print('')
        except (ValueError, RuntimeError, TypeError, NameError):
            print('\nInvalid payment date: Submit a number between 1 and the end of the month')
            input('   Press enter to continue ')
            print('')
            
def getPaymentMethod(close_character: str, payment_methods: list):
    # Entering Payment Method
    while True:
        try:
            print('Enter the payment method of the expense from the options below\n')
            print(payment_methods)
            payment_method = str(input('\n   - '))
            closeCheck(payment_method, close_character)
            if payment_method in payment_methods:
                return payment_method
            else:
                print('\nInvalid payment method: Submit a payment method from the options')
                input('   Press enter to continue ')
                print('')
        except (ValueError, RuntimeError, TypeError, NameError):
            print('\nInvalid payment method: Submit a payment method from the options')
            input('   Press enter to continue ')
            print('')
            
def getNewPaymentMethod(close_character: str, payment_methods: list):
    # Entering new Payment Method
    while True:
        try:
            print('Enter a new payment method (current methods shown below as reference)\n')
            print(payment_methods)
            payment_method = str(input('\n   - '))
            closeCheck(payment_method, close_character)
            if payment_method in payment_methods:
                print('\nInvalid payment method: Submit a new method not from the options')
                input('   Press enter to continue ')
                print('')
            else:
                return payment_method
        except (ValueError, RuntimeError, TypeError, NameError):
            print('\nInvalid payment method: Submit a new method not from the options')
            input('   Press enter to continue ')
            print('')
            
def getSearchParameters(close_character: str):
    # Entering Parameter
    parameter_options = ['Year', 'Month', 'Expense_Category', 'Expense_Type', 'Amount_Due', 'Due_Date',
                         'Amount_Paid', 'Payment_Date', 'Payment_Method']
    parameters = []
    while True:
        try:
            print('Enter one of the parameters from the options below or enter Done to finish\n')
            print(parameter_options)
            parameter = str(input('\n   - '))
            closeCheck(parameter, close_character)
            if parameter in parameter_options and parameter not in parameters:
                if (parameter == 'Due_Date' or parameter == 'Payment_Date') and not ('Year' in parameters and 'Month' in parameters):
                    print('\nInvalid parameter: Submit year and month before date')
                    input('   Press enter to continue ')
                    print('')
                elif (parameter == 'Expense_Type') and not ('Expense_Category' in parameters):
                    print('\nInvalid parameter: Submit expense category before type')
                    input('   Press enter to continue ')
                    print('')
                else:
                    parameters.append(parameter)
            elif parameter == 'Done' and len(parameters) > 0:
                return parameters
            else:
                print('\nInvalid parameter: Submit a parameter from the options')
                input('   Press enter to continue ')
                print('')
        except (ValueError, RuntimeError, TypeError, NameError):
            print('\nInvalid parameter: Submit a parameter from the options')
            input('   Press enter to continue ')
            print('')
            
def getIndex(close_character: str, index_max: int):
    # Entering Index
    while True:
        try:
            index = input('Enter the index of the expense to be modified\n   - ')
            closeCheck(index, close_character)
            index = int(index)
            if index <= index_max and index > 0:
                return index
            else:
                print('\nInvalid index: Submit a number between 1 and the highest index')
                input('   Press enter to continue ')
                print('')
        except (ValueError, RuntimeError, TypeError, NameError):
            print('\nInvalid index: Submit a number between 1 and the highest index')
            input('   Press enter to continue ')
            print('')
            
def getModifyParameters(close_character: str):
    # Entering Parameter
    parameter_options = ['Year', 'Month', 'Expense_Category', 'Expense_Type', 'Amount_Due', 'Due_Date',
                         'Amount_Paid', 'Payment_Date', 'Payment_Method']
    parameters = []
    while True:
        try:
            print('Enter one of the parameters from the options below or enter Done to finish\n')
            print(parameter_options)
            parameter = str(input('\n   - '))
            closeCheck(parameter, close_character)
            if parameter in parameter_options and parameter not in parameters:
                if (parameter == 'Expense_Category') and not ('Expense_Type' in parameters):
                    print('\nInvalid parameter: Submit expense type before category')
                    input('   Press enter to continue ')
                    print('')
                elif (parameter == 'Year' or parameter == 'Month') and not ('Due_Date' in parameters and 'Payment_Date' in parameters):
                    print('\nInvalid parameter: Submit due date and payment date before year or month')
                    input('   Press enter to continue ')
                    print('') 
                else:
                    parameters.append(parameter)
            elif parameter == 'Done' and len(parameters) > 0:
                return parameters
            else:
                print('\nInvalid parameter: Submit a parameter from the options')
                input('   Press enter to continue ')
                print('')
        except (ValueError, RuntimeError, TypeError, NameError):
            print('\nInvalid parameter: Submit a parameter from the options')
            input('   Press enter to continue ')
            print('')


'''---------------------
      Closing Function
   ---------------------'''
        
def closeCheck(input_string: str, close_character: str):
    if input_string == close_character:
        print('     ___________________')
        print('    | Close the program |')
        print('    |___________________|\n')
        sys.exit(0)


'''---------------------
      Running Main
   ---------------------'''

if __name__ == "__main__":
    try:
        main()
    except KeyboardInterrupt:
        print('\n     ___________________')
        print('    | Close the program |')
        print('    |___________________|\n')
        try:
            sys.exit(0)
        except SystemExit:
            os._exit(0)
        