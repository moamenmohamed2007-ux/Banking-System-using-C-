// Libraries ##
#include <iostream>
#include <fstream>
#include <windows.h>
#include <cstring>
#include <ctime>
#include <cstdlib>
#include <iomanip>

using namespace std;

// Constants ##
const int max_tellers = 10;
const int week_days = 7;
const int year_days = 365;

const char *days[] = {"Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"};

// Inputs ##
int teller_no[week_days];

char opening_time[6] = "";
char closing_time[6] = "";

int opening_hour, opening_minute;
int closing_hour, closing_minute;

int total_opening;
int total_closing;
int days_count = 0;
int total_customers = 0;
double sum_waiting = 0;

int trans_no = 0;

struct Transaction
{
    char name[20];
    int duration;
};

Transaction *transaction = new Transaction[20];

int yearly_target;
int daily_avg;

int sim_day;
int work_hours;
double sum_work_hours = 0;
double sum_available_time = 0;

int *customers_per_hour = new int[24];
int P_hour;

int peak_day;
char peak_hour1[6];
char peak_hour2[6];
char *weekly_peakhour[7];
char *yearlypeak_hour[365];
char peak_overall1[6];
char peak_overall2[6];
int peak_customer_no;
int peak_customer_overall = 0;

int waiting_time;
int waiting_sum;
double waiting_avg;

int weeklyserved;
int weeklynot;
double weeklyaveragewait;
double weeklywait;
double utilization;

struct Customer
{
    char id[4];
    char arr_time[6];
    char svc_start[6];
    char svc_end[6];
    int waiting_time;
    char transaction[20];
    bool served;
};

int customer_no;
int served_count;

Customer *customer = NULL;

struct Tellers
{
    char id[5];
    char svc_end[6];
    int idle_time;
    int customers_served;
};

Tellers *tellers = new Tellers[10];

bool Exit = false;

// Helping Functions ##
void inSensitive(char *str)
{
    if (str[0] >= 'a' && str[0] <= 'z')
    {
        str[0] = str[0] - 'a' + 'A';
    }

    for (int i = 1; i < strlen(str); i++)
    {
        if (str[i] >= 'A' && str[i] <= 'Z')
        {
            str[i] = str[i] - 'A' + 'a';
        }
    }
}

void sortstruct(Customer *arr, int n)
{
    for (int i = 0; i < n - 1; i++)
    {
        for (int j = 0; j < n - 1 - i; j++)
        {
            if (strcmp(arr[j].arr_time,
                       arr[j + 1].arr_time) > 0)
            {
                Customer temp = arr[j];
                arr[j] = arr[j + 1];
                arr[j + 1] = temp;
            }
        }
    }

    for (int i = 0; i < n; i++)
    {
        arr[i].id[0] = (i + 1) / 100 + '0';
        arr[i].id[1] = ((i + 1) / 10) % 10 + '0';
        arr[i].id[2] = (i + 1) % 10 + '0';
        arr[i].id[3] = '\0';
    }
}

void add_duration(char *time, int duration)
{
    int HH =
        (time[0] - '0') * 10 +
        (time[1] - '0');

    int MM =
        (time[3] - '0') * 10 +
        (time[4] - '0');

    int total = HH * 60 + MM + duration;

    HH = (total / 60) % 24;
    MM = total % 60;

    time[0] = HH / 10 + '0';
    time[1] = HH % 10 + '0';

    time[3] = MM / 10 + '0';
    time[4] = MM % 10 + '0';
}

int time_diff(char *t1, char *t2)
{
    int H1 =
        (t1[0] - '0') * 10 +
        (t1[1] - '0');

    int M1 =
        (t1[3] - '0') * 10 +
        (t1[4] - '0');

    int H2 =
        (t2[0] - '0') * 10 +
        (t2[1] - '0');

    int M2 =
        (t2[3] - '0') * 10 +
        (t2[4] - '0');

    int H_diff = H1 - H2;
    int M_diff = M1 - M2;

    return H_diff * 60 + M_diff;
}

void modePeakHour(char **arr, int n)
{
    int count, temp = 0;
    char result[6];

    for (int i = 0; i < n; i++)
    {
        count = 0;
        for (int j = 0; j < n; j++)
            if (strcmp(arr[i], arr[j]) == 0)
                count++;
        if (temp < count)
        {
            temp = count;
            strcpy(result, arr[i]);
        }
    }

    int start_hour = (result[0] - '0') * 10 + (result[1] - '0');
    int end_hour = start_hour + 1;
    cout << "Peak Hour: ";
    if (start_hour < 10)
        cout << "0";
    cout << start_hour << ":00 - ";
    if (end_hour < 10)
        cout << "0";
    cout << end_hour << ":00 (most frequent) \n";
}

// Main Functions ##

void Main_Menu();
void Sys_Config();
void Set_Tellers();
void Working_Hours();
void Transactions();
void Yearly_Target();
void Current_Config();

void Run_Simulation();
void Daily_Sim();
void Weekly_Sim();
void Yearly_Sim();

void View_Reports();
void Customer_Reports();
void Teller_Reports();
void Export_Reports();

void View_Statistics();

int main()
{
    srand(time(0));
    cout << "\n=========================================\n||    BANK COSTUMERS QUEUING SYSTEM    ||\n=========================================\n";

    Main_Menu();
    return 0;
}

void Main_Menu()
{
    int choice;
    while (true)
    {
        cout << "\n=== MAIN MENU === \n\n1. System Configuration \n2. Run Simulation \n3. View Reports \n4. View Statistics \n5. Exit\n";
        if (!(cin >> choice))
        {
            system("cls");

            cout << "Invalid Choice. Please enter a valid number.\n";
            cin.clear();
            cin.ignore(10000, '\n');
            continue;
        }
        system("cls");

        switch (choice)
        {
        case 1:
            Sys_Config();
            break;

        case 2:
            Run_Simulation();
            break;

        case 3:
            View_Reports();
            break;

        case 4:
            View_Statistics();
            break;

        case 5:
            Exit = true;
            break;

        default:
            system("cls");
            cout << "Invalid Choice. Please enter a valid number.\n";
        }

        if (Exit)
        {
            break;
        }
    }
}

void Sys_Config()
{
    int choice;

    while (true)
    {
        cout << "\n=== SYSTEM CONFIGURATION === \n\n1. Set Number of tellers \n2. Set Working Hours \n3. Define Transaction Types \n4. Set Yearly Customer Target \n5. View Current Configuration \n6. Back to Main Menu \n";
        if (!(cin >> choice))
        {
            system("cls");

            cout << "Invalid Choice. Please enter a valid number.\n";
            cin.clear();
            cin.ignore(10000, '\n');
            continue;
        }
        cin.ignore();
        system("cls");

        switch (choice)
        {
        case 1:
            Set_Tellers();
            break;

        case 2:
            Working_Hours();
            break;

        case 3:
            Transactions();
            break;

        case 4:
            Yearly_Target();
            break;

        case 5:
            Current_Config();
            break;

        case 6:
            return;

        default:
            system("cls");
            cout << "Invalid Choice. Please enter a valid number.\n";
        }
    }
}

void Set_Tellers()
{
    cout << " --- SET NUMBER OF TELLERS ---\n ";

    for (int i = 0; i < week_days; i++)
    {
        while (true)
        {
            cout << "\n Enter number of tellers for " << *(days + i) << ": ";

            if (!(cin >> teller_no[i]))
            {
                cout << " Invalid input. Please try again.\n";
                cin.clear();
                cin.ignore(10000, '\n');
                continue;
            }

            if (teller_no[i] <= 0 || teller_no[i] > max_tellers)
            {
                cout << " Invalid input, Please Enter a value between 1 and 10 (max tellers). \n";
                continue;
            }
            break;
        }
    }
    cout << "\nNumber of tellers is set successfully!\n";
    Sleep(1000);
}

void Working_Hours()
{
    cout << " --- SET WORKING HOURS ---\n ";

    do
    {
        cout << "\nEnter the opening hours in HH:MM, in 24-hour format: ";
        cin >> opening_time;

        if (strlen(opening_time) != 5 || opening_time[2] != ':')
        {
            cout << "Invalid opening time. Please enter time in HH:MM format.\n";
            continue;
        }

        opening_hour = (opening_time[0] - '0') * 10 + (opening_time[1] - '0');
        opening_minute = (opening_time[3] - '0') * 10 + (opening_time[4] - '0');

        if (opening_hour < 0 || opening_hour > 23 || opening_minute < 0 || opening_minute > 59)
        {
            cout << "Invalid opening time! Enter a valid time, Hours (00-23) & minutes (00-59).\n";
            continue;
        }

        break;
    } while (true);
    cout << "\nOpening time is set successfully!\n";

    do
    {
        cout << "\nEnter the closing hours in HH:MM, in 24-hour format: ";
        cin >> closing_time;

        if (strlen(closing_time) != 5 || closing_time[2] != ':')
        {
            cout << "Invalid closing time. Please enter time in HH:MM format.\n";
            continue;
        }

        closing_hour = (closing_time[0] - '0') * 10 + (closing_time[1] - '0');
        closing_minute = (closing_time[3] - '0') * 10 + (closing_time[4] - '0');

        if (closing_hour < 0 || closing_hour > 23 || closing_minute < 0 || closing_minute > 59)
        {
            cout << "Invalid closing time! Enter a valid time, Hours: 00-23 & minutes: 00-59.\n";
            continue;
        }

        total_opening = opening_hour * 60 + opening_minute;
        total_closing = closing_hour * 60 + closing_minute;

        if (total_closing <= total_opening)
        {
            cout << "Closing time must be after opening time.\n";
            continue;
        }

        break;
    } while (true);

    cout << "\nClosing time is set successfully!\n";
    Sleep(1000);
}

void Transactions()
{
    char name[20];
    bool repeated, done = false;

    cout << " --- DEFINE TRANSACTION TYPES ---\n ";

    for (int i = 0; !done; i++)
    {
        do
        {
            repeated = false;

            cout << "\nEnter transaction name (or 'done' to finish): ";
            cin.getline(name, 20);

            int len = strlen(name);
            while (len > 0 && (name[len - 1] == ' ' || name[len - 1] == '\t'))
            {
                name[len - 1] = '\0';
                len--;
            }

            int start = 0;
            while (name[start] == ' ' || name[start] == '\t')
            {
                start++;
            }

            if (start > 0)
            {
                int k = 0;
                while (name[start + k] != '\0')
                {
                    name[k] = name[start + k];
                    k++;
                }
                name[k] = '\0';
            }

            inSensitive(name);

            if (strcmp(name, "\0") == 0)
            {
                cout << "Transaction name should not be empty.\n";
                continue;
            }

            for (int j = 0; j < i; j++)
            {
                if (strcmp(name, transaction[j].name) == 0)
                {
                    cout << "Transaction name must be unique.\n";
                    repeated = true;
                    break;
                }
            }

            if (repeated)
            {
                continue;
            }

            if (strcmp(name, "Done") == 0)
            {
                done = true;
                break;
            }

            strcpy(transaction[i].name, name);
            break;
        } while (true);

        if (done)
        {
            break;
        }

        do
        {
            cout << "Enter average duration in minutes: ";
            if (!(cin >> transaction[i].duration))
            {
                cout << "Invalid input, enter a new duration: \n\n";
                cin.clear();
                cin.ignore(10000, '\n');
                continue;
            }

            cin.ignore(10000, '\n');

            if (transaction[i].duration <= 0)
            {
                cout << "Duration must be positive.\n";
                continue;
            }

            cout << "Transaction added successfully! \n";
            trans_no++;
            break;

        } while (true);
    }
    Sleep(1000);
}

void Yearly_Target()
{
    cout << " --- SET YEARLY CUSTOMER TARGET ---\n ";

    do
    {
        cout << "\nEnter target number of customers per year: ";
        if (!(cin >> yearly_target))
        {
            cout << "Invalid input. Try again. \n";
            cin.clear();
            cin.ignore(10000, '\n');
            continue;
        }
        cin.ignore();

        if (yearly_target < 30000)
        {
            cout << "The value must be greater than 30,000 \n";
            continue;
        }
        break;
    } while (true);

    daily_avg = yearly_target / year_days;
    cout << "Target is set successfully! (Average ~" << daily_avg << " customers/day) \n";
    Sleep(1000);
}

void Current_Config()
{
    cout << " --- CURRENT CONFIGURATION ---\n ";

    cout << "\nNumber of tellers: \n";
    for (int i = 0; i < 7; i++)
    {
        printf("%.3s: ", days[i]);
        if (teller_no[i] == 0)
        {
            cout << "not set yet.\n";
        }
        else
            cout << teller_no[i] << " tellers\n";
    }

    if (strcmp(opening_time, "\0") == 0)
    {
        cout << "\nOpening time : not set yet.";
    }
    else
        cout << "\nOpening time : " << opening_time;

    if (strcmp(closing_time, "\0") == 0)
    {
        cout << "\nClosing time : not set yet.\n";
    }
    else
        cout << "\nClosing time : " << closing_time << "\n";

    if (strcmp(transaction[0].name, "\0") == 0)
    {
        cout << "\nTran1sactions: not set yet.\n";
    }
    else
    {
        cout << "\nTransactions: \n";
        for (int i = 0; i < trans_no; i++)
        {
            cout << transaction[i].name << "\n";
        }
    }

    if (yearly_target == 0)
    {
        cout << "\nYearly Customer Target: not set yet.\n";
    }
    else
        cout << "\nYearly Customer Target: " << yearly_target << "\n";

    Sleep(1000);
}

void Run_Simulation()
{
    if (teller_no[0] == 0)
    {
        cout << "\nConfiguration missing. The number of tellers is not set yet.\n";
        return;
    }

    if (strcmp(opening_time, "\0") == 0)
    {
        cout << "\nConfiguration missing. Opening and Closing times are not set yet.\n";
        return;
    }

    if (strcmp(transaction[0].name, "\0") == 0)
    {
        cout << "\nConfiguration missing. Transactions are not set yet.\n";
        return;
    }

    if (yearly_target == 0)
    {
        cout << "\nConfiguration missing. The Yearly Customer Target is not set yet.\n";
        return;
    }

    int choice;

    while (true)
    {
        cout << "\n=== Run Simulation === \n\n1. Run Daily Simulation \n2. Run Weekly Simulation \n3. Run Full Year Simulation \n4. Back to Main Menu \n";
        if (!(cin >> choice))
        {
            system("cls");

            cout << "Invalid Choice. Please enter a valid number.\n";
            cin.clear();
            cin.ignore(10000, '\n');
            continue;
        }
        cin.ignore();
        system("cls");

        switch (choice)
        {
        case 1:
            cout << " --- DAILY SIMULATION ---\n";

            do
            {
                for (int i = 0; i < week_days; i++)
                {
                    cout << i + 1 << ". " << days[i] << "\n";
                }
                cout << "Choose simulation day ";

                if (!(cin >> sim_day))
                {
                    system("cls");

                    cout << "Invalid Choice. Please enter a valid number.\n";
                    cin.clear();
                    cin.ignore(10000, '\n');
                    continue;
                }

                if (sim_day < 1 || sim_day > 7)
                {
                    cout << " Invalid input, Please enter a valid number. \n";
                    continue;
                }
                break;
            } while (true);

            Daily_Sim();

            system("cls");
            cout << "\nSimulating " << days[sim_day - 1] << "... \n";
            cout << "Customers generated: " << customer_no << "\n";
            cout << "Processing customers...\n[===============================] 100%\n\n";

            cout << " === Daily Summary === \n";
            cout << "Customers Served: " << served_count << "\n";
            cout << "Customers Not Served: " << customer_no - served_count << "\n";
            cout << "Average Waiting Time: " << waiting_avg << "\n";
            cout << "Peak Hour: " << peak_hour1 << " - " << peak_hour2 << " (" << peak_customer_no << " customers)\n";
            days_count++;
            total_customers += served_count;
            sum_waiting += waiting_sum;

            if (peak_customer_no > peak_customer_overall) // for statistics
            {
                peak_customer_overall = peak_customer_no;

                strcpy(peak_overall1, peak_hour1);
                strcpy(peak_overall2, peak_hour2);
                peak_day = sim_day;
            }

            for (int i = 0; i < teller_no[sim_day - 1]; i++)
            {
                sum_work_hours += (total_closing - total_opening) - tellers[i].idle_time;
                sum_available_time += (total_closing - total_opening);
            }
            break;

        case 2:
            Weekly_Sim();
            break;

        case 3:
            Yearly_Sim();
            break;

        case 4:
            return;

        default:
            system("cls");
            cout << "Invalid Choice. Please enter a valid number.\n";
        }
    }
}

void Daily_Sim()
{
    char time[6];

    waiting_sum = 0;
    served_count = 0;
    work_hours = time_diff(closing_time, opening_time) / 60 + 1;

    // Generate customer count
    if (rand() % 2 == 0)
    {
        customer_no = daily_avg + rand() % 11;
    }
    else
    {
        customer_no = daily_avg - rand() % 11;
    }

    if (customer_no < 1)
    {
        customer_no = 1;
    }

    // Allocate customers safely
    if (customer != NULL)
    {
        delete[] customer;
    }

    customer = new Customer[customer_no];

    // Generate customers
    for (int i = 0; i < customer_no; i++)
    {
        int rand_total = total_opening + rand() % (total_closing - total_opening);
        int randH = rand_total / 60;
        int randM = rand_total % 60;

        time[0] = randH / 10 + '0';
        time[1] = randH % 10 + '0';
        time[2] = ':';
        time[3] = randM / 10 + '0';
        time[4] = randM % 10 + '0';
        time[5] = '\0';

        strcpy(customer[i].arr_time, time);
        strcpy(customer[i].svc_start, time);
        strcpy(customer[i].svc_end, time);

        customer[i].served = false;

        int rand_transaction = rand() % trans_no;

        strcpy(customer[i].transaction, transaction[rand_transaction].name);

        int variation = transaction[rand_transaction].duration / 3;

        if (variation < 1)
        {
            variation = 1;
        }

        int service_duration;

        if (rand() % 2 == 0)
        {
            service_duration = transaction[rand_transaction].duration + rand() % variation;
        }
        else
        {
            service_duration = transaction[rand_transaction].duration - rand() % variation;
        }

        if (service_duration < 1)
        {
            service_duration = 1;
        }

        add_duration(customer[i].svc_end, service_duration);
    }

    // Sort customers
    sortstruct(customer, customer_no);

    // Customers per hour
    for (int i = 0; i < work_hours; i++)
    {
        int count = 0;

        for (int j = 0; j < customer_no; j++)
        {
            int arr_hour = (customer[j].arr_time[0] - '0') * 10 + (customer[j].arr_time[1] - '0');

            if (arr_hour == opening_hour + i)
            {
                count++;
            }
        }

        customers_per_hour[i] = count;
    }

    // Peak hour
    peak_customer_no = customers_per_hour[0];
    P_hour = opening_hour;

    for (int i = 0; i < work_hours; i++)
    {
        if (customers_per_hour[i] > peak_customer_no)
        {
            peak_customer_no = customers_per_hour[i];
            P_hour = opening_hour + i;
        }
    }

    peak_hour1[0] = P_hour / 10 + '0';
    peak_hour1[1] = P_hour % 10 + '0';
    peak_hour1[2] = ':';
    peak_hour1[3] = '0';
    peak_hour1[4] = '0';
    peak_hour1[5] = '\0';

    peak_hour2[0] = (P_hour + 1) / 10 + '0';
    peak_hour2[1] = (P_hour + 1) % 10 + '0';
    peak_hour2[2] = ':';
    peak_hour2[3] = '0';
    peak_hour2[4] = '0';
    peak_hour2[5] = '\0';

    // Initialize tellers
    for (int i = 0; i < teller_no[sim_day - 1]; i++)
    {
        tellers[i].id[0] = 'T';
        tellers[i].id[1] = '0';
        tellers[i].id[2] = (i + 1) / 10 + '0';
        tellers[i].id[3] = (i + 1) % 10 + '0';
        tellers[i].id[4] = '\0';

        strcpy(tellers[i].svc_end, opening_time);
        tellers[i].idle_time = 0;
        tellers[i].customers_served = 0;
    }

    // Serve customers
    for (int i = 0; i < customer_no; i++)
    {
        int best_teller = 0;

        for (int j = 1; j < teller_no[sim_day - 1]; j++)
        {
            if (strcmp(tellers[j].svc_end, tellers[best_teller].svc_end) < 0)
            {
                best_teller = j;
            }
        }

        int service_duration = time_diff(customer[i].svc_end, customer[i].svc_start);

        // Teller busy
        if (strcmp(tellers[best_teller].svc_end, customer[i].arr_time) > 0)
        {
            waiting_time = time_diff(tellers[best_teller].svc_end, customer[i].arr_time);
            waiting_sum += waiting_time;

            strcpy(customer[i].svc_start, tellers[best_teller].svc_end);

            // Customer leaves
            if (waiting_time >= 10 && (rand() % 4 == 0 || rand()%4 == 2))
            {
                continue;
            }
        }
        else
        {
            waiting_time = 0;
            int idle = time_diff(customer[i].arr_time, tellers[best_teller].svc_end);

            tellers[best_teller].idle_time += idle;
            strcpy(customer[i].svc_start, customer[i].arr_time);
        }

        strcpy(customer[i].svc_end, customer[i].svc_start);

        add_duration(customer[i].svc_end, service_duration);

        strcpy(tellers[best_teller].svc_end, customer[i].svc_end);

        tellers[best_teller].customers_served++;

        customer[i].served = true;

        served_count++;
    }

    for (int i = 0; i < teller_no[sim_day - 1]; i++)
    {
        if (strcmp(tellers[i].svc_end, closing_time) < 0)
        {
            tellers[i].idle_time += time_diff(closing_time, tellers[i].svc_end);
        }
    }

    waiting_avg = (double)waiting_sum / customer_no;
}

void Weekly_Sim()
{
    int peakcustom_no = 0;
    weeklyserved = 0;
    weeklynot = 0;
    weeklyaveragewait = 0;

    for (int i = 0; i < week_days; i++)
        weekly_peakhour[i] = new char[6];

    for (int i = 0; i < week_days; i++)
    {
        sim_day = i + 1;
        Daily_Sim();
        strcpy(weekly_peakhour[i], peak_hour1);

        weeklyserved += served_count;
        weeklynot += (customer_no - served_count);
        weeklywait += waiting_avg;
    }

    weeklyaveragewait = weeklywait / week_days;

    cout << " === Weekly Summary === \n";
    cout << "Customers Served: " << weeklyserved << "\n";
    cout << "Customers Not Served: " << weeklynot << "\n";
    cout << "Average Waiting Time: " << weeklyaveragewait << "\n";
    modePeakHour(weekly_peakhour, week_days);
}

void Yearly_Sim()
{
    int customer_year = 0;
    int served_year = 0;
    double waiting_time_year = 0;

    sim_day = 1;

    for (int i = 0; i < year_days; i++)
        yearlypeak_hour[i] = new char[6];

    for (int i = 0; i < year_days; i++)
    {
        if (sim_day > 7)
        {
            sim_day = 1;
        }

        Daily_Sim();

        strcpy(yearlypeak_hour[i], peak_hour1);
        customer_year += customer_no;
        served_year += served_count;
        waiting_time_year += waiting_avg;

        sim_day++;
    }

    cout << "\n=== YEARLY SUMMARY ===\n";
    cout << "Customers Generated: " << customer_year << "\n";
    cout << "Customers Served: " << served_year << "\n";
    cout << "Customers Not Served: " << customer_year - served_year << "\n";
    cout << "Average Waiting Time: " << waiting_time_year / year_days << " minutes\n";

    modePeakHour(yearlypeak_hour, year_days);
}

void View_Reports()
{
    int choice;

    while (true)
    {
        cout << "\n=== VIEW REPORTS === \n\n1. View customer Reports \n2. View Teller Reports \n3. Export Reports To Files \n4. Back to Main Menu \n";
        if (!(cin >> choice))
        {
            system("cls");

            cout << "Invalid Choice. Please enter a valid number.\n";
            cin.clear();
            cin.ignore(10000, '\n');
            continue;
        }
        cin.ignore();
        system("cls");

        switch (choice)
        {
        case 1:
            Customer_Reports();
            break;

        case 2:
            Teller_Reports();
            break;

        case 3:
            Export_Reports();
            break;

        case 4:
            return;

        default:
            system("cls");
            cout << "Invalid Choice. Please enter a valid number.\n";
        }
    }
}

void Customer_Reports()
{
    cout << left
         << setw(6) << "ID"
         << setw(12) << "Arr Time"
         << setw(12) << "Svc Start"
         << setw(12) << "Svc End"
         << setw(20) << "Transaction"
         << setw(8) << "Served" << "\n";

    cout << string(70, '-') << "\n";

    for (int i = 0; i < customer_no; i++)
    {
        cout << left
             << setw(6) << customer[i].id
             << setw(12) << customer[i].arr_time
             << setw(12) << customer[i].svc_start
             << setw(12) << customer[i].svc_end
             << setw(20) << customer[i].transaction
             << setw(8) << (customer[i].served ? "Yes" : "No") << "\n";
    }
}

void Teller_Reports()
{
    cout << left
         << setw(12) << "Teller ID"
         << setw(12) << "Customers"
         << setw(12) << "Idle (min)"
         << setw(16) << "Working (min)"
         << setw(14) << "Utilization" << "\n";

    cout << string(66, '-') << "\n";

    for (int i = 0; i < teller_no[sim_day - 1]; i++)
    {
        int working_time = (total_closing - total_opening) - tellers[i].idle_time;
        double util = 100.0 * working_time / (total_closing - total_opening);

        cout << left
             << setw(12) << tellers[i].id
             << setw(12) << tellers[i].customers_served
             << setw(12) << tellers[i].idle_time
             << setw(16) << working_time
             << fixed << setprecision(2) << util << "%\n";
    }
}

void Export_Reports()
{
    // Export Customers Report
    ofstream customers_report("customers_report.txt");

    if (!customers_report.is_open())
    {
        cout << "Error: Could not open customers_report.txt for writing.\n";
    }
    else
    {
        customers_report << left
                         << setw(6) << "ID"
                         << setw(12) << "Arr Time"
                         << setw(12) << "Svc Start"
                         << setw(12) << "Svc End"
                         << setw(20) << "Transaction"
                         << setw(8) << "Served" << "\n";

        customers_report << string(70, '-') << "\n";

        for (int i = 0; i < customer_no; i++)
        {
            customers_report << left
                             << setw(6) << customer[i].id
                             << setw(12) << customer[i].arr_time
                             << setw(12) << customer[i].svc_start
                             << setw(12) << customer[i].svc_end
                             << setw(20) << customer[i].transaction
                             << setw(8) << (customer[i].served ? "Yes" : "No") << "\n";
        }

        customers_report.close();
        cout << "Customers report exported to customers_report.txt\n";
    }

    // Export Tellers Report
    ofstream tellers_report("tellers_report.txt");

    if (!tellers_report.is_open())
    {
        cout << "Error: Could not open tellers_report.txt for writing.\n";
    }
    else
    {
        tellers_report << left
                       << setw(12) << "Teller ID"
                       << setw(12) << "Customers"
                       << setw(12) << "Idle (min)"
                       << setw(16) << "Working (min)"
                       << setw(14) << "Utilization" << "\n";

        tellers_report << string(66, '-') << "\n";

        for (int i = 0; i < teller_no[sim_day - 1]; i++)
        {
            int working_time = (total_closing - total_opening) - tellers[i].idle_time;
            double util = 100.0 * working_time / (total_closing - total_opening);

            tellers_report << left
                           << setw(12) << tellers[i].id
                           << setw(12) << tellers[i].customers_served
                           << setw(12) << tellers[i].idle_time
                           << setw(16) << working_time
                           << fixed << setprecision(2) << util << "%\n";
        }

        tellers_report.close();
        cout << "Tellers report exported to tellers_report.txt\n";
    }

    Sleep(1000);
}

void View_Statistics()
{
    if (days_count == 0)
    {
        cout << "\nNo simulation has been run yet. Please run a simulation first.\n";
        return;
    }
    utilization = 100.0 * (sum_work_hours / sum_available_time);

    cout << "\n=== Simulation Statistics ===\n";
    cout << "Total Simulation Days: " << days_count << endl;
    cout << "Total Customers: " << total_customers << "\n";
    cout << "Average Waiting Time: " << sum_waiting / total_customers << " minutes\n";
    cout << "Peak Hour (Overall): " << peak_overall1 << " - " << peak_overall2 << "\n";
    cout << "Busiest Day: " << days[peak_day - 1] << " (" << peak_customer_overall << " customers)\n";
    cout << "Average Teller Utilization: " << utilization << "%\n";
}