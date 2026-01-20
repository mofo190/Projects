#VAE -> Voting Analysis Engine

#Imported Packages used throughout program
import sys
import pandas as pd
import matplotlib.pyplot as plt

#Class Declarations
class Country:
    def __init__(self, name, totalVotes, validVotes, invalidVotes, maleMps, femaleMps, constituencies, partyVotes):
        self.name = name
        self.totalVotes = totalVotes
        self.validVotes = validVotes
        self.invalidVotes = invalidVotes
        self.maleMps = maleMps
        self.femaleMps = femaleMps
        self.constituencies = constituencies
        self.partyVotes = partyVotes

class Region:
    def __init__(self, name, totalVotes, validVotes, invalidVotes, maleMps, femaleMps, constituencies, partyVotes):
        self.name = name
        self.totalVotes = totalVotes
        self.validVotes = validVotes
        self.invalidVotes = invalidVotes
        self.maleMps = maleMps
        self.femaleMps = femaleMps
        self.constituencies = constituencies
        self.partyVotes = partyVotes

class Constituency:
    def __init__(self, name, constituencyType, result, firstParty, firstPartyVotes, secondParty, secondPartyVotes, leastParty, country, region):
        self.name = name
        self.constituencyType = constituencyType
        self.result = result
        self.firstParty = firstParty
        self.firstPartyVotes = firstPartyVotes
        self.secondParty = secondParty
        self.secondPartyVotes = secondPartyVotes
        self.leastParty = leastParty
        self.country = country
        self.region = region

class Party:
    def __init__(self, name, maleMps, femaleMps, totalVotes, partyWins):
        self.name = name
        self.maleMps = maleMps
        self.femaleMps = femaleMps
        self.totalVotes = totalVotes
        self.partyWins = partyWins

class MP:
    def __init__(self, firstName, lastName, gender, country, region, constituency, party, votes):
        self.firstName = firstName
        self.lastName = lastName
        self.gender = gender
        self.country = country
        self.region = region
        self.constituency = constituency
        self.party = party
        self.votes = votes


# Load file functions w/ exception handling
def load_data(VAData):
    try:
        df = pd.read_csv(VAData)
        print("File loaded successfully.")
        return df
    except FileNotFoundError:
        print("Error: File not found at", VAData)
        sys.exit()
    except Exception as e:
        print("Error occurred:", e)
        sys.exit()

# Menu - Once chosen allows for more specific requests
def prime_menu():
    print("\n-- House of Commons 2024 - Voting Analysis Engine --")
    print("1. View Data")
    print("2. Comparison Calculator")
    print("3. View Graphical Data")
    print("4. Exit")

# Sub-menu for option 1 - View Data
def viewData_menu():
    print("\n-- 1. View Data --")
    print("1. View Constituency Data")
    print("2. View Party Data")
    print("3. View MP Data")
    print("4. Return to main menu")

# Sub-menu for option 2 - Comparison Calculator
def compData_menu():
    print("\n-- 2. Comparison Calculator --")
    print("1. Compare Country")
    print("2. Compare Constituency")
    print("3. Compare MP")
    print("4. Return to Calculate from Data")

# Sub-menu for option 3 - View Graphical Data
def graphData_menu():
    print("\n-- 3. View Graphical Data --")
    print("1. View Graph - Male vs Female per Party")
    print("2. View Graph - Wins per Party")
    print("3. View Graph - Electorate per Constituency")
    print("4. Return to main menu")


# Functions: Option #1 - View Data

# Function to display data for Constituency using class
def viewConstituencyData(df):


    constituencyName = input("Enter name of constituency: ")

    # Filter dataset for specified constituency
    constituencyData = df[df['Constituency'] == constituencyName]

    # Fetching data for Constituency and Instantiating class
    constituency = Constituency(
        name=constituencyName,
        constituencyType=constituencyData['Constituency type'].values[0],
        result=constituencyData['Result'].values[0],
        firstParty=constituencyData['First Party'].values[0],
        firstPartyVotes=constituencyData[constituencyData['First Party'].values[0]].values[0],
        secondParty=constituencyData['Second Party'].values[0],
        secondPartyVotes=constituencyData[constituencyData['Second Party'].values[0]].values[0],
        leastParty=constituencyData.iloc[:, 14:27].sum().idxmin(),
        country=constituencyData['Country'].values[0],
        region=constituencyData['Region'].values[0]
    )

    # Display Constituency information
    print("Constituency Details:")
    print("Name:", constituency.name)
    print("Type:", constituency.constituencyType)
    print("Result:", constituency.result)
    print("First Party:", constituency.firstParty, "with", constituency.firstPartyVotes, "votes")
    print("Second Party:", constituency.secondParty, "with", constituency.secondPartyVotes, "votes")
    print("Least Party:", constituency.leastParty)
    print("Country:", constituency.country)
    print("Region:", constituency.region)

# Function to display data for Party using class
def viewPartyData(df):

    partyName = input("Enter name of the party: ")

    # Fetching data for Party and Instantiating class
    party = Party(
        name=partyName,
        maleMps=df[(df['MP Gender'] == 'Male') & (df['First Party'] == partyName)].shape[0],
        femaleMps=df[(df['MP Gender'] == 'Female') & (df['First Party'] == partyName)].shape[0],
        totalVotes=df[partyName].sum(),
        partyWins=df[df['First Party'] == partyName].shape[0]
    )

    # Display Party information
    print("Party Details:")
    print("Name:", party.name)
    print("Male MPs:", party.maleMps)
    print("Female MPs:", party.femaleMps)
    print("Total Votes:", party.totalVotes)
    print("Wins:", party.partyWins)

# Function displays data for MP using class
def viewMpData(df):

    # User enters name of MP
    mpName = input("Enter last name of MP: ")

    # Filter data file for specified MP
    mpData = df[df['MP Last Name'].str.contains(mpName)]

    # Fetch data for MP and instantiate class
    mp = MP(
        firstName=mpData['MP First Name'].values[0],
        lastName=mpData['MP Last Name'].values[0],
        gender=mpData['MP Gender'].values[0],
        country=mpData['Country'].values[0],
        region=mpData['Region'].values[0],
        constituency=mpData['Constituency'].values[0],
        party=mpData['First Party'].values[0],
        votes=mpData[mpData['First Party'].values[0]].values[0]
    )

    # Display MP information
    print("MP Details:")
    print("First Name:", mp.firstName)
    print("Last Name:", mp.lastName)
    print("Gender:", mp.gender)
    print("Country:", mp.country)
    print("Region:", mp.region)
    print("Constituency:", mp.constituency)
    print("Party:", mp.party)
    print("Votes:", mp.votes)

# Functions: Option #2 - Comparison Calculator

# Functions to compare two Countries
def compareCountries(df):

    # User enters two countries
    country1 = input("Enter first country: ")
    country2 = input("Enter second country: ")

    # Filters dataset for both countries
    data1 = df[df['Country'] == country1]
    data2 = df[df['Country'] == country2]

    # Data Calculations

    #Sum of electorate in each country
    electorate1 = data1['Electorate'].sum()
    electorate2 = data2['Electorate'].sum()

    # Sum of valid votes in each country
    validVotes1 = data1['Valid Votes'].sum()
    validVotes2 = data2['Valid Votes'].sum()

    # Sum of invalid votes in each country
    invalidVotes1 = data1['Invalid Votes'].sum()
    invalidVotes2 = data2['Invalid Votes'].sum()

    # Voter turnout in each country
    turnout1 = ((validVotes1 + invalidVotes1) / electorate1) * 100
    turnout2 = ((validVotes2 + invalidVotes2) / electorate2) * 100

    # Percentage of valid votes in each country
    ValpercV1 = (validVotes1 / (validVotes1 + invalidVotes1)) * 100
    ValpercV2 = (validVotes2 / (validVotes2 + invalidVotes2)) * 100

    # Percentage of invalid votes in each country
    InvpercV1 = (invalidVotes1 / (validVotes1 + invalidVotes1)) * 100
    InvpercV2 = (invalidVotes2 / (validVotes2 + invalidVotes2)) * 100

    # Count of all male MPs in country 1
    maleMps1 = data1[data1['MP Gender'] == 'Male'].shape[0]
    maleMps2 = data2[data2['MP Gender'] == 'Male'].shape[0]

    # Count of all male MPs in country 2
    femaleMps1 = data1[data1['MP Gender'] == 'Female'].shape[0]
    femaleMps2 = data2[data2['MP Gender'] == 'Female'].shape[0]

    # Display results
    print("\nComparing " + country1 + " and " + country2 + ":")
    print("Electorate: " + str(electorate1) + " vs " + str(electorate2))
    print("Valid Votes: " + "{:.2f}".format(ValpercV1) + "% vs " + "{:.2f}".format(ValpercV2) + "%")
    print("Invalid Votes: " + "{:.2f}".format(InvpercV1) + "% vs " + "{:.2f}".format(InvpercV2) + "%")
    print("Voter Turnout: " + "{:.2f}".format(turnout1) + "% vs " + "{:.2f}".format(turnout2) + "%")
    print("Male MPs: " + str(maleMps1) + " vs " + str(maleMps2))
    print("Female MPs: " + str(femaleMps1) + " vs " + str(femaleMps2))

# Functions to compare two Constituencies
def compareConstituencies(df):

    # User enters two constituencies
    constituency1 = input("Enter First constituency: ")
    constituency2 = input("Enter Second constituency: ")

    # Filters dataset for both constituencies
    data1 = df[df['Constituency'] == constituency1]
    data2 = df[df['Constituency'] == constituency2]

    # Calculate data

    # Sum of electorate for both constituencies
    electorate1 = data1['Electorate'].sum()
    electorate2 = data2['Electorate'].sum()

    # Sum of valid votes for both constituencies
    validVotes1 = data1['Valid Votes'].sum()
    validVotes2 = data2['Valid Votes'].sum()

    # Sum of invalid votes for both constituencies
    invalidVotes1 = data1['Invalid Votes'].sum()
    invalidVotes2 = data2['Invalid Votes'].sum()

    # Winning party and votes achieved for first constituency
    firstParty1 = data1['First Party'].values[0]
    firstPartyVotes1 = data1[firstParty1].values[0]

    # Winning party and votes achieved for second constituency
    firstParty2 = data2['First Party'].values[0]
    firstPartyVotes2 = data2[firstParty2].values[0]

    turnout1 = ((validVotes1 + invalidVotes1) / electorate1) * 100
    turnout2 = ((validVotes2 + invalidVotes2) / electorate2) * 100

    # Display results
    print("\nComparison between " + constituency1 + " and " + constituency2 + ":")
    print("Electorate: " + str(electorate1) + " vs " + str(electorate2))
    print("Voter Turnout: " + "{:.2f}".format(turnout1) + "% vs " + "{:.2f}".format(turnout2) + "%")
    print("First Party: " + firstParty1 + " (" + str(firstPartyVotes1) + " votes) vs " + firstParty2 + " (" + str(firstPartyVotes2) + " votes)")

# Functions to compare two MPs + writes to file which can be read from
def compareMps(df):

    # User enters last name for both MPs
    mp1 = input("Enter last name for first MP: ")
    mp2 = input("Enter last name for second MP: ")

    # Filters dataset for both MP last names
    data1 = df[df['MP Last Name'].str.contains(mp1)]
    data2 = df[df['MP Last Name'].str.contains(mp2)]

    # Fetching MPs data from data file
    firstName1 = data1['MP First Name'].values[0]
    fullName1 = firstName1 + " " + mp1
    party1 = data1['First Party'].values[0]
    votes1 = data1[party1].values[0]
    constituency1 = data1['Constituency'].values[0]
    electorate1 = data1['Electorate'].sum()
    vPercent1 = (votes1 / electorate1) * 100

    firstName2 = data2['MP First Name'].values[0]
    fullName2 = firstName2 + " " + mp2
    party2 = data2['First Party'].values[0]
    votes2 = data2[party2].values[0]
    constituency2 = data2['Constituency'].values[0]
    electorate2 = data2['Electorate'].sum()
    vPercent2 = (votes2 / electorate2) * 100

    # Write results directly to file
    txtFile = open("MPResults.txt", 'w')
    txtFile.write("Comparison between MPs " + fullName1 + " and " + fullName2 + ":\n")
    txtFile.write("Party: " + party1 + " vs " + party2 + "\n")
    txtFile.write("Total Votes: " + str(votes1) + " vs " + str(votes2) + "\n")
    txtFile.write("Constituency: " + constituency1 + " vs " + constituency2 + "\n")
    txtFile.write("Electorate: " + str(electorate1) + " vs " + str(electorate2) + "\n")
    txtFile.write("Vote Percentage in Constituency: " + "{:.2f}".format(vPercent1) + "% vs " + "{:.2f}".format(vPercent2) + "%\n")
    txtFile.close()

    #Print appropriate message and opens file
    print("Results written to File!")
    txtFile = open("MPResults.txt", 'r')
    print(txtFile.read())
    txtFile.close()

#Functions for Option #3 - View Graph Data

# Function plots bar chart of no.of male vs female MPs
def plotOpt1(df):

    # Count number of male and female MPs
    maleCount = df[df['MP Gender'] == 'Male'].shape[0]
    femaleCount = df[df['MP Gender'] == 'Female'].shape[0]

    # Creates list of counts and labels for bar chart
    value = [maleCount, femaleCount]
    label = ['Male', 'Female']

    # Generate bar chart for gender distribution
    plt.bar(label, value)
    plt.xlabel('Gender')  # x-axis label
    plt.ylabel('Number of MPs')  # y-axis label
    plt.title('Gender Distribution of MPs')  # Title for chart
    plt.show()

# Function plots number of wins per party
def plotOpt2(df):

    # Calculate number of wins for each party based on 'First Party' column
    partyWins = df['First Party'].value_counts()

    # Generate a bar chart for party wins
    plt.bar(partyWins.index, partyWins.values)
    plt.xlabel('Party')  # x-axis label
    plt.ylabel('Number of Wins')  # y-axis label
    plt.title('Number of Wins per Party')  # Title for chart
    plt.show()

# Function plots electorate size for each constituency
def plotOpt3(df):

    # Generates bar chart with electorate on y-axis and constituency on x-axis
    plt.bar(df['Constituency'], df['Electorate'])
    plt.xlabel('Constituency')  # x-axis label
    plt.ylabel('Electorate Size')  # y-axis label
    plt.title('Electorate Size per Constituency')  # Title for chart
    plt.show()

#Main Code
def main():
    VAData = "data.csv"
    df = load_data(VAData)
    while True:
        prime_menu()
        choice = input("Enter your choice: ")
        if choice == "1":
            while True:
                viewData_menu()
                sub_choice = input("Enter your choice: ")
                if sub_choice == "1":
                    viewConstituencyData(df)
                elif sub_choice == "2":
                    viewPartyData(df)
                elif sub_choice == "3":
                    viewMpData(df)
                elif sub_choice == "4":
                    print("Returning to main menu")
                    break
                else:
                    print("Invalid option - Please try again.")

        elif choice == "2":
            while True:
                compData_menu()
                sub_choice = input("Enter your choice: ")
                if sub_choice == "1":
                    compareCountries(df)
                elif sub_choice == "2":
                    compareConstituencies(df)
                elif sub_choice == "3":
                    compareMps(df)
                elif sub_choice == "4":
                    print("Returning to main menu")
                    break
                else:
                    print("Invalid option - Please try again.")

        elif choice == "3":
            while True:
                graphData_menu()
                sub_choice = input("Enter your choice: ")
                if sub_choice == "1":
                    plotOpt1(df)
                elif sub_choice == "2":
                    plotOpt2(df)
                elif sub_choice == "3":
                    plotOpt3(df)
                elif sub_choice == "4":
                    print("Returning to main menu")
                    break
                else:
                    print("Invalid option - Please try again.")
        elif choice == "4":
            print("Thank you for using the Voting Analysis Engine - GoodBye")
            sys.exit()
        else:
            print("Invalid option - Please try again.")


main()

