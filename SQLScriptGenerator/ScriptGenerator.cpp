#include<iostream>
#include<fstream>
#include<ctime>
using namespace std;
ofstream g("script.txt");
int cmd,numberOfEntries;
struct tableEntries { char txt[100]; int table, column; }entries[1000];
struct column { char name[100], type[100]; bool null, identity, primary; };
struct table { char name[100]; column columns[100]; int numberOfColumns; };
struct database { char name[100]; table tables[100]; int numberOfTables; };
database dbo;

bool tableExists(char n[])
{
	for (int i = 1; i <= dbo.numberOfTables; i++)
		if (!strcmp(dbo.tables[i].name, n))
			return true;
	return false;
}

bool columnExists(char n[], int index)
{
	for (int i = 1; i <= dbo.tables[index].numberOfColumns; i++)
		if (!strcmp(dbo.tables[index].columns[i].name, n))
			return true;
	return false;
}

void printTables()
{
	for (int i = 1; i <= dbo.numberOfTables; i++)
		cout << i << ". " << dbo.tables[i].name << '\n';
}

void printColumns(int tableIndex)
{
	for (int i = 1; i <= dbo.tables[tableIndex].numberOfColumns; i++)
		cout << i << ". " << dbo.tables[tableIndex].columns[i].name << '\n';
}

void deleteTable(int index)
{
	for (int i = index; i <= dbo.numberOfTables - 1; i++)
		dbo.tables[i] = dbo.tables[i + 1];
	dbo.numberOfTables--;
}

void deleteColumn(int tableIndex, int columnIndex)
{
	for (int i = columnIndex; i <= dbo.tables[tableIndex].numberOfColumns - 1; i++)
		dbo.tables[tableIndex].columns[i] = dbo.tables[tableIndex].columns[i + 1];
}

void generate()
{
	
	g << "CREATE DATABASE " << dbo.name << "; \nGO\n USE " << dbo.name << " \nGO \n ";
	for (int i = 1; i <= dbo.numberOfTables; i++)
	{
		if (dbo.tables[i].numberOfColumns)
		{
			g << "CREATE TABLE dbo." << dbo.tables[i].name << "(\n";
			g << dbo.tables[i].columns[1].name << ' ' << dbo.tables[i].columns[1].type << ' ';
			if (dbo.tables[i].columns[1].primary) g << "PRIMARY KEY ";
			if (dbo.tables[i].columns[1].null) g << "NULL ";
			else
				g << "NOT NULL ";
			if (dbo.tables[i].columns[1].identity)
				g << "IDENTITY(1,1) ";
			for (int j = 2; j <= dbo.tables[i].numberOfColumns; j++)
			{
				g << ",\n";
				g << dbo.tables[i].columns[j].name << ' ' << dbo.tables[i].columns[j].type << ' ';
				if (dbo.tables[i].columns[j].primary)
					g << "PRIMARY KEY ";
				if (dbo.tables[i].columns[j].null)
					g << "NULL ";
				else
					g << "NOT NULL ";
				if (dbo.tables[i].columns[j].identity)
					g << "IDENTITY(1,1) ";
			}
			g << ")\nGO \n";
			for (int j = 1; j <= numberOfEntries; j++)
			{
				if (entries[j].table == i)
				{
					int k = 1, jj;
					jj = j;
					while (dbo.tables[i].columns[k].identity)
						k++;
					g << "INSERT INTO dbo." << dbo.tables[i].name << " (" << dbo.tables[i].columns[k++].name;
					for (; k <= dbo.tables[i].numberOfColumns; k++, jj++)
						g << "," << dbo.tables[i].columns[k].name;
					g << ")\n";



					k = 1;
					while (dbo.tables[i].columns[k].identity)
						k++, j++;
					g << "VALUES ('" << entries[j++].txt << "'";
					k++;
					for (; k <= dbo.tables[i].numberOfColumns; k++, j++)
						g << ",'" << entries[j].txt << "'";
					g << ");\n";
				}
			}
		}
	}
}

int main()
{
	char filepath[1000];
	time_t now = time(NULL);
	strftime(filepath, 20, "%d-%m-%Y_%H-%M-%S", localtime(&now));
	strcat(filepath, ".txt");
	ofstream command_log(filepath);
	while (1)
	{
		cout << "Chose your command: \n";
		cout << "1.Create database \n";
		cout << "2.Add table \n";
		cout << "3.Add column \n";
		cout << "4.Delete table\n";
		cout << "5.Delete column \n";
		cout << "6.Show tables\n";
		cout << "7.Add entry to table  \n";
		cout << "0. GENERATE SCRIPTS AND EXIT\n";
		cin >> cmd;
		command_log << cmd << '\n';
		cout << "\n\n\n";
		if (cmd == 1)
		{
			if (strcmp(dbo.name, "") == 0)
			{
				cout << "name your database: ";
				cin >> dbo.name;
				command_log << dbo.name<<'\n';
			}
			else
				cout << "you already have a database. exit this run to generate the scripts and run again to start a new database \n";
		}
		if (cmd == 2)
		{
			if (strcmp(dbo.name, "") != 0)
			{
				cout << "For each column the command is 'add'\n when you are done, type 'exit' \n Command:";
				char namee[100];
				cin >> namee;
				command_log << namee << '\n';
				while (strcmp(namee, "exit"))
				{
					cout << "Table name:";
					cin >> namee;
					command_log << namee << '\n';
					if (!tableExists(namee))
					{
						dbo.numberOfTables++;
						strcpy(dbo.tables[dbo.numberOfTables].name, namee);
					}
					else
						cout << "table already exists \n";
					cout << "Command (add/exit):";
					cin >> namee;
					command_log << namee << '\n';
				}
			}
			else
				cout << "please create a database\n";
		}
		if (cmd == 3)
		{
			cout << "Select a table. Write only the index: \n";
			printTables();
			int tableIndex;
			char namee[100];
			cin >> tableIndex;
			command_log << tableIndex << '\n';
			cout << "For each column the command is 'add'\n when you are done, type 'exit' \n Command:";
			cin >> namee;
			command_log << namee << '\n';
			while (strcmp(namee, "exit"))
			{
				column col;
				char ans;
				cout << "Coulmn name:";
				cin >> namee;
				command_log << namee << '\n';
				if (!columnExists(namee, tableIndex))
					strcpy(col.name, namee);
				else
					cout << "column name used";
				cout << "Column type (only sql accepted data types, please double check):";
				cin >> col.type;
				command_log << col.type << '\n';
				cout << "is this column nullable? ('y'/'n'):";
				cin >> ans;
				command_log << ans << '\n';
				if (ans == 'y')
					col.null = true;
				else
					col.null = false;


				cout << "do you want this column to autoincrement? ('y'/'n'):";
				cin >> ans;
				command_log << ans << '\n';
				if (ans == 'y')
					col.identity = true;
				else
					col.identity = false;

				cout << "is this column primary key? ('y'/'n'):";
				cin >> ans;
				command_log << ans << '\n';
				if (ans == 'y')
					col.primary = true;
				else
					col.primary = false;

				dbo.tables[tableIndex].numberOfColumns++;
				dbo.tables[tableIndex].columns[dbo.tables[tableIndex].numberOfColumns] = col;
				cout << "Command (add/exit):";
				cin >> namee;
				command_log << namee << '\n';
			}
		}
		if (cmd == 4)
		{
			cout << "chose a table to delete, write only the index. to cancel type '0':";
			printTables();
			int index;
			cin >> index;
			command_log << index << '\n';
			if (index)
				deleteTable(index);
		}
		if (cmd == 5)
		{
			int tableIndex, columnIndex;
			cout << "Select a table, type only the index. To cancel type '0':";
			printTables();
			cin >> tableIndex;
			command_log << tableIndex << '\n';
			if (tableIndex)
			{
				cout << "Select a column, type only the index. To cancel type '0':";
				printColumns(tableIndex);
				cin >> columnIndex;
				command_log << columnIndex << '\n';
				if (columnIndex)
					deleteColumn(tableIndex, columnIndex);
			}
		}
		if (cmd == 6)
			printTables();
		if (cmd == 7)
		{
			int tableIndex, columnIndex;
			cout << "Select a table, type only the index. To cancel type '0':";
			printTables();
			cin >> tableIndex;
			command_log << tableIndex << '\n';
			if (tableIndex)
			{
				for (int i = 1; i <= dbo.tables[tableIndex].numberOfColumns; i++)
				{
					cout << dbo.tables[tableIndex].columns[i].name << ": ";
					cin>>entries[++numberOfEntries].txt;
					command_log << entries[numberOfEntries].txt << '\n';
					entries[numberOfEntries].table = tableIndex;
					entries[numberOfEntries].column = i;
					cout << '\n';
				}
			}
		}
		if (cmd == 0)
		{
			generate();
			return 0;
		}
		cout << "\n\n\n";
	}
	return 0;
}