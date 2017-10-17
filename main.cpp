#include <string>
#include <iostream>
#include <unistd.h>
#include <vector>
#include <chrono>
#include "string.h"

using namespace std;
using namespace std::chrono;
char *progName;

class SimpleAnagramMatcher {
    private:
        vector<string> words;
        void trimTrailing(char * str) {
            int index, i;
            i = 0;
            while(str[i] != '\0') {
                if(str[i] != ' ' && str[i] != '\t' && str[i] != '\n') {
                    index= i;
                }
                i++;
            }
            str[index + 1] = '\0';
        }

        bool checkAnagram(string s1, string s2) {
            for (char i : s1) {
                size_t pos = s2.find(i,0);
                if (pos != string::npos) {
                    s2.erase(pos,1);
                } else {
                    return false;
                }
            }
            return s2.empty();
        }


    public:

        SimpleAnagramMatcher(const char* filename) {
            FILE* fp = fopen(filename,"r");
            if (fp == NULL) {
                printf("Can't open file \n");
                return;
            }
            int cnt = 0;
            char buff[1024];
            // get the next word from dictionary file.
            while (fgets(buff, 1024, fp)) 
            {
                cnt++;
                trimTrailing(buff);
                words.push_back(buff);
            }
            fclose(fp);
        }


        string findFirstAngaramWord(string word) {
            string res = "";
            for (int i=0; i<words.size(); i++) {
                if (checkAnagram(word,words[i])) {
                    res = words[i];
                    break;
                }
            }
            return res;
        }

};


void printUsage(const char* error) {
    if (error) {
        printf("ERROR: %s \n", error);
    }
    printf("Usage: %s <dictionary> <word> \n", progName);
    printf("\t Required: <dictionary> file containing all words in dictionary \n");
    printf("\t Required: <word> word to find the matching anagram(s) from dictionary \n");
}

bool checkFile(char* filename) {
    return (access( filename, F_OK ) != -1 );
}

string RandomString(int len)
{
   srand(time(0));
   string str = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz";
   string newstr;
   int pos;
   while(newstr.size() != len) {
    pos = ((rand() % (str.size() - 1)));
    newstr += str.substr(pos,1);
   }
   return newstr;
}


int main(int argc, char* argv[])
{
    progName = argv[0];

    if (argc < 3) {
        printUsage(NULL);
        exit(1);
    } else if (!checkFile(argv[1])) {
        printUsage("given dictionary file does not exist");
    }
    const char *fileName = argv[1];
    const char *word = argv[2];

    SimpleAnagramMatcher simpleMatcher(fileName);
    //dict.dumpHashTable();

    for (int i=0; i< 20; i++) {
        string mystr = RandomString(10);

        high_resolution_clock::time_point t1 = high_resolution_clock::now();
        string matchWord = simpleMatcher.findFirstAngaramWord(mystr);
        high_resolution_clock::time_point t2 = high_resolution_clock::now();
        auto duration = duration_cast<microseconds>( t2 - t1 ).count();

        if (matchWord.length() > 0) {
            cout << "First Found match: " << matchWord  << " for " << word <<  " took " << duration << " microseconds" << endl;
        } else {
            cout << "No match for: " << mystr << " took " << duration << " microseconds" << endl;
        }
    }

    return 0;
}
