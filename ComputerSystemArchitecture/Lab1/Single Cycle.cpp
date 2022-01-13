#include<iostream>
#include<string>
#include<vector>
#include<bitset>
#include<fstream>

using namespace std;

#define ADDU 1
#define SUBU 3
#define AND 4
#define OR  5
#define NOR 7

// Memory size.
// In reality, the memory size should be 2^32, but for this lab and space reasons,
// we keep it as this large number, but the memory is still 32-bit addressable.
#define MemSize 65536


class RF
{
  public:
    bitset<32> ReadData1, ReadData2; 
    RF()
    { 
      Registers.resize(32);  
      Registers[0] = bitset<32> (0); 

    }

    void ReadWrite(bitset<5> RdReg1, bitset<5> RdReg2, bitset<5> WrtReg, bitset<32> WrtData, bitset<1> WrtEnable)
	{
		if (WrtEnable.to_ulong() == 1) {
			Registers[WrtReg.to_ulong()] = 0;
			for (int i = 0; i < 32; i++) {
				if (WrtData[i] == 1) {
					Registers[WrtReg.to_ulong()].set(i);
				}
			}
			//Registers[WrtReg.to_ulong()] = WrtData.to_ulong();
		}
		ReadData1 = Registers[RdReg1.to_ulong()];
		ReadData2 = Registers[RdReg2.to_ulong()];
      // TODO: implement!               
    }

    void OutputRF()
    {
      ofstream rfout;
      rfout.open("RFresult.txt",std::ios_base::app);
      if (rfout.is_open())
      {
        rfout<<"A state of RF:"<<endl;
        for (int j = 0; j<32; j++)
        {        
          rfout << Registers[j] << endl;
        }

      }
      else cout<<"Unable to open file";
      rfout.close();

    }     
  private:
    vector<bitset<32> >Registers;
};


class ALU
{
  public:
    bitset<32> ALUresult;
    bitset<32> ALUOperation (bitset<3> ALUOP, bitset<32> oprand1, bitset<32> oprand2)
    {   
		if (ALUOP.to_ulong() == 1) {	//ALUOP="001"  addu/addiu
			ALUresult = oprand1.to_ulong()+oprand2.to_ulong();
		}
		else if (ALUOP.to_ulong() == 2) {//ALUOP="010" jump
			return 0;
		}
		else if (ALUOP.to_ulong() == 3) {//ALUOP="011" subu/lw/sw
			ALUresult = oprand1.to_ulong() - oprand2.to_ulong();
		}
		else if (ALUOP.to_ulong() == 4) {//ALUOP="100" and/beq
			ALUresult = oprand1 & oprand2;
		}
		else if (ALUOP.to_ulong() == 5) {//ALUOP="101" or
			ALUresult = oprand1 | oprand2;
		}
		else if (ALUOP.to_ulong() == 7) {//ALUOP="111"  nor/halt
			ALUresult = ~(oprand1 | oprand2);
		}
      // TODO: implement!
      return ALUresult;
    }            
};


class INSMem
{
  public:
    bitset<32> Instruction;
    INSMem()
    {      
      IMem.resize(MemSize); 
      ifstream imem;
      string line;
      int i=0;
      imem.open("imem.txt");
      if (imem.is_open())
      {
        while (getline(imem,line))
        {      
          IMem[i] = bitset<8>(line);
          i++;
        }

      }
      else cout<<"Unable to open file";
      imem.close();

    }

    bitset<32> ReadMemory (bitset<32> ReadAddress) 
	{	
		Instruction = 0;
		if (ReadAddress.to_ulong() % 4 == 0) {//bits1.to_ulong()
			for (int j = 0; j < 8; j++) {//Big Endian
				if (IMem[ReadAddress.to_ulong()][j] == 1) {
					Instruction.set(j + 24);
				}
			}
			for (int j = 0; j < 8; j++) {
				if (IMem[ReadAddress.to_ulong() + 1][j] == 1) {
					Instruction.set(j + 16);
				}
			}
			for (int j = 0; j < 8; j++) {
				if (IMem[ReadAddress.to_ulong() + 2][j] == 1) {
					Instruction.set(j + 8);
				}
			}
			for (int j = 0; j < 8; j++) {
				if (IMem[ReadAddress.to_ulong() + 3][j] == 1) {
					Instruction.set(j);
				}
			}
		}
      // TODO: implement!
      // (Read the byte at the ReadAddress and the following three byte).
      //
      return Instruction;     
    }     

  private:
    vector<bitset<8> > IMem;

};

class DataMem    
{
  public:
    bitset<32> readdata;  
    DataMem()
    {
      DMem.resize(MemSize); 
      ifstream dmem;
      string line;
      int i=0;
      dmem.open("dmem.txt");
      if (dmem.is_open())
      {
        while (getline(dmem,line))
        {      
          DMem[i] = bitset<8>(line);
          i++;
        }
      }
      else cout<<"Unable to open file";
      dmem.close();

    }  
    bitset<32> MemoryAccess (bitset<32> Address, bitset<32> WriteData, bitset<1> readmem, bitset<1> writemem) 
	{
		if (writemem == 1) {
			DMem[Address.to_ulong()] = 0;
			DMem[Address.to_ulong()+1] = 0;
			DMem[Address.to_ulong()+2] = 0;
			DMem[Address.to_ulong()+3] = 0;
			for (int i = 0; i < 8; i++) {
				if (WriteData[24 + i] == 1) {
					DMem[Address.to_ulong()].set(i);
				}
			}
			for (int i = 0; i < 8; i++) {
				if (WriteData[16 + i] == 1) {
					DMem[Address.to_ulong() + 1].set(i);
				}
			}
			for (int i = 0; i < 8; i++) {
				if (WriteData[8 + i] == 1) {
					DMem[Address.to_ulong() + 2].set(i);
				}
			}
			for (int i = 0; i < 8; i++) {
				if (WriteData[i] == 1) {
					DMem[Address.to_ulong() + 3].set(i);
				}
			}
		}
		if (readmem == 1) {
			readdata = (0);
			for (int i = 0; i < 8; i++) {
				if (DMem[Address.to_ulong()][i]==1) {
					readdata.set(i + 24);
				}
			}
			for (int i = 0; i < 8; i++) {
				if (DMem[Address.to_ulong()+1][i] == 1) {
					readdata.set(i + 16);
				}
			}
			for (int i = 0; i < 8; i++) {
				if (DMem[Address.to_ulong()+2][i] == 1) {
					readdata.set(i + 8);
				}
			}
			for (int i = 0; i < 8; i++) {
				if (DMem[Address.to_ulong()+3][i] == 1) {
					readdata.set(i);
				}
			}
			//return readdata;
		}
		//else
		//{
		//	return 0;
		//}
      // TODO: implement!
      return readdata;     
    }   

    void OutputDataMem()
    {
      ofstream dmemout;
      dmemout.open("dmemresult.txt");
      if (dmemout.is_open())
      {
        for (int j = 0; j< 1000; j++)
        {     
          dmemout << DMem[j]<<endl;
        }

      }
      else cout<<"Unable to open file";
      dmemout.close();

    }             

  private:
    vector<bitset<8> > DMem;

};  



int main()
{
  RF myRF;
  ALU myALU;
  INSMem myInsMem;
  DataMem myDataMem;
  bitset <32> INSAddress(0);
  int stage = 1;
  
  while (1)
  {
	  bitset <1> WrtEnable(0);
	  bitset <1> readmem(0);
	  bitset <1> writemem(0);
	  bitset <3> ALUOP(0);
	  bitset <5> WrtReg(0);
	  bitset <5> RdReg1(0);
	  bitset <5> RdReg2(0);
	  bitset <6> opcode(0);
	  bitset <32> EXimm(0);
	  bitset <32> Instruction(0);

	  

	  



	  Instruction=myInsMem.ReadMemory(INSAddress);

	  if (Instruction.to_ulong() == 4294967295) {
		  break;
	  }
		
	  //decode
	  for (int i = 0; i < 6; i++) {
		  if (Instruction[26 + i] == 1) {
			  opcode.set(i);
		  }
		  //opcode[i] = Instruction[26 + i];
	  }

	  if ((opcode.to_ulong() == 43)||(opcode.to_ulong() ==35)) {//IsLoad? = (Inst[31:26] == 100011)||IsStore? = (Inst[31:26] == 101011)
		  ALUOP = 1;										//ALUop = 001 Add
	  }
	  else if (opcode.to_ulong() ==0) {						//Is it R-Type?
		  for (int j = 0; j < 3; j++) {
			  if (Instruction[j] == 1) {
				  ALUOP.set(j);
			  }
			  //ALUOP[j] = Instruction[j];
		  }
	  }
	  else {
		  for (int j = 0; j < 3; j++) {
			  if (Instruction[26 + j] == 1) {
				  ALUOP.set(j);
			  }
			  //ALUOP[j] = Instruction[26 + j];
		  }
	  }
	  

	  
	  if (opcode.to_ulong() == 0) {												// R-type
		  for (int j = 0; j < 5; j++) {
			  if (Instruction[21 + j] == 1) {
				  RdReg1.set(j);
			  }
		  }
		  for (int j = 0; j < 5; j++) {
			  if (Instruction[16 + j] == 1) {
				  RdReg2.set(j);
			  }
		  }
		  for (int j = 0; j < 5; j++) {
			  if (Instruction[11 + j] == 1) {
				  WrtReg.set(j);
			  }
		  }
		  INSAddress = INSAddress.to_ulong() + 4;
	  }
	  else if ((opcode.to_ulong()!=0)&&(opcode.to_ulong()!=2)&&(opcode.to_ulong()!=3))//I-Type
	  {
		  for (int j = 0; j < 5; j++) {
			  if (Instruction[21 + j] == 1) {
				  RdReg1.set(j);
			  }
			  if (Instruction[16 + j] == 1) {
				  WrtReg.set(j);
				  RdReg2.set(j);
			  }
		  }
		  for (int j = 0; j < 16; j++) {
			  if (Instruction[j] == 1) {
				  EXimm.set(j);														//immediate
			  }
		  }
		  if (EXimm[15] == 1) {														//overflow
			  for (int j = 16; j < 32; j++) {
				  EXimm.set(j);														//sign extended
			  }
		  }

		  if ((opcode.to_ulong() == 4) && (myRF.ReadData1.to_ulong() == myRF.ReadData2.to_ulong())) {// Is Branch?
			 
			  printf("\nIt is branch\n");
			  INSAddress = INSAddress.to_ulong() + 4 + 4*EXimm.to_ulong();
			  //continue;
		  }
		  else
		  {
			  INSAddress = INSAddress.to_ulong() + 4;
		  }
		  
	  }
	  else {																		//J-Type
		  bitset<32> JumpAddress(0);
		  INSAddress = INSAddress.to_ulong() + 4;
		  for (int j = 0; j < 32; j++) {
			  if ((j < 26)&&(Instruction[j]==1)) {									//JumpAddress = { (PC+4)[31:28], address, 2��b0 }
				  JumpAddress.set(j+2);
			  }
			  else if((j>27)&&(INSAddress[j]==1)){
				  JumpAddress.set(j);
			  }
		  }
		  printf("\nIt is Jump\n");
		  INSAddress = JumpAddress.to_ulong();
		  //continue;
	  }

	  myRF.ReadWrite(RdReg1,RdReg2,WrtReg,0,0);
	  if ((opcode.to_ulong() != 0) && (opcode.to_ulong() != 2) && (opcode.to_ulong() != 3)) { //I-Type//Load or Store (opcode.to_ulong() == 35)|| (opcode.to_ulong() == 43)
		  myALU.ALUOperation(ALUOP, myRF.ReadData1, EXimm);
	  }
	  else {
		  myALU.ALUOperation(ALUOP, myRF.ReadData1, myRF.ReadData2);
	  }

	  if (opcode.to_ulong() == 35) {//IsLoad?
		  readmem = 1;
	  }
	  else
	  {
		  readmem = 0;
	  }
	  if (opcode.to_ulong() == 43) {//IsStore?
		  writemem = 1;
	  }
	  else {
		  writemem = 0;
	  }





	  myDataMem.MemoryAccess(myALU.ALUresult,myRF.ReadData2,readmem,writemem);

	  cout << "STAGE" << stage << '\n';

	  cout << "myALU" << '\n';
	  cout << myALU.ALUresult << "||"<<myALU.ALUresult.to_ulong()<<'\n';

	  cout << "myRF" << '\n';
	  cout << myRF.ReadData1 << "||" << myRF.ReadData1.to_ulong()<< '\n';
	  cout << myRF.ReadData2 << "||" << myRF.ReadData2.to_ulong() << '\n';

	  cout << "readdata" << '\n';
	  cout << myDataMem.readdata << "||" << myDataMem.readdata.to_ulong() << '\n';

	  cout << "WrtReg" << '\n';
	  cout << WrtReg << "||" << WrtReg.to_ulong() << '\n';

	  cout << "RdReg2" << '\n';
	  cout << RdReg2 << "||" << RdReg2.to_ulong() << '\n';

	  cout << "RdReg1" << '\n';
	  cout << RdReg1 << "||" << RdReg1.to_ulong() << '\n';
	  cout << '\n';
	  cout << '\n';

	  



	  if ((opcode.to_ulong() == 43) || (opcode.to_ulong() == 2) || (opcode.to_ulong() == 4)) {//(IsStore? | IsBranch? | J-Type?)
		  WrtEnable = 0;
	  }
	  else {
		  WrtEnable = 1;
	  }


	  if (opcode.to_ulong() == 35) {									//IsLoad?
		  myRF.ReadWrite(RdReg1, RdReg2, WrtReg, myDataMem.readdata,WrtEnable);
	  }
	  else {
		  myRF.ReadWrite(RdReg1, RdReg2, WrtReg, myALU.ALUresult, WrtEnable);
	  }
	  stage = stage + 1;


    // Fetch

    // If current insturciton is "11111111111111111111111111111111", then break;

    // decode(Read RF)

    // Execute

    // Read/Write Mem

    // Write back to RF

    myRF.OutputRF(); // dump RF;    
  }
  myDataMem.OutputDataMem(); // dump data mem

  return 0;
}
