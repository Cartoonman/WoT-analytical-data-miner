// g++ '/home/helsinki/Downloads/urltest.cpp' -lcurl -o '/home/helsinki/Downloads/urltest'
#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <cmath>
#include <curl/curl.h> //your directory may be different
//#include "boost/python.hpp"  //BROKEN BLAME LIBBOOST NOT BUILDING
using namespace std;

string data; //will hold the url's contents
/*
char const* greet()
{
   return "hello, world";
}

BOOST_PYTHON_MODULE(hello_ext)
{
	using namespace boost::python;
    def("greet", greet);
}
*/
size_t writeCallback(char* buf, size_t size, size_t nmemb, void* up)
   { //callback must have this declaration
    //buf is a pointer to the data that curl has for us
    //size*nmemb is the size of the buffer

   for (unsigned int c = 0; c<size*nmemb; c++)
      {
      data.push_back(buf[c]);
      }
          
   return size*nmemb; //tell curl how many bytes we handled
   }
   

void cURL_Fetch_Dump( ){
}

class PlayerStat {
	string winRATE, wn8;
	public:
	void set_values (string,string);
	string fetch() {return (winRATE + " " + wn8 + ", ");} 
	};
	

		
void PlayerStat::set_values (string a, string b){
	winRATE = a;
	wn8 = b;
	}

	

//Initialized Vars

   ifstream input;
   vector<string> NameChecker; // Checks for redundancy.
   vector<PlayerStat> v;  //Vector for Redundancy check
   PlayerStat player; //Object for Redundancy check
   PlayerStat fetcher; //Object for Fetching data in Redundancy Check
   
   
   int index, countWr, count1, count2, datCnt, x;
   int bcInd = 0;
   double* arrWN = new double[1000]; //TEMP FIX FOR HEAP NOT LOADING ENOUGH MEMORY
   double* arrWR = new double[1000];
   string names, wr, wn, URL, numStat;
   bool idMatch, insuffBatt;
   double avgWR, avgWN;
   std::string::size_type sz;
	

int main()
   {  
   
   input.open("C:\\Users\\colena64\\Desktop\\File\\input.txt");  // start input of data
   ofstream extractedData("C:\\Users\\colena64\\Desktop\\File\\dataextract.txt");
   
   if(!input) { // file couldn't be opened
      cerr << "Error: file could not be opened\ninput.txt missing" << endl;
      return 0;
      } 

   while(!input.eof()){   //begin of datamining, line by line
      names = "";    
      getline(input, names);   //line input
      if (names.substr(0, 1).compare("#") == 0){
         extractedData << names << endl;
         continue;
         }
      count1 = 0;  //Initial pos of word
      count2 = 0;  //length of word
	  
      for(unsigned int trav = 0; trav < names.length(); trav++){    //name splicer
         count2++;
         if (names.substr(trav, 1).compare(",") == 0){
			idMatch = false;
			for (int i = 0; i < NameChecker.size(); i++){
				if(names.substr(count1,count2-1).compare(NameChecker.at(i)) == 0) {
					index = i;
					idMatch = true; 
					break;
					}
				}
            if (!idMatch){		
			   wn = "";
			   wr = "";
               URL = "http://www.wotinfo.net/en/efficiency?server=US&playername=+ ";
               URL = URL.insert(URL.length()-1, names.substr(count1,count2-1)); //URL splicer based on template and input data   
			   
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
			   
			   insuffBatt = false;
			   for (unsigned int i = 0; i < data.length(); i++){ // Battle Counter
					if(data.substr(i,7).compare("battles") == 0){
						for(int j = i; j > 0; j--){
							if(data.substr(j,1).compare("x")==0){
								j+=4;
								i=j;
								while(data.substr(j,1).compare("(") != 0){
									j++;
									}
								if( atoi(data.substr(i, j-i).c_str()) <= 200){
									insuffBatt = true;
									}
								break;
								}
							}
						break;
						}
					}
			   if (!insuffBatt){	
               countWr = 0;
               for ( int i = 0; i < data.length(); i++) // finding the extractedData
                   {  
               
                  if (data.substr(i,8).compare("win rate") == 0){
                     countWr++;
					 if (countWr == 2){
						cout << data.substr(i-33, 5) << endl;  // This gets the WR as a whole string
						extractedData << data.substr(i-33, 5) << " ";
						wr = data.substr(i-33, 5);
						break;
						}
					}
               
                  
				} 	
				for (int i = 0; i < data.length(); i++) // finding WN8
					{  
					
					if (data.substr(i,3).compare("WN8") == 0){
						while(data.substr(i, 5).compare("<var>") !=0){
							i++;
						}
					i=i+16;
					while(data.substr(i,1).compare("\n") !=0){  // This gets the WN8 data char by char, excluding the ",".
						if (data.substr(i,1).compare(",") ==0){
							i++;
							continue;
						}         
						extractedData << data.substr(i,1);
						wn = wn.insert(wn.length(), data.substr(i,1)); // placing wn8 into wn string for redundancy check
						i++;
						}
					extractedData << ", ";
					break;
					}
				}
			   if(wr!="" || wn!=""){								
					player.set_values(wr,wn);  // puts wr and wn values into object player
					v.push_back(player);      // puts values into obj vector
					NameChecker.push_back(names.substr(count1,count2-1)); // Puts name into vector for reduncancy checker
					}
				}
			   data = string();  //clears data string cache for next run
			   count1 = trav+2;
               count2 = -1;  
			   }
			   		 
			else{  // Redundancy Check implementation
			    count1 = trav+2;
                count2 = -1;   
				fetcher = v.at(index);
				extractedData << fetcher.fetch();
			}
        }
    
    }
	extractedData << "\n" << endl;  //finishes line
}
 
   extractedData.close();
   input.close();
   
   input.open("C:\\Users\\colena64\\Desktop\\File\\dataextract.txt");
   ofstream dataAnalysis("C:\\Users\\colena64\\Desktop\\File\\dataanalysis.txt");
   
   while(!input.eof()){
		names = "";
		getline(input, names);   //line input
		if (names.substr(0, 1).compare("#") == 0 || names.substr(0, 1) == "\0"){
         dataAnalysis << names << endl;
         continue;
         }
		 datCnt = 0;
		 x = 0;
		 for(int i=0; i <=names.size(); i++){
			if(names.substr(i,1).compare(",") == 0){
			datCnt++;
			}
		}
		count1 = 0;
		count2 = 0;
		
		arrWN[datCnt];
		arrWR[datCnt];
		for(int i=0; i<=names.size(); i++){
			if(names.substr(i,1).compare(",") != 0){
				count2++;
				continue;
				}
			numStat = names.substr(count1, count2);
			count1=i+2;
			count2=-1;
			arrWR[x] = stod(numStat,&sz);
			cout << arrWR[x] << endl;
			arrWN[x] = stod(numStat.substr(sz));
			cout << arrWR[x] << endl;
			x++;
		}
		
		//Count of Votes
		
		
		//Average Calculator
		avgWR = 0.0;
		avgWN = 0.0;
		for(int i = 0; i < datCnt; i++){
			avgWR += arrWR[i];
			avgWN += arrWN[i];
		}
		dataAnalysis << "Average Winrate: " << avgWR/(double)datCnt << endl;
		dataAnalysis << "Average WN8: " << avgWN/(double)datCnt << endl;
		
		double E = 0;
		for(unsigned int i = 0;i<datCnt;i++)
			E += pow((double)arrWR[i] - (avgWR/(double)datCnt), 2);
		
		dataAnalysis << "Winrate Standard Deviation: " << sqrt(E/(double)datCnt) << endl;
		
		E = 0;
		for(unsigned int i = 0;i<datCnt;i++)
			E += pow((double)arrWR[i] - (avgWN/(double)datCnt), 2);
			
			
		dataAnalysis << "WN8 Standard Deviation: " << sqrt(E/(double)datCnt) << endl;
			
			
			
			
	}
			
			

   
return 0;
 }

   
      