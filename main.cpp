#include <iostream>
#include <fstream>
#include <vector>

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

std::string int_to_string(int bits, int cut = 0)
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

std::string exor(std::string top, std::string bottom) {
    std::string result = top;
    for(int i = 0; i < top.size(); i++){
        if (top[i] == bottom[i]) {
            result[i] = '0';
        } else {
            result[i] = '1';
        }
    }
    return result;
}

std::string left_shift(std::string bits)
{
    std::string result = bits;
    for (int i = 0; i <= bits.size() - 1; i++)
    {
        result[i] = bits[(i + 1) % bits.size()];
    }
    return result;
}

std::string shuffle_10(std::string bits)
{
    std::string result = bits;
    for (int i = 0; i < 10; i++)
    {
        result[i] = bits[P10[i] - 1];
    }
    return result;
}

std::string shuffle_8(std::string bits, int rule)
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

std::string shuffle_4(std::string bits, int rule)
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

std::string funko(const std::string& text_WORKABLE, const std::string& text_EXTRA, const std::string& KEY, bool comments = false) {

    // Work with Half
    std::string H1_EP = shuffle_8(text_WORKABLE, 3);
    if (comments) {
        std::cout << "Half -> EP : " << H1_EP << "\n";
    }

    std::string H1_EP_XOR_KEY_1 = exor(H1_EP,KEY);
    if (comments) {
        std::cout << "Half -> EP X KEY : " << H1_EP_XOR_KEY_1 << "\n";
    }

    std::string x;
    x += H1_EP_XOR_KEY_1[0];
    x += H1_EP_XOR_KEY_1[3];
    int left = stoi(x, nullptr, 2);

    std::string y;
    y += H1_EP_XOR_KEY_1[1];
    y += H1_EP_XOR_KEY_1[2];
    int right = stoi(y, nullptr, 2);

    std::string x2;
    x2 += H1_EP_XOR_KEY_1[4];
    x2 += H1_EP_XOR_KEY_1[7];
    int left2 = stoi(x2, nullptr, 2);

    std::string y2;
    y2 += H1_EP_XOR_KEY_1[5];
    y2 += H1_EP_XOR_KEY_1[6];
    int right2 = stoi(y2, nullptr, 2);

    std::string s0s1 = S0[left][right] + S1[left2][right2];
    if (comments) {
        std::cout << "S0 = ( " << left << ", " << right << ") : S1 = ( " << left2 << ", " << right2 << ")\n";
        std::cout << "S0 + S1 : " << s0s1 << "\n";
    }

    std::string s0s1_P4 = shuffle_4(s0s1, 1);
    if (comments) {
        std::cout << "S0S1 -> P4 : " << s0s1_P4 << "\n";
    }

    std::string s0s1_P4_H1_txt = exor(s0s1_P4, text_EXTRA);

    return  s0s1_P4_H1_txt;

}

std::string getKEY1(const std::string& key10) {
    std::string key10_mutaded_ls = left_shift(shuffle_10(key10));

    return shuffle_8(key10_mutaded_ls, 1);
}

std::string getKEY2(const std::string& key10) {
    std::string key10_mutaded_ls = left_shift(shuffle_10(key10));

    std::string key10_h1_str = key10_mutaded_ls.substr(0, 5);
    std::string key10_h2_str = key10_mutaded_ls.substr(5, 10);

    std::string key10_h1_l2 = left_shift(left_shift(key10_h1_str));
    std::string key10_h2_l2 = left_shift(left_shift(key10_h2_str));

    return shuffle_8(key10_h1_l2 + key10_h2_l2, 1);
}

std::string FLDSMDFR(bool encrypt, const std::string& key10, const std::string& plain_text, bool comments = false)
{

    // GET KEYS
    std::string KEY1 = getKEY1(key10);
    std::string KEY2 = getKEY2(key10);

    if (comments) {
        std::cout << "KEY 1 : " << KEY1 << " | KEY 2 : " << KEY2 << "\n";
    }


    // ENCRYPT
    std::string text_IP = shuffle_8(plain_text, 2);
    if (comments) {
        std::cout << "text -> IP : " << text_IP << "\n";
    }

    std::string text_IP_H1 = text_IP.substr(0, 4);
    std::string text_IP_H2 = text_IP.substr(4, 8);

    // Work with H1
    if (comments) {
        std::cout << " Half 1: \n";
    }

    // init moist to store first part
    std::string moist;

    if (encrypt) {
        moist = funko(text_IP_H2,text_IP_H1, KEY1, comments);
    } else {
        moist = funko(text_IP_H2,text_IP_H1, KEY2, comments);
    }

    if (comments) {
        std::cout << " ---------------\n";
        std::cout << " Half 2: \n";
    }

    if (encrypt) {
        std::string endgame = funko(moist, text_IP_H2, KEY2, comments) + moist;
        if (comments) {
            std::cout << "\nCypher text : " << endgame << "\n";
        }
        return shuffle_8(endgame,4);
    } else {
        std::string endgame = funko(moist, text_IP_H2, KEY1, comments) + moist;
        if (comments) {
            std::cout << "\nPlain text : " << endgame << "\n";
        }
        return shuffle_8(endgame,4);
    }

}

std::vector<std::pair<std::string, std::string> > generateKeys(std::vector<std::pair<std::string, std::string> >& list) {
    std::string posibleKey;
    int success = 0;
    int total = 0;
    std::vector<std::pair<std::string, std::string> > keysTexts;

    for(int i = 0; i < 1024; i++) {
        posibleKey = int_to_string(i);

        if (list.size() == 0) {
            break;
        }

        for(int j = 0; j < list.size(); j++) {

            if (list[j].second == FLDSMDFR(true, posibleKey, list[j].first)) {
                list.erase(list.begin() + j - 1);
                success++;
                keysTexts.push_back(std::make_pair(posibleKey, list[j].first));
            }
            total++;
        }

    }


    std::cout << "Total iterations : " << total << "\n";

    return keysTexts;
}

std::string generateUniqueKey(std::vector<std::pair<std::string, std::string> >& list) {
    std::string posibleKey;
    int count = 0;
    int total = 0;

    for(int i = 0; i < 1024; i++) {
        posibleKey = int_to_string(i);

        if (list.size() == 0) {
            break;
        }

        for(int j = 0; j < list.size(); j++) {

            if (list[j].second == FLDSMDFR(true, posibleKey, list[j].first)) {
                count++;
            }
            if (count == list.size()) {
                // key found
                std::cout << "KEY : " << posibleKey << "\n";
                return posibleKey;
            }
            total++;
        }
        count = 0;

    }

    std::cout << "Total iterations : " << total << "\n";

    return "error";
}

std::vector<std::pair<std::string, std::string> > readFile() {
    std::string line;
    std::ifstream myfile("alberto.txt");

    std::vector<std::pair<std::string, std::string> > list;

    int count = 0;

    if (myfile.is_open())
    {
        while (getline(myfile, line))
        {
            list.push_back(std::make_pair(line.substr(0,8), line.substr(9,16)));
            count++;
        }
        myfile.close();
    }

    return list;

}


int main()
{
    bool normal = false;

    if (normal) {
        std::string text;
        std::string key;
        short batman;

        std::cout << "Enter 1 to encrypt. 2 to decrypt :";
        std::cin >> batman;

        switch (batman) {
            case 1:
                // encrypt
                std::cout << "Encrypt\n";
                std::cout << "Enter your plain text: ";
                std::cin >> text;

                std::cout << "Enter your key: ";
                std::cin >> key;

                FLDSMDFR(true, key, text, true);
                break;
            case 2:
                // decrypt
                std::cout << "Decrypt\n";
                std::cout << "Enter your cypher text: ";
                std::cin >> text;

                std::cout << "Enter your key: ";
                std::cin >> key;

                FLDSMDFR(false, key, text, true);
                break;
            default:
                std::cout << "wrong number.\n";
                break;
        }
    } else {
        std::vector<std::pair<std::string, std::string> > list = readFile();
        //std::vector<std::pair<std::string, std::string>> result = generateKeys(list);
        std::string result = generateUniqueKey(list);

//        for(auto x : result) {
//            std::cout << "Key : " << x.first << " | plain text : " << x.second << "\n";
//        }

    }

    return 0;
}