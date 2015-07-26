// g++ '/home/helsinki/Downloads/urltest.cpp' -lcurl -o '/home/helsinki/Downloads/urltest'

#include <iostream>
#include <string>
#include <fstream>
#include <set>
#include <curl/curl.h> //your directory may be different
using namespace std;


string data; //will hold the url's contents


size_t writeCallback(char* buf, size_t size, size_t nmemb, void* up)
   { //callback must have this declaration
    //buf is a pointer to the data that curl has for us
    //size*nmemb is the size of the buffer

   for (int c = 0; c<size*nmemb; c++)
      {
      data.push_back(buf[c]);
      }
          
   return size*nmemb; //tell curl how many bytes we handled
   }
   
void cURL_Fetch_Dump( ){
}

int main()
   {
   ofstream winrate("dataextract.txt");
   set<string> = NameChecker;
   ifstream input;
   int cnt3;
   input.open("input.txt");  // start input of data
   
   if(!input) { // file couldn't be opened
      cerr << "Error: file could not be opened" << endl;
      return 0;
      }
   while(!input.eof()){   //begin of datamining, line by line
      string names = "";    
      getline(input, names);   //line input
      if (names.substr(0, 1).compare("#") == 0){
         winrate << names << endl;
         continue;
         }
      int count1 = 0;  //Initial pos of word
      int count2 = 0;  //length of word
      for(int trav = 0; trav < names.length(); trav++){    //name splicer
         count2++;
         cnt3 = NameChecker.size();
         if (names.substr(trav, 1).compare(",") == 0){
            NameChecker.insert(names.substr(count1,count2-1));
            if (NameChecker.size() > cnt3){
            
               string URL = "http://wotinfo.net/en/efficiency?server=US&playername=+ ";
               URL = URL.insert(URL.length()-1, names.substr(count1,count2-1)); //URL splicer based on template and input data
               count1 = trav+2;
               count2 = -1;        
               CURL* curl; //our curl object
            
               curl_global_init(CURL_GLOBAL_ALL); //pretty obvious
               curl = curl_easy_init();
            
               curl_easy_setopt(curl, CURLOPT_URL, URL.c_str());
               curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, &writeCallback);
               //curl_easy_setopt(curl, CURLOPT_VERBOSE, 1L); //tell curl to output its progress
            
               curl_easy_perform(curl);
            
               //cout << endl << data << endl;    
             
               curl_easy_cleanup(curl);
               curl_global_cleanup();
            
               int count = 0;
               for ( int i = 0; i < data.length(); i++) // finding the winrate
                  {  
               
                  if (data.substr(i,8).compare("win rate") == 0)
                     count++;
                  if (count == 2){
                     cout << data.substr(i-33, 5) << endl;
                     winrate << data.substr(i-33, 5) << " ";
                     break;
                     }
               
                  
               }   
            for (int i = 0; i < data.length(); i++){  // finding WN8
               if (data.substr(i,3).compare("WN8") == 0){
                  while(data.substr(i, 5).compare("<var>") !=0){
                     i++;
                     }
                  i=i+16;
                  while(data.substr(i,1).compare("\n") !=0){
                     if (data.substr(i,1).compare(",") ==0){
                        i++;
                        continue;
                        }         
                     winrate << data.substr(i,1);
                     i++;
                     }
                  winrate << ", ";
                  break;
                  }
               }   
            data = string();  //clears data string cache for next run
            }
         winrate << "\n" << endl;  //finishes line
         
         }
      
   
      return 0;
      }

   
      