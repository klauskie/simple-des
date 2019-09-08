#include <iostream>
#include <string>

using namespace std;

/* TABLES */
const int P10[10] = {3, 5, 2, 7, 4, 10, 1, 9, 8, 6};
const int P8[8] = {6, 3, 7, 4, 8, 5, 10, 9};
const int IP[8] = {2, 6, 3, 1, 4, 8, 5, 7};
const int IP1[8] = {4, 1, 3, 5, 7, 2, 8, 6};
const int EP[8] = {4, 1, 2, 3, 2, 3, 4, 1};
const int P4[4] = {2, 4, 3, 1};
const std::string S0[4][4] =
        {
                {"01", "00", "11", "10"},
                {"11", "10", "01", "00"},
                {"00", "10", "01", "11"},
                {"11", "01", "11", "10"}};
const std::string S1[4][4] =
        {
                {"00", "01", "10", "11"},
                {"10", "00", "01", "11"},
                {"11", "00", "01", "00"},
                {"10", "01", "00", "11"}};
/* END TABLES */

// int | bin  to bin shape string
std::string bin_to_string(int bits, int cut = 0)
{
    std::string bin_string = "0000000000";
    short count = 10;
    while (bits)
    {
        count--;
        if (bits & 1)
        {
            bin_string[count] = '1';
        }
        else
        {
            bin_string[count] = '0';
        }
        bits >>= 1;
    }

    return bin_string.substr(cut, bin_string.size());
}

std::string left_shift(string bits)
{
    std::string result = bits;
    int size = bits.size();
    for (int i = 0; i <= size - 1; i++)
    {
        result[i] = bits[(i + 1) % size];
    }
    return result;
}

std::string shuffle_10(string bits)
{
    std::string result = "0000000000";
    for (int i = 0; i < 10; i++)
    {
        result[i] = bits[P10[i] - 1];
    }
    return result;
}

std::string shuffle_8(string bits, int rule)
{
    std::string result = "0000000000";
    for (int i = 0; i < 10; i++)
    {
        if (rule == 1)
        {
            result[i + 2] = bits[P8[i] - 1];
        }
        else if (rule == 2)
        {
            result[i + 2] = bits[IP[i] - 1];
        }
        else if (rule == 3)
        {
            result[i + 2] = bits[EP[i] - 1];
        }
        else if (rule == 4)
        {
            result[i + 2] = bits[IP1[i] - 1];
        }
    }
    return result.substr(2, 10);
}

std::string shuffle_4(string bits, int rule)
{
    std::string result = "000000";
    for (int i = 0; i < 4; i++)
    {
        if (rule == 1)
        {
            result[i + 2] = bits[P4[i] - 1];
        }
    }
    return result.substr(2, 6);
}

std::string funko(std::string text_WORKABLE, std::string text_EXTRA, int KEY) {
    std::cout << "text_IP_Half : " << text_WORKABLE << std::endl;

    // Work with Half
    std::string H1_EP = shuffle_8(text_WORKABLE, 3);
    std::cout << "H1 - EP : " << H1_EP << std::endl;

    int H1_EP_bits = stoi(H1_EP, nullptr, 2);
    std::string H1_EP_XOR_KEY_1 = bin_to_string(H1_EP_bits ^ KEY, 2);
    std::cout << "H1_EP_XOR_KEY_1 : " << H1_EP_XOR_KEY_1 << std::endl;

    std::string x;
    x += H1_EP_XOR_KEY_1[0];
    x += +H1_EP_XOR_KEY_1[3];
    int left = stoi(x, nullptr, 2);
    std::cout << "x : " << left << "\n";

    std::string y;
    y += H1_EP_XOR_KEY_1[1];
    y += +H1_EP_XOR_KEY_1[2];
    int right = stoi(y, nullptr, 2);
    std::cout << "y : " << right << "\n";

    std::string left_part = S0[right][left];
    std::cout << "left_part : " << left_part << "\n";

    std::string x2;
    x2 += H1_EP_XOR_KEY_1[4];
    x2 += +H1_EP_XOR_KEY_1[7];
    int left2 = stoi(x2, nullptr, 2);
    std::cout << "x2 : " << left2 << "\n";

    std::string y2;
    y2 += H1_EP_XOR_KEY_1[5];
    y2 += +H1_EP_XOR_KEY_1[6];
    int right2 = stoi(y2, nullptr, 2);
    std::cout << "y2 : " << right2 << "\n";

    std::string baka = S0[left][right] + S1[left2][right2];

    std::cout << "baka : " << baka << std::endl;

    // PUNANI
    int baka_P4 = stoi(shuffle_4(baka, 1), nullptr, 2);
    std::cout << "shuffle_4(baka, 1) : " << shuffle_4(baka, 1) << std::endl;
    std::cout << "text_EXTRA : " << text_EXTRA << std::endl;
    int baka_P4_H1 = baka_P4 ^ stoi(text_EXTRA, nullptr, 2);
    std::string baka_P4_H1_txt = bin_to_string(baka_P4_H1, 0);
    std::cout << "baka_P4_H1_txt : " << baka_P4_H1_txt << std::endl;

    return  baka_P4_H1_txt.substr(6);

}

std::string init_encrypt(std::string key10, std::string plain_text)
{


    // GET KEYS
    std::string key10_mutaded_ls = left_shift(shuffle_10(key10));

    int KEY1 = stoi(shuffle_8(key10_mutaded_ls, 1), nullptr, 2);

    std::string key10_h1_str = key10_mutaded_ls.substr(0, 5);
    std::string key10_h2_str = key10_mutaded_ls.substr(5, 10);

    std::string key10_h1_l2 = left_shift(left_shift(key10_h1_str));
    std::string key10_h2_l2 = left_shift(left_shift(key10_h2_str));

    int KEY2 = stoi(shuffle_8(key10_h1_l2 + key10_h2_l2, 1), nullptr, 2);

    std::cout << "KEY 1 : " << bin_to_string(KEY1) << " | KEY 2 : " << bin_to_string(KEY2) << std::endl;

    // ENCRYPT
    std::string text_IP = shuffle_8(plain_text, 2);
    std::cout << "Plan text - IP : " << text_IP << std::endl;

    std::string text_IP_H1 = text_IP.substr(0, 4);
    std::string text_IP_H2 = text_IP.substr(4, 8);

    // Work with H1
    std::string moist = funko(text_IP_H2,text_IP_H1, KEY1);

    std::cout << " --------------- " << std::endl;

    std::string endgame = funko(moist, text_IP_H2, KEY2) + moist;
    std::cout << endgame << "\n";

    std::cout << shuffle_8(endgame,4) << "\n";
    return shuffle_8(endgame,4);
}


int main()
{
    //std::string plain_text = "00101000";
    //std::string key = "1100011110";

    std::string plain_text = "00001011";
    std::string key = "1001100100";

    std::string cypher = init_encrypt(key, plain_text);
    //tests();
    return 0;
}