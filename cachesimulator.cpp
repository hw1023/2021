/*
   Cache Simulator
   Level one L1 and level two L2 cache parameters are read from file
   (block size, line per set and set per cache).
   The 32 bit address is divided into:
   -tag bits (t)
   -set index bits (s)
   -block offset bits (b)

   s = log2(#sets)   b = log2(block size)  t=32-s-b
*/

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>
#include <iomanip>
#include <stdlib.h>
#include <cmath>
#include <bitset>
#include <map>

using namespace std;
//access state:
#define NA 0 // no action
#define RH 1 // read hit
#define RM 2 // read miss
#define WH 3 // write hit
#define WM 4 // write miss


struct config{
  int L1blocksize;
  int L1setsize;
  int L1size;
  int L2blocksize;
  int L2setsize;
  int L2size;
};

/* class cache */
class cache{
  config config_params;
  //tag_L1, blockOffset_L1, setIndexBit_L2, tag_L2, blockOffset_L2
  int setIndexBit_L1;
  int setIndexBit_L2;
  int tag_L1;
  int tag_L2;
  int blockOffset_L1;
  int blockOffset_L2;
  //two dimension dynamic array
  vector<vector<int> > cache_L1;
  vector<vector<int> > cache_L2;
  vector<vector<int> > validBit_L1;
  vector<vector<int> > validBit_L2;
  public :
      cache(config config_param){
          //get value from cacheconfig.txt
          config_params = config_param;  

          //b = log2(block size)
          blockOffset_L1 = log2(config_params.L1blocksize);  
          blockOffset_L2 = log2(config_params.L2blocksize);

          //s = log2(#sets)
          setIndexBit_L1 = log2((config_params.L1size)*(1024)/(config_params.L1blocksize*config_params.L1setsize)); 
          setIndexBit_L2 = log2((config_params.L2size)*(1024)/(config_params.L2blocksize*config_params.L2setsize));

          //t=32-s-b
          tag_L1 = 32 - setIndexBit_L1 - blockOffset_L1;  
          tag_L2 = 32 - setIndexBit_L2 - blockOffset_L2;

          //index of set
          int indexRow_L1 = pow(2, setIndexBit_L1);    
          int indexRow_L2 = pow(2, setIndexBit_L2);

          //valid bit of L1 cache
          validBit_L1.resize(config_params.L1setsize);  
          for(int i=0;i<config_params.L1setsize;i++){
              validBit_L1[i].resize(indexRow_L1);
          }

          //valid bit of L2 cache
          validBit_L2.resize(config_params.L2setsize);  
          for(int i=0;i<config_params.L2setsize;i++){
               validBit_L2[i].resize(indexRow_L2);
          }

          int blockSize_L1 = pow(2, blockOffset_L1);
          int blockSize_L2 = pow(2, blockOffset_L2);

          cache_L1 = createCache(config_params.L1setsize, indexRow_L1, blockSize_L1);
          cache_L2 = createCache(config_params.L2setsize, indexRow_L2, blockSize_L2);
      }

        

      vector<int> getBits(bitset<32> addr) {     //use string array to store address information of L1 of L2
          bitset<32> tempbits=0;
          vector<int> bits(4);

          for(int i=blockOffset_L1;i<setIndexBit_L1+blockOffset_L1;i++){    //store index of L1 in bits[0]
              if(addr[i]==1){
                  tempbits.set(i-blockOffset_L1);
              }
          }
          bits[0] = tempbits.to_ulong();
          tempbits=0;
          
          for(int i=setIndexBit_L1+blockOffset_L1;i<32;i++){                //store tag of L1 in bits[1]
              if(addr[i]==1){
                  tempbits.set(i-setIndexBit_L1-blockOffset_L1);
              }
          }
          bits[1] = tempbits.to_ulong();
          tempbits=0;

          for(int i=blockOffset_L2;i<setIndexBit_L2+blockOffset_L2;i++){    //store index of L2 in bits[2]
              if(addr[i]==1){
                  tempbits.set(i-blockOffset_L2);
              }
          }
          bits[2] = tempbits.to_ulong();
          tempbits=0;

          for(int i=setIndexBit_L2+blockOffset_L2;i<32;i++){                //store tag of L 2in bits[3]
              if(addr[i]==1){
                  tempbits.set(i-setIndexBit_L2-blockOffset_L2);
              }
          }
          bits[3] = tempbits.to_ulong();
          tempbits=0;

          return bits;
      }

      vector<vector<int> > createCache(int &way, int &indexRow, int &blockSize){    // Create empty Cache
          vector<vector<int> > myCache;
          myCache.resize(way);
          for(int i=0; i<way;i++) {
              myCache[i].resize(indexRow);

          }

          return myCache;
      }

    vector<vector<int> > getL1Cache(){  //function: get cache of L1
        return cache_L1;
    }

    vector<vector<int> > getL2Cache(){  //function: get cache of L2
        return cache_L2;
    }

    vector<vector<int> > getValidL1(){  //function: get valid bit of L1
        return validBit_L1;
    }

    vector<vector<int> > getValidL2(){  //function: get valid bit of L2
        return validBit_L2;
    }


};

map<int, bool> isWaysFull(vector<vector<int> > &L_cache, int way_L, int index_L){
    map<int, bool> cacheMap;
    for(int i=0;i<way_L;i++){
        cacheMap[i] = L_cache[i][index_L] != 0;
    }
    return cacheMap;
}

void loadTag(vector<vector<int> > &L_cache, vector<vector<int> > &L_validBit, int &way_L,int &wayNumber, int &index_L, int tag_L){
    L_cache[wayNumber][index_L] = tag_L;
    L_validBit[wayNumber][index_L] = 1;
}

int incCounter (int &way_L, int &wayNumber){    //The counter is incremented by 1 after an eviction.
    return (wayNumber+1)%way_L;
}

/* you can define the cache class here, or design your own data structure for L1 and L2 cache
   class cache {
       create two dimension array as a map to store data
       counter function
       check ways full function
   }
*/       

int main(int argc, char* argv[]){
    //cout<<string(argv[2]) + "argv[2]";
    //cout<<string(argv[1]) + "argv[1]";
    //argv[1]="cacheconfig.txt";
    //argv[2]="trace.txt";
    /*  for (int i = 0; i < argc; ++i){
            cout << argv[i] << "\n";
    }
    */
  config cacheconfig;
  ifstream cache_params;
  string dummyLine;
  cache_params.open(argv[1]);   //cache_params.open(argv[1]);"cacheconfig.txt"
  while(!cache_params.eof())    //read config file
  {
    cache_params>>dummyLine;
    cache_params>>cacheconfig.L1blocksize;
    cache_params>>cacheconfig.L1setsize;              
    cache_params>>cacheconfig.L1size;
    cache_params>>dummyLine;              
    cache_params>>cacheconfig.L2blocksize;           
    cache_params>>cacheconfig.L2setsize;        
    cache_params>>cacheconfig.L2size;
  }



  // Implement by you: 
  // initialize the hirearch cache system with those configs
  // probably you may define a Cache class for L1 and L2, or any data structure you like
  if(cacheconfig.L1setsize == 0){
      cacheconfig.L1setsize = cacheconfig.L1size*1024/cacheconfig.L1blocksize;  //direct map
  }
  if(cacheconfig.L2setsize == 0){
      cacheconfig.L2setsize = cacheconfig.L2size*1024/cacheconfig.L2blocksize;  //direct map
  }

  cache cacheObj(cacheconfig);

  vector<vector<int> > L1_cache = cacheObj.getL1Cache();
  vector<vector<int> > L2_cache = cacheObj.getL2Cache();
  vector<vector<int> > validBit_L1 = cacheObj.getValidL1();
  vector<vector<int> > validBit_L2 = cacheObj.getValidL2();

  int sizeL1Way = L1_cache.size();
  int sizeL2Way = L2_cache.size();
  int row_L1 = L1_cache[0].size();
  int row_L2 = L2_cache[0].size();

  int L1AcceState =0; // L1 access state variable, can be one of NA, RH, RM, WH, WM;
  int L2AcceState =0; // L2 access state variable, can be one of NA, RH, RM, WH, WM;

  ifstream traces;
  ofstream tracesout;
  string outname;
  outname = string(argv[2]) + ".out";//outname = string(argv[2]) + "traceout";"trace.txt.out"

  traces.open(argv[2]);     //traces.open(argv[2]);"trace.txt"
  tracesout.open(outname.c_str());

  string line;
  string accesstype;        // the Read/Write access type from the memory trace;
  string xaddr;             // the address from the memory trace store in hex;
  unsigned int addr;        // the address from the memory trace store in unsigned int;        
  bitset<32> accessaddr;    // the address from the memory trace store in the bitset;
  
  
  vector<int> counter_L1;
  vector<int> counter_L2;

  counter_L1.resize(row_L1);
  counter_L2.resize(row_L2);
  int count=0;  


  if (traces.is_open()&&tracesout.is_open()){    
    while (getline (traces,line)){   // read mem access file and access Cache

        istringstream iss(line); 
        if (!(iss >> accesstype >> xaddr)) {break;}
        stringstream saddr(xaddr);
        saddr >> std::hex >> addr;
        accessaddr = bitset<32> (addr);

        vector<int> bits = cacheObj.getBits(accessaddr);
        map<int, bool> isFull;
        int hit_L1=0;
        int hit_L2=0;
        int index_L1=bits[0];
        int tag_L1=bits[1];
        int index_L2=bits[2];
        int tag_L2=bits[3];
    
        //tag bit matching on L1
        for(int i=0; i<sizeL1Way; i++){               //check all ways
            if(L1_cache[i][index_L1] == tag_L1 && validBit_L1[i][index_L1]==1){
                hit_L1=1;                             //hit
                break;
            }else{                                    //miss
                hit_L1=0;
            }
        }

        //L1 miss goto L2
        if(hit_L1== 0){                               
            for(int i=0; i<sizeL2Way; i++){           //check all ways
                if(L2_cache[i][index_L2] == tag_L2){  //hit
                    hit_L2=1;
                    break;
                }else{                                //miss
                    hit_L2=0;
                }
            }
        }

        // access the L1 and L2 Cache according to the trace;
        if (accesstype.compare("R")==0)
        {    
        //Implement by you:
        // read access to the L1 Cache, 
        //  and then L2 (if required), 
        //  update the L1 and L2 access state variable;
            if(hit_L1==0){
                if(hit_L2==0){
                  
                L1AcceState = RM;
                L2AcceState = RM;

                //Goto main memory and fetch the data of that tag
                //update the L2 cache
                //checking which L2 Ways are empty
                isFull = isWaysFull(L2_cache, sizeL2Way, index_L2);

                //counter_L2 will be having the Way number in which we have to load the data
                //checking whether Way number provided by counter is empty or not
                
                for(int i=0;i<sizeL2Way;i++){
                  if(isFull[counter_L2[index_L2]]==false){
                      break;
                  }
                  counter_L2[index_L2] = (++counter_L2[index_L2])%sizeL2Way;
                }
                /*
                int temp=0;
                while(true){
                    if(isFull[counter_L2[index_L2]]){
                        break;
                    }else if(temp<sizeL2Way){
                        break;
                    }
                    counter_L2[index_L2] = (++counter_L2[index_L2])%sizeL2Way;
                    temp++;
                }*/
                // Finally loading Tag onto L2 cache
                loadTag(L2_cache, validBit_L2, sizeL2Way,counter_L2[index_L2], index_L2, tag_L2);
                counter_L2[index_L2] = incCounter(sizeL2Way, counter_L2[index_L2]);

                // update the L1 cache
                isFull = isWaysFull(L1_cache, sizeL1Way, index_L1);
                
                for(int i=0;i<sizeL1Way;i++){
                  if(isFull[counter_L1[index_L1]]==false){
                      break;
                  }
                  counter_L1[index_L1] = (++counter_L1[index_L1])%sizeL1Way;
                }

                loadTag(L1_cache, validBit_L1, sizeL1Way, counter_L1[index_L1], index_L1, tag_L1);
                counter_L1[index_L1] = incCounter(sizeL1Way, counter_L1[index_L1]);
                }else {   //hit_L1== 0, hit_L2==1
                L1AcceState = RM;
                L2AcceState = RH;
                /*when there is a read miss in L1 and read hit in L2, the L1 cache might have to evict some data to make room for the data returned by the L2 cache.
                If the evicted data is dirty, this will result in a write access to L2. If the write access to L2 results in a hit no data will be evicted/replaced.
                If it results in a miss, the data will be forwarded to main memory without changing the L2 cache state since the L2 cache is a write-no-allocate cache*/

                //put content of l2 into l1
                loadTag(L1_cache, validBit_L1, sizeL1Way, counter_L1[index_L1], index_L1, tag_L1);
                counter_L1[index_L1] = incCounter(sizeL1Way, counter_L1[index_L1]);
                }
            }else { //hit_L1 == 1
                L1AcceState = RH;
                L2AcceState = NA;
            }
            isFull = isWaysFull(L1_cache, sizeL1Way, index_L1);
        }
        else // Write
        {    
        //Implement by you:
        // write access to the L1 Cache, 
        //and then L2 (if required), 
        //update the L1 and L2 access state variable;
        if (hit_L1 == 0) {
            if(hit_L2==0){
                L1AcceState = WM;
                L2AcceState = WM;
            }else{                        //hit_L1=0, hit_L2=1
                L1AcceState = WM;         //Do not allocate any space on L1. Pass the write access to L2
                L2AcceState = WH;         //Goto L2 cache and update the data array with the new data on that tag
            }
        }else {                       //hit_L1 == 1
            L1AcceState = WH;
            L2AcceState = NA;         // update the data array on this tag at L1 
            }

        }

      // Output hit/miss results for L1 and L2 to the output file
      tracesout<< L1AcceState << " " << L2AcceState << endl;

    }
    traces.close();
    tracesout.close(); 
  }
  else cout<< "Unable to open trace or traceout file ";

  return 0;
}
