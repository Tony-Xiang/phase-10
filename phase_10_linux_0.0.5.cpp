#include<bits/stdc++.h>

using namespace std;

bool isout[109] = {};

int shown = 0;
int not_shown = 0;
int total_player = 0;
const int card_number = 108;

// structures

struct player_data
{
    bool isskipped;
    bool isskipped_lastround;
    int player_card[13];
    bool player_isphased[13];
    int player_score;
    int player_phase;
    int player_current;
    string player_word;
    string player_password;
    string player_enter;
};
player_data player[11];

struct phase_data
{
    int phase_number;
    bool isset;
    int set_num;
    bool isrun;
    int run_start;
    int run_stop;
    bool issamecolor;
};

phase_data phase[21];

// functions

void play_game();

void initialize(int total_player);

int card_out();

bool isclear(int player_number);

void round(int player_number);

void show_card(int player_number);

bool cmp(int a, int b);

void transfer(int card_number);

void pick_card(int choice, int player_number);

int phase_card(int player_number, int phase_number);

int discard_card(bool discard_module, int player_number);

int get_number(string enter);

int search_card_number(int start, int stop, int card_number, int player_number);

int search_card_order(int start, int stop, int card_number, int player_number);

int skip_person(int player_number);

int enter_set(int set_volume, int player_number, int return_value);

int check_set(int *arr, int total);

int enter_hit_location();

void show_phase();

int hit_phase(int player_number);

int search_phase(int num);

void calculate_score(int player_number);

void show_score();

int main()
{
    play_game();
    return 0;
}

void play_game()
{
    system("clear");
    cout<<"Enter the players: ";
    while (!(cin>>total_player) || (total_player <= 0) || (total_player > 10))
    {
        cin.clear();
        while (cin.get() != '\n')
            continue;
        cout<<"Wrong syntax! Please reenter: ";
    }
    cin.get();
    initialize(total_player);

    cout<<"The game is ready to start.\n";
    system("sleep 2");
    system("clear");

    srand(time(0));
    int start_from = rand() % total_player + 1;
    int start_card = card_out();
    int random_card = card_out();
    int discard_card = 0;

    // initial cards
    shown = start_card;
    not_shown = random_card;

    int player_number = start_from;
    if (start_card >= 105)
        player[player_number].isskipped = true;

    while (1)
    {
        if (!player[player_number].isskipped)
        {
            round(player_number);
            player[player_number].isskipped_lastround = false;
            player[player_number].isskipped = false;
            if (isclear(player_number))
                break;
        }
        else
        {
            cout<<"Player "<<player_number<<", you are skipped this round.\n";
            player[player_number].isskipped = false;
            player[player_number].isskipped_lastround = true;
        }
        player_number = player_number % total_player + 1;
        // user message control
        if (total_player == 2)
            cout<<"Get ready for your opponent's round.\n";
        else if (total_player > 2)
            cout<<"Get ready for your opponents' round.\n";
        else
            cout<<"Play yourself happily!\n";
        system("sleep 2");
        system("clear");
    }


    // calculate scores
    for (int i = 1; i <= total_player; i++)
        calculate_score(i);
    // show scores
    system("clear");
    cout<<"Hand complete!\nThe scores now:\n";
    show_score();

    cout<<"Get ready for next hand.\n";
}

void initialize(int total_player)
{
    system("clear");

    // initialize player
    for (int i = 1; i <= 10; i++)
        for (int j = 1; j <= 11; j++)
            player[i].player_card[j] = 0;

    // initialize card_numbers
    srand(time(0));
    for (int i = 1; i <= total_player; i++)
        for (int j = 1; j <= 10; j++)
            player[i].player_card[j] = card_out();

    // initialize phase status
    for (int i = 1; i <= 10; i++)
        for (int j = 1; j <= 11; j++)
            player[i].player_isphased[j] = false;

    // initialize player phase status
    for (int i = 1; i <= 10; i++)
    {
        player[i].player_phase = 0;
        player[i].player_current = 1;
    }

    // initialize skip status
    for (int i = 1; i <= 10; i++)
    {
        player[i].isskipped = false;
        player[i].isskipped_lastround = false;
    }

    // enter password
    for (int i = 1; i <= total_player; i++)
    {
        cout<<"Player "<<i<<", please enter your password: ";
        getline(cin,player[i].player_password);
        system("clear");
    }

    return;
}

int card_out()
{
    srand(time(0));
    int start_card = rand() % card_number + 1;
    if (isout[start_card])
        while (isout[start_card])
            start_card = rand() % card_number + 1;
    isout[start_card] = true;
    return start_card;
}

bool isclear(int player_number)
{
    bool check = false;
    for (int j = 1; j <= 11; j++)
        if (player[player_number].player_card[j] != 0)
            return false;
    return true;
}

void round(int player_number)
{
    cin.clear();
    cout<<"Player "<<player_number<<", enter your password: ";
    getline(cin, player[player_number].player_enter);

    while (player[player_number].player_enter != player[player_number].player_password)
    {
        cout<<"Wrong password! Please reenter!\n";
        getline(cin, player[player_number].player_enter);
    }

    show_card(player_number);
    cout<<endl;

    cout<<"The shown card is: ";
    transfer(shown);
    cout<<"And there is a random card which can't be shown.\n\n";

    if (shown <= 104)
    {
        cout<<"Pick one, enter 1 for the shown card, 2 for the random one.\n";
        int confirmer = 0;
        while ((!(cin>>confirmer)) || ((confirmer != 1) && (confirmer != 2)))
        {
            cin.clear();
            while (cin.get() != '\n')
                continue;
            cout<<"Wrong syntax! Please reenter: ";
        }
        pick_card(confirmer, player_number);
        if (confirmer == 2)
            not_shown = card_out();
    }
    else
    {
        cout<<"You can't pick Skip Card.\nAutomatically take the random card instead.\n";
        pick_card(2, player_number);
        not_shown = card_out();
    }

    // phase
    if (player[player_number].player_phase != 1)
    {
        system("sleep 1");
        system("clear");
        show_card(player_number);
        cout<<"Do you have a phase? Enter \"yes(y)\" to confirm.\n";
        string confirmer_phase = "";
        while (confirmer_phase == "")
            getline(cin, confirmer_phase);
        if ((confirmer_phase == "y") || (confirmer_phase == "yes"))
        {
            player[player_number].player_phase = phase_card(player_number, player[player_number].player_current);
            int counter_phase = 1;
            confirmer_phase = "";
            while (player[player_number].player_phase == -1)
            {
                player[player_number].player_phase = phase_card(player_number, player[player_number].player_current);
                counter_phase++;
                if (player[player_number].player_phase != -1)
                    break;
                if (counter_phase == 3)
                {
                    cout<<"\nYou have entered the wrong card 3 times.Are you sure you have the right card?\nEnter \"yes(y)\" to confirm.\n";
                    while (confirmer_phase == "")
                        getline(cin, confirmer_phase);
                    if ((confirmer_phase == "y") || (confirmer_phase == "yes"))
                    {
                        cout<<"Please reenter: ";
                        continue;
                    }
                    else
                        break;
                }
                if (counter_phase == 4)
                    cout<<"\nYou have only one chance to enter!\nPlease reenter: ";
                if (counter_phase == 5)
                {
                    cout<<"\nSorry, force break!\n";
                    break;
                }
            }
        }
    }

    // hit
    if (player[player_number].player_phase == 1)
    {
        system("sleep 1");
        system("clear");
        show_card(player_number);
        cout<<"You have phased! Do you have cards to hit the phases?\nEnter \"yes(y)\" to confirm.\n";
        cout<<"By the way, the phases on the desk are:\n";
        show_phase();
        cout<<endl;
        string confirmer_hit = "";
        while (confirmer_hit == "")
            getline(cin, confirmer_hit);
        if ((confirmer_hit == "y") || (confirmer_hit == "yes"))
        {
            int counter_hit = 1;
            confirmer_hit = "";
            cout<<"Enter the card you want to hit: ";
            int copyhit = hit_phase(player_number);
            while (copyhit < 0)
            {
                copyhit = hit_phase(player_number);
                if (copyhit == -1)
                    counter_hit++;
                else
                {
                    counter_hit = 0;
                    confirmer_hit = "";
                }
                if (copyhit >= 0)
                    break;
                if (counter_hit == 3)
                {
                    cout<<"\nYou have entered the wrong card 3 times.Are you sure you have the right card?\nEnter \"yes(y)\" to confirm.\n";
                    while (confirmer_hit == "")
                        getline(cin, confirmer_hit);
                    if ((confirmer_hit == "y") || (confirmer_hit == "yes"))
                    {
                        cout<<"Please reenter: ";
                        continue;
                    }
                    else
                        break;
                }
                if (counter_hit == 4)
                    cout<<"\nYou have only one chance to enter!\nPlease reenter: ";
                if (counter_hit == 5)
                {
                    cout<<"\nSorry, force break!\n";
                    break;
                }
            }
        }
    }

    // prejudge if a hand ends
    if (isclear(player_number))
        return; // return when ends...

    // discard
    system("sleep 1");
    system("clear");
    show_card(player_number);
    cout<<"Enter a card you want to discard: ";
    int copyshown = discard_card(1, player_number);
    while (copyshown == -1)
        copyshown = discard_card(1, player_number);
    shown = copyshown;

    if ((shown >= 105) && (shown <= 108) && (total_player > 2))
    {
        cout<<"You can only skip one person. Choose one: ";
        int choice = skip_person(player_number);
        while (choice == -1)
            choice = skip_person(player_number);
        player[choice].isskipped = true;
        cin.get();
    }

    if ((shown >= 105) && (total_player == 2) && (!player[player_number % 2 + 1].isskipped) && (!player[player_number % 2 + 1].isskipped_lastround))
        player[player_number % 2 + 1].isskipped = true;
    else if ((shown >= 105) && (total_player == 2) && (player[player_number % 2 + 1].isskipped_lastround))
        cout<<"This skip card won't take effect.\n";

    if ((total_player == 1) && (shown >= 105))
        cout<<"Skip card won't help! Play around yourself!\n";

    // prejudge if a hand ends
    if (isclear(player_number))
        return; // return when ends...

    return;
}

int skip_person(int player_number)
{
    int choice = 0;
    if (!(cin>>choice))
    {
        cin.clear();
        while (cin.get() != '\n')
            continue;
        cout<<"Wrong syntax! Please reenter: ";
        return -1;
    }
    else if ((choice <= 0) || (choice > total_player))
    {
        cout<<"Out of range! Please reenter: ";
        return -1;
    }
    else if (choice == player_number)
    {
        cout<<"Don't be ridiculous! You can't skip yourself! Please reenter: ";
        return -1;
    }
    else if (player[choice].isskipped_lastround)
    {
        cout<<"You can't skip whoever is skipped the last round! Please reenter: ";
        return -1;
    }
    else if (player[choice].isskipped)
    {
        cout<<"You can't skip whoever is skipped now! Please reenter: ";
        return -1;
    }
    else
        return choice;
}

void show_card(int player_number)
{
    system("clear");
    cout<<"You own:\n";
    sort(player[player_number].player_card + 1, player[player_number].player_card + 11 + 1, cmp);
    for (int i = 1; i <= 11; i++)
        if (player[player_number].player_card[i] != 0)
        {
            transfer(player[player_number].player_card[i]);
            cout<<endl;
            // cout<<"Card No."<<player[player_number].player_card[i]<<"\n\n";
        }
    return;
}

bool cmp(int a, int b)
{
    if ((a <= 96) && (b <= 96) && ((a - 1) / 2 % 12 + 1 != (b - 1) / 2 % 12 + 1))
        return ((a - 1) / 2 % 12 + 1 < (b - 1) / 2 % 12 + 1);
    else
        return (a < b);
}

void transfer(int card_number)
{
    if (card_number <= 96)
        switch ((card_number - 1) / 24)
        {
            case 0:
                cout<<"Red "<<(card_number - 1) / 2 % 12 + 1<<"\n";
                break;
            case 1:
                cout<<"Yellow "<<(card_number - 1) / 2 % 12 + 1<<"\n";
                break;
            case 2:
                cout<<"Blue "<<(card_number - 1) / 2 % 12 + 1<<"\n";
                break;
            case 3:
                cout<<"Green "<<(card_number - 1) / 2 % 12 + 1<<"\n";
                break;
        }
    else if (card_number >= 105)
        cout<<"Skip Card\n";
    else if (card_number <= 0)
        return;
    else
        cout<<"Wild Card\n";
    return;
}

void pick_card(int choice, int player_number)
{
    if (choice == 1)
    {
        player[player_number].player_card[1] = shown;
        show_card(player_number);
    }
    else if (choice == 2)
    {
        cout<<"You get a ";
        transfer(not_shown);
        system("sleep 3");
        player[player_number].player_card[1] = not_shown;
        show_card(player_number);
    }
    return;
}

int phase_card(int player_number, int phase_number)
{
    for (int i = 1; i <= 11; i++)
        player[player_number].player_isphased[i] = false;

    switch (phase_number)
    {
        case 1:
        {
            for (int i = 1; i <= 2 * total_player; i++)
            {
                phase[i].isset = true;
                phase[i].isrun = false;
            }
            cout<<"\nPhase 1: 2 sets of 3.\nEnter the first set of 3.\n";
            // part 1
            int phase1 = enter_set(3, player_number, 2 * player_number - 1);
            if (phase1 == -1)
                return -1;
            cout<<"Correct! Enter the next set of 3.\n";
            // part 2
            phase1 = enter_set(3, player_number, 2 * player_number);
            if (phase1 == -1)
            {
                cout<<"Wrong phase! Please reenter the whole phase!\n";
                return -1;
            }

            cout<<"Congratulations! You phase the card!\n";
            player[player_number].player_current++;

            break;
        }
    }

    // clear the cards
    for (int i = 1; i <= 11; i++)
    {
        if (player[player_number].player_isphased[i])
            player[player_number].player_card[i] = 0;
        player[player_number].player_isphased[i] = false; // reset phase status
    }
    system("sleep 1");

    // reshow the cards
    show_card(player_number);

    return 1;
}

int enter_set(int set_volume, int player_number, int return_value)
{
    int pool[13] = {};
    for (int i = 1; i <= set_volume; i++)
    {
        cout<<"Card "<<i<<": ";
        pool[i] = discard_card(0, player_number);
        if ((pool[i] >= 105) || (player[player_number].player_isphased[search_card_order(pool[i], pool[i], 11, player_number)]) || (pool[i] == -1))
            return -1;
        player[player_number].player_isphased[search_card_order(pool[i], pool[i], 11, player_number)] = true;
        if ((pool[i] <= 96) && (pool[i] % 2 != 0))
            pool[i]++;
    }

    cout<<"Checking the validity of the cards...\nPlease wait...\n";
    int copyset_num = check_set(pool, 3);
    if (copyset_num > 0)
        phase[return_value].set_num = copyset_num;
    else
    {
        cout<<"Wrong phase! Please reenter the whole phase!\n";
        for (int i = 1; i <= 3; i++)
            pool[i] = 0;
        for (int i = 1; i <= 11; i++)
            player[player_number].player_isphased[i] = false;
        return -1;
    }
    return 0;
}

int discard_card(bool discard_module, int player_number)
{
    string card_enter = "";
    int card_number = 0;
    int card_order = 0;

    if ((get_number(card_enter) == -1) && (card_enter != ""))
        return -1;

    while (card_enter == "")
        getline(cin, card_enter);
    int card_range = 0;
    card_range = get_number(card_enter);

    if ((card_range > 0) && (card_range <= 96))
    {
        card_number = search_card_number(card_range - 1, card_range, 11, player_number);
        card_order = search_card_order(card_range - 1, card_range, 11, player_number);
        if (card_number != 0)
        {
            if (discard_module)
            {
                cout<<"You discard a ";
                transfer(card_number);
                player[player_number].player_card[card_order] = 0;
                system("sleep 2.0");
                show_card(player_number);
            }
            return card_number;
        }
        else
        {
            cout<<"You don't have a ";
            transfer(card_range);
            cout<<"Please reenter: ";
            return -1;
        }
    }
    else if ((card_range >= 97) && (card_range <= 104))
    {
        card_number = search_card_number(card_range, card_range + 7, 11, player_number);
        card_order = search_card_order(card_range, card_range + 7, 11, player_number);
        if (card_number != 0)
        {
            if (discard_module)
            {
                cout<<"You discard a ";
                transfer(card_number);
                player[player_number].player_card[card_order] = 0;
                system("sleep 2.0");
                show_card(player_number);
            }
            return card_number;
        }
        else
        {
            cout<<"You don't have a ";
            transfer(card_range);
            cout<<"Please reenter: ";
            return -1;
        }
    }
    else if ((card_range >= 105) && (card_range <= 108))
    {
        card_number = search_card_number(card_range, card_range + 3, 11, player_number);
        card_order = search_card_order(card_range, card_range + 3, 11, player_number);
        if (card_number != 0)
        {
            if (discard_module)
            {
                cout<<"You discard a ";
                transfer(card_number);
                player[player_number].player_card[card_order] = 0;
                system("sleep 2.0");
                show_card(player_number);
            }
            return card_number;
        }
        else
        {
            cout<<"You don't have a ";
            transfer(card_range);
            cout<<"Please reenter: ";
            return -1;
        }
    }
    else
        return -1;
}

int get_number(string enter)
{
    string middle;
    int output = 0;
    for (int i = 0; i < enter.size(); i++)
        if (enter[i] != ' ')
            middle += tolower(enter[i]);

    // for red
    string red_check = "";
    red_check = red_check + middle[0] + (middle[1]) + middle[2];
    if ((middle[0] == 'r') && (middle[1] >= '0') && (middle[1] <= '9'))
    {
        if ((middle[2] >= '0') && (middle[2] <= '2') && (middle[1] == '1'))
            output = 0 + 2 * (int(middle[1] - '0') * 10 + int(middle[2] - '0'));
        else
            output = 0 + 2 * int(middle[1] - '0');
    }
    else if ((red_check == "red") && (middle[3] >= '0') && (middle[3] <= '9'))
    {
        if ((middle[4] >= '0') && (middle[4] <= '2') && (middle[3] == '1'))
            output = 0 + 2 * (int(middle[3] - '0') * 10 + int(middle[4] - '0'));
        else
            output = 0 + 2 * int(middle[3] - '0');
    }

    // for yellow
    string yellow_check = "";
    yellow_check = yellow_check + middle[0] + middle[1] + middle[2] + middle[3] + middle[4] + middle[5];
    if ((middle[0] == 'y') && (middle[1] >= '0') && (middle[1] <= '9'))
    {
        if ((middle[2] >= '0') && (middle[2] <= '2') && (middle[1] == '1'))
            output = 24 + 2 * (int(middle[1] - '0') * 10 + int(middle[2] - '0'));
        else
            output = 24 + 2 * int(middle[1] - '0');
    }
    else if ((yellow_check == "yellow") && (middle[6] >= '0') && (middle[6] <= '9'))
    {
        if ((middle[7] >= '0') && (middle[7] <= '2') && (middle[6] == '1'))
            output = 24 + 2 * (int(middle[6] - '0') * 10 + int(middle[7] - '0'));
        else
            output = 24 + 2 * int(middle[6] - '0');
    }

    // for blue
    string blue_check = "";
    blue_check = blue_check + middle[0] + middle[1] + middle[2] + middle[3];
    if ((middle[0] == 'b') && (middle[1] >= '0') && (middle[1] <= '9'))
    {
        if ((middle[2] >= '0') && (middle[2] <= '2') && (middle[1] == '1'))
            output = 48 + 2 * (int(middle[1] - '0') * 10 + int(middle[2] - '0'));
        else
            output = 48 + 2 * int(middle[1] - '0');
    }
    else if ((blue_check == "blue") && (middle[4] >= '0') && (middle[4] <= '9'))
    {
        if ((middle[5] >= '0') && (middle[5] <= '2') && (middle[4] == '1'))
            output = 48 + 2 * (int(middle[4] - '0') * 10 + int(middle[5] - '0'));
        else
            output = 48 + 2 * int(middle[4] - '0');
    }

    // for green
    string green_check = "";
    green_check = green_check + middle[0] + middle[1] + middle[2] + middle[3] + middle[4];
    if ((middle[0] == 'g') && (middle[1] >= '0') && (middle[1] <= '9'))
    {
        if ((middle[2] >= '0') && (middle[2] <= '2') && (middle[1] == '1'))
            output = 72 + 2 * (int(middle[1] - '0') * 10 + int(middle[2] - '0'));
        else
            output = 72 + 2 * int(middle[1] - '0');
    }
    else if ((green_check == "green") && (middle[5] >= '0') && (middle[5] <= '9'))
    {
        if ((middle[6] >= '0') && (middle[6] <= '2') && (middle[5] == '1'))
            output = 72 + 2 * (int(middle[5] - '0') * 10 + int(middle[6] - '0'));
        else
            output = 72 + 2 * int(middle[5] - '0');
    }

    // for wild cards
    string wild_check_a = "";
    string wild_check_b = "";
    wild_check_a = wild_check_a + middle[0] + middle[1] + middle[2] + middle[3];
    wild_check_b = wild_check_b + middle[0] + middle[1] + middle[2] + middle[3] + middle[4] + middle[5] + middle[6] + middle[7];
    if ((middle[0] == 'w') && ((wild_check_a == "wild") || (wild_check_b == "wildcard")) || (middle == "w") || (middle == "wc"))
        output = 97;

    // for skip cards
    string skip_check_a = "";
    string skip_check_b = "";
    skip_check_a = skip_check_a + middle[0] + middle[1] + middle[2] + middle[3];
    skip_check_b = skip_check_b + middle[0] + middle[1] + middle[2] + middle[3] + middle[4] + middle[5] + middle[6] + middle[7];
    if ((middle[0] == 's') && ((skip_check_a == "skip") || (skip_check_b == "skipcard")) || (middle == "s") || (middle == "sc"))
        output = 105;

    if (output != 0)
        return output;

    if ((enter != "") && (output == 0))
    {
        cout<<"Wrong syntax! Please check and reenter: ";
        return -1;
    }

    if (enter == "")
        return -1;
}

int search_card_number(int start, int stop, int card_number, int player_number)
{
    for (int i = 1; i <= card_number; i++)
        for (int j = start; j <= stop; j++)
            if ((player[player_number].player_card[i] == j) && (!player[player_number].player_isphased[i]))
                return j;
    return 0;
}

int search_card_order(int start, int stop, int card_number, int player_number)
{
    for (int i = 1; i <= card_number; i++)
        for (int j = start; j <= stop; j++)
            if ((player[player_number].player_card[i] == j) && (!player[player_number].player_isphased[i]))
                return i;
    return 0;
}

int check_set(int *arr, int total)
{
    // deal with raw data
    for (int i = 1; i <= total; i++)
        if ((*(arr + i) >= 97) && (*(arr + i) <= 104))
            *(arr + i) = 13;
        else
            *(arr + i) = (*(arr + i) - 2) % 24 / 2 + 1;
    // check if it's all wild cards & find where the first normal card is...
    int counter = 1;
    for (counter = 1; counter <= total; counter++)
        if (*(arr + counter) != 13)
            break;
        else if (counter == total)
        {
            cout<<"You must at least drop a NORMAL CARD!\n";
            return 0;
        }
    // check if it's a set
    for (int i = 1; i <= total; i++)
        if ((*(arr + i) != *(arr + counter)) && (*(arr + i) != 13))
            return 0;
    return *(arr + counter);
}

void show_phase()
{
    for (int i = 1; i <= 2 * total_player; i++)
        if (phase[i].set_num != 0)
            cout<<"A set of "<<phase[i].set_num<<endl;
    return;
}

int hit_phase(int player_number)
{
    int hit_seq = 0;
    int hit = discard_card(0, player_number);
    if (hit == -1)
        return -1;
    if (hit <= 96)
        hit_seq = ((hit - 1) % 24 / 2 + 1);
    else if (hit >= 105)
        hit_seq = 14;
    else
        hit_seq = 13;
    int counter = search_phase(hit_seq);
    if (counter == 0)
    {
        cout<<"Phase to hit not found! Please reenter: ";
        return -1;
    }
    else if (counter >= 1)
    {
        player[player_number].player_card[search_card_order(hit, hit, 11, player_number)] = 0;
        show_card(player_number);

        // prejudge if a hand ends
        if (isclear(player_number))
            return 100; // return when ends...

        cout<<"Do you have other cards to hit? Enter \"yes(y)\" to confirm.\n";
        string confirmer_hit = "";
        while (confirmer_hit == "")
            getline(cin, confirmer_hit);
        if ((confirmer_hit == "y") || (confirmer_hit == "yes"))
        {
            cout<<"Please enter the card you want to hit: ";
            return -2;
        }
        else return 0;
    }
    else
    {
        cout<<"You can hit more than 1 phase because you drop a Wild Card.\nPlease choose one by entering the number of a set.\nPlease enter: ";
        int phase_location = enter_hit_location();
        while (phase_location == -1)
            phase_location = enter_hit_location();
        player[player_number].player_card[search_card_order(hit, hit, 11, player_number)] = 0;
        show_card(player_number);

        // prejudge if a hand ends
        if (isclear(player_number))
            return 100; // return when ends...

        cout<<"Do you have other cards to hit? Enter \"yes(y)\" to confirm.\n";
        string confirmer_hit = "";
        while (confirmer_hit == "")
            getline(cin, confirmer_hit);
        if ((confirmer_hit == "y") || (confirmer_hit == "yes"))
        {
            cout<<"Please enter the card you want to hit: ";
            return -2;
        }
        else return 0;
    }
}

int enter_hit_location()
{
    int location;
    if (!(cin>>location))
    {
        cout<<"Wrong syntax!\nThe phases are: ";
        show_phase();
        cout<<"\nPlease reenter: ";
        return -1;
    }
    else if (search_phase(location) <= 0)
    {
        cout<<"Hit not found!\nThe phases are: ";
        show_phase();
        cout<<"\nPlease reenter: ";
        return -1;
    }
    else return 0;
}

int search_phase(int num)
{
    // can extend here, but finish first...
    int counter = 0;
    for (int i = 1; i <= 2 * total_player; i++)
    {
        if (phase[i].isset)
        {
            if ((num == phase[i].set_num) || (num == 13))
                counter++;
        }
    }
    if (num != 13)
        return counter;
    else
        return -1;
}

void calculate_score(int player_number)
{
    for (int i = 1; i <= 11; i++)
    {
        if ((player[player_number].player_card[i] <= 96) && (player[player_number].player_card[i] > 0))
        {
            if ((player[player_number].player_card[i] - 1) % 24 / 2 + 1 <= 9)
                player[player_number].player_score += 5;
            else
                player[player_number].player_score += 10;
        }
        else if (player[player_number].player_card[i] >= 105)
            player[player_number].player_score += 15;
        else if ((player[player_number].player_card[i] >= 97) && (player[player_number].player_card[i] <= 104))
            player[player_number].player_score += 25;
    }
}

void show_score()
{
    int max_phase = 0;
    int min_score = 100000000;
    int winner_player_current = 0;
    // rank
    for (int i = 1; i <= total_player; i++)
    {
        if ((player[i].player_current >= max_phase) && (player[i].player_score < min_score))
        {
            max_phase = player[i].player_current;
            min_score = player[i].player_score;
            winner_player_current = i;
        }
    }
    cout<<"Current winner: Player "<<winner_player_current<<endl<<"Statistics:\n";
    for (int i = 1; i <= total_player; i++)
        if (i == winner_player_current)
            cout<<"[+]Player "<<i<<": Phase "<<player[i].player_current<<", Score "<<player[i].player_score<<endl;
        else
            cout<<"[-]Player "<<i<<": Phase "<<player[i].player_current<<", Score "<<player[i].player_score<<endl;
}
