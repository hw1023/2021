#include<iostream>
#include<string>
#include<vector>
#include<bitset>
#include<fstream>
using namespace std;
#define MemSize 1000 // memory size, in reality, the memory size should be 2^32, but for this lab, for the space resaon, we keep it as this large number, but the memory is still 32-bit addressable.

struct IFStruct {
    bitset<32>  PC;
    bool        nop;  
};

struct IDStruct {
    bitset<32>  Instr;
    bool        nop;  
};

struct EXStruct {
    bitset<32>  Read_data1;
    bitset<32>  Read_data2;
    bitset<16>  Imm;
    bitset<5>   Rs;
    bitset<5>   Rt;
    bitset<5>   Wrt_reg_addr;
    bool        is_I_type;
    bool        rd_mem;
    bool        wrt_mem; 
    bool        alu_op;     //1 for addu, lw, sw, 0 for subu 
    bool        wrt_enable;
    bool        nop;  
};

struct MEMStruct {
    bitset<32>  ALUresult;
    bitset<32>  Store_data;
    bitset<5>   Rs;
    bitset<5>   Rt;    
    bitset<5>   Wrt_reg_addr;
    bool        rd_mem;
    bool        wrt_mem; 
    bool        wrt_enable;    
    bool        nop;    
};

struct WBStruct {
    bitset<32>  Wrt_data;
    bitset<5>   Rs;
    bitset<5>   Rt;     
    bitset<5>   Wrt_reg_addr;
    bool        wrt_enable;
    bool        nop;     
};

struct stateStruct {
    IFStruct    IF;
    IDStruct    ID;
    EXStruct    EX;
    MEMStruct   MEM;
    WBStruct    WB;
};

class RF
{
    public: 
        bitset<32> Reg_data;
     	RF()
    	{ 
			Registers.resize(32);  
			Registers[0] = bitset<32> (0);  
        }
	
        bitset<32> readRF(bitset<5> Reg_addr)
        {   
            Reg_data = Registers[Reg_addr.to_ulong()];
            return Reg_data;
        }
    
        void writeRF(bitset<5> Reg_addr, bitset<32> Wrt_reg_data)
        {
            Registers[Reg_addr.to_ulong()] = Wrt_reg_data;
        }
		 
		void outputRF()
		{
			ofstream rfout;
			rfout.open("RFresult.txt",std::ios_base::app);
			if (rfout.is_open())
			{
				rfout<<"State of RF:\t"<<endl;
				for (int j = 0; j<32; j++)
				{        
					rfout << Registers[j]<<endl;
				}
			}
			else cout<<"Unable to open file";
			rfout.close();               
		} 
			
	private:
		vector<bitset<32> >Registers;	
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
                  
		bitset<32> readInstr(bitset<32> ReadAddress) 
		{    
			string insmem;
			insmem.append(IMem[ReadAddress.to_ulong()].to_string());
			insmem.append(IMem[ReadAddress.to_ulong()+1].to_string());
			insmem.append(IMem[ReadAddress.to_ulong()+2].to_string());
			insmem.append(IMem[ReadAddress.to_ulong()+3].to_string());
			Instruction = bitset<32>(insmem);		//read instruction memory
			return Instruction;     
		}     
      
    private:
        vector<bitset<8> > IMem;     
};
      
class DataMem    
{
    public:
        bitset<32> ReadData;  
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
		
        bitset<32> readDataMem(bitset<32> Address)
        {	
			string datamem;
            datamem.append(DMem[Address.to_ulong()].to_string());
            datamem.append(DMem[Address.to_ulong()+1].to_string());
            datamem.append(DMem[Address.to_ulong()+2].to_string());
            datamem.append(DMem[Address.to_ulong()+3].to_string());
            ReadData = bitset<32>(datamem);		//read data memory
            return ReadData;               
		}
            
        void writeDataMem(bitset<32> Address, bitset<32> WriteData)            
        {
            DMem[Address.to_ulong()] = bitset<8>(WriteData.to_string().substr(0,8));
            DMem[Address.to_ulong()+1] = bitset<8>(WriteData.to_string().substr(8,8));
            DMem[Address.to_ulong()+2] = bitset<8>(WriteData.to_string().substr(16,8));
            DMem[Address.to_ulong()+3] = bitset<8>(WriteData.to_string().substr(24,8));  
        }   
                     
        void outputDataMem()
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

void printState(stateStruct state, int cycle)
{
    ofstream printstate;
    printstate.open("stateresult.txt", std::ios_base::app);
    if (printstate.is_open())
    {
        printstate<<"State after executing cycle:\t"<<cycle<<endl; 
        
        printstate<<"IF.PC:\t"<<state.IF.PC.to_ulong()<<endl;        
        printstate<<"IF.nop:\t"<<state.IF.nop<<endl; 
        
        printstate<<"ID.Instr:\t"<<state.ID.Instr<<endl; 
        printstate<<"ID.nop:\t"<<state.ID.nop<<endl;
        
        printstate<<"EX.Read_data1:\t"<<state.EX.Read_data1<<endl;
        printstate<<"EX.Read_data2:\t"<<state.EX.Read_data2<<endl;
        printstate<<"EX.Imm:\t"<<state.EX.Imm<<endl; 
        printstate<<"EX.Rs:\t"<<state.EX.Rs<<endl;
        printstate<<"EX.Rt:\t"<<state.EX.Rt<<endl;
        printstate<<"EX.Wrt_reg_addr:\t"<<state.EX.Wrt_reg_addr<<endl;
        printstate<<"EX.is_I_type:\t"<<state.EX.is_I_type<<endl; 
        printstate<<"EX.rd_mem:\t"<<state.EX.rd_mem<<endl;
        printstate<<"EX.wrt_mem:\t"<<state.EX.wrt_mem<<endl;        
        printstate<<"EX.alu_op:\t"<<state.EX.alu_op<<endl;
        printstate<<"EX.wrt_enable:\t"<<state.EX.wrt_enable<<endl;
        printstate<<"EX.nop:\t"<<state.EX.nop<<endl;        

        printstate<<"MEM.ALUresult:\t"<<state.MEM.ALUresult<<endl;
        printstate<<"MEM.Store_data:\t"<<state.MEM.Store_data<<endl; 
        printstate<<"MEM.Rs:\t"<<state.MEM.Rs<<endl;
        printstate<<"MEM.Rt:\t"<<state.MEM.Rt<<endl;   
        printstate<<"MEM.Wrt_reg_addr:\t"<<state.MEM.Wrt_reg_addr<<endl;              
        printstate<<"MEM.rd_mem:\t"<<state.MEM.rd_mem<<endl;
        printstate<<"MEM.wrt_mem:\t"<<state.MEM.wrt_mem<<endl; 
        printstate<<"MEM.wrt_enable:\t"<<state.MEM.wrt_enable<<endl;         
        printstate<<"MEM.nop:\t"<<state.MEM.nop<<endl;        

        printstate<<"WB.Wrt_data:\t"<<state.WB.Wrt_data<<endl;
        printstate<<"WB.Rs:\t"<<state.WB.Rs<<endl;
        printstate<<"WB.Rt:\t"<<state.WB.Rt<<endl;        
        printstate<<"WB.Wrt_reg_addr:\t"<<state.WB.Wrt_reg_addr<<endl;
        printstate<<"WB.wrt_enable:\t"<<state.WB.wrt_enable<<endl;        
        printstate<<"WB.nop:\t"<<state.WB.nop<<endl; 
    }
    else cout<<"Unable to open file";
    printstate.close();
}
 

int main()
{
    
    RF myRF;
    INSMem myInsMem;
    DataMem myDataMem;
	stateStruct state;
	

	int cycle = 0;
	//int stall = 0;
	state.IF.nop = 0;
	state.ID.nop = 1;
	state.EX.nop = 1;
	state.MEM.nop = 1;
	state.WB.nop = 1;




			
             
    while (1) {
		stateStruct newState;
		bitset<64> buffer;
		bitset<32> signextend;
		bitset<32> branchaddress;
		newState.EX.alu_op = 0;
		newState.EX.is_I_type = 0;
		newState.EX.rd_mem = 0;
		newState.EX.wrt_enable = 0;
		newState.EX.wrt_mem = 0;
		newState.EX.Imm=0;

		newState.MEM.rd_mem = 0;
		newState.MEM.wrt_enable = 0;
		newState.MEM.wrt_mem = 0;

		newState.WB.wrt_enable = 0;
		int branch=0;
		
        /* --------------------- WB stage --------------------- */
		if (state.WB.nop == 0) {
			
			if (state.WB.wrt_enable == 1) {
				myRF.writeRF(state.WB.Wrt_reg_addr, state.WB.Wrt_data);
			}
			//newState.WB.Wrt_data
		}


        /* --------------------- MEM stage --------------------- */
		if (state.MEM.nop == 0) {
			//myDataMem.MemoryAccess(myALU.ALUresult, myRF.ReadData2, readmem, writemem);
			if (state.MEM.rd_mem == 1) {//Load
				newState.WB.Wrt_data = myDataMem.readDataMem(state.MEM.ALUresult).to_ulong();
			}
			else {
				newState.WB.Wrt_data = state.MEM.ALUresult.to_ulong();
			}
			if (state.MEM.wrt_mem == 1) {//Store
				myDataMem.writeDataMem(state.MEM.ALUresult, state.MEM.Store_data);
			}
			newState.WB.Rs = state.MEM.Rs.to_ulong();
			newState.WB.Rt = state.MEM.Rt.to_ulong();
			newState.WB.wrt_enable = state.MEM.wrt_enable;
			newState.WB.Wrt_reg_addr = state.MEM.Wrt_reg_addr.to_ulong();
			
			newState.WB.nop = 0;
		}
		else {
			newState.WB.nop = 1;
			newState.IF.PC = state.IF.PC.to_ulong();
		}


        /* --------------------- EX stage --------------------- */
		if (state.EX.nop == 0) {
			if (state.EX.is_I_type == 1) {//I-type
				if (state.EX.alu_op == 1) {// add
					if (state.EX.rd_mem == 1 || state.EX.wrt_mem == 1) {//Load or Store
						newState.MEM.ALUresult = state.EX.Read_data1.to_ulong() + state.EX.Imm.to_ulong();
					}
					else
					{
						newState.MEM.ALUresult = state.EX.Read_data1.to_ulong() + state.EX.Read_data2.to_ulong();
					}
				}
				else {//sub
					newState.MEM.ALUresult = state.EX.Read_data1.to_ulong() - state.EX.Read_data2.to_ulong();
				}
			}
			else {//R-type
				if (state.EX.alu_op == 1) {// add
					newState.MEM.ALUresult = state.EX.Read_data1.to_ulong() + state.EX.Read_data2.to_ulong();
				}
				else {//sub
					newState.MEM.ALUresult = state.EX.Read_data1.to_ulong() - state.EX.Read_data2.to_ulong();
				}
			}
			newState.MEM.rd_mem = state.EX.rd_mem;
			newState.MEM.wrt_mem = state.EX.wrt_mem;
			newState.MEM.wrt_enable = state.EX.wrt_enable;
			newState.MEM.Wrt_reg_addr = state.EX.Wrt_reg_addr.to_ulong();
			newState.MEM.Rs = state.EX.Rs.to_ulong();
			newState.MEM.Rt = state.EX.Rt.to_ulong();
			if(newState.MEM.wrt_mem==1){
				newState.MEM.Store_data = state.EX.Read_data2.to_ulong();
			}else{
				newState.MEM.Store_data = 0;
			}

			newState.MEM.nop = 0;
		}
		else {
			newState.MEM.nop = 1;
			newState.IF.PC = state.IF.PC.to_ulong();
		}
          

        /* --------------------- ID stage --------------------- */
		if (state.ID.nop == 0) {
			bitset <6> opcode(0);
			bitset <3> ALUOP(0);

			//if (state.ID.Instr.to_ulong() == 4294967295) {
			//	newState.ID.nop = 1;
			//	newState.IF.nop = 1;
			//}
			
			for (int i = 0; i < 6; i++) {
				if (state.ID.Instr[26 + i] == 1) {
					opcode.set(i);
				}
				//opcode[i] = Instruction[26 + i];
			}

			if ((opcode.to_ulong() == 43) || (opcode.to_ulong() == 35)) {//IsLoad? = (Inst[31:26] == 100011)||IsStore? = (Inst[31:26] == 101011)
				newState.EX.alu_op = 1;
				//ALUOP = 1;										//ALUop = 001 Add
			}
			else if (opcode.to_ulong() == 0) {						//Is it R-Type?
				for (int j = 0; j < 3; j++) {
					if (state.ID.Instr[j] == 1) {
						ALUOP.set(j);
					}
					//ALUOP[j] = Instruction[j];
				}
			}
			else {
				for (int j = 0; j < 3; j++) {
					if (state.ID.Instr[26 + j] == 1) {
						ALUOP.set(j);
					}
					//ALUOP[j] = Instruction[26 + j];
				}
			}



			if (opcode.to_ulong() == 0) {												// R-type
				newState.EX.is_I_type = 0;
				for (int j = 0; j < 5; j++) {
					if (state.ID.Instr[21 + j] == 1) {
						newState.EX.Rs.set(j);
						//RdReg1.set(j);
					}
				}
				for (int j = 0; j < 5; j++) {
					if (state.ID.Instr[16 + j] == 1) {
						newState.EX.Rt.set(j);
						//RdReg2.set(j);
					}
				}
				for (int j = 0; j < 5; j++) {
					if (state.ID.Instr[11 + j] == 1) {
						newState.EX.Wrt_reg_addr.set(j);
						//WrtReg.set(j);
					}
				}
				
			}
			else if ((opcode.to_ulong() != 0) && (opcode.to_ulong() != 2) && (opcode.to_ulong() != 3))//I-Type
			{	
				newState.EX.is_I_type = 1;
				for (int j = 0; j < 5; j++) {
					if (state.ID.Instr[21 + j] == 1) {
						newState.EX.Rs.set(j);
						//RdReg1.set(j);
					}
					if (state.ID.Instr[16 + j] == 1) {
						newState.EX.Rt.set(j);
						newState.EX.Wrt_reg_addr.set(j);
						//WrtReg.set(j);
						//RdReg2.set(j);
					}
				}
				for (int j = 0; j < 16; j++) {
					if (state.ID.Instr[j] == 1) {
						newState.EX.Imm.set(j);
						//EXimm.set(j);														//immediate
					}
				}
				if (newState.EX.Imm[15] == 1) {														//overflow
					for (int j = 18; j < 32; j++) {
						signextend.set(j);
						//EXimm.set(j);														//sign extended
					}
				}

				if ((opcode.to_ulong() == 4) && (myRF.readRF(newState.EX.Rt) != myRF.readRF(newState.EX.Rs))) {// Is Branch? Assuming not taken

					printf("\nIt is branch\n");
					cout<<newState.EX.Rs<<'\n';
					cout<<newState.EX.Rt<<'\n';
					cout<<"signextend:"<<signextend<<'\n';
					cout<<"signextend:"<<signextend.to_ulong()<<'\n';
					cout<<"imm:"<<newState.EX.Imm<<'\n';
					cout<<"imm:"<<newState.EX.Imm.to_ulong()<<'\n';
					cout<<"PC:"<<state.IF.PC.to_ulong()<<'\n';
					buffer = state.IF.PC.to_ulong()  + 4*newState.EX.Imm.to_ulong() + signextend.to_ulong();
					cout<<"Buffer:"<<buffer<<'\n';
					branchaddress=0;
					for(int i=0;i<32;i++){
						if(buffer[i]==1){
							branchaddress.set(i);
						}

					}
					cout<<"Branch address:"<<branchaddress<<'\n';
					//newState.IF.PC = state.IF.PC.to_ulong() + 4 + newState.EX.Imm.to_ulong();
					branch=1;
					//INSAddress = INSAddress.to_ulong() + 4 + 4 * EXimm.to_ulong();
					//continue;
				}

			}
			

			if (opcode.to_ulong() == 35) {//IsLoad?
				//readmem = 1;
				newState.EX.rd_mem = 1;
			}
			else
			{
				//readmem = 0;
				newState.EX.rd_mem = 0;
			}
			if (opcode.to_ulong() == 43) {//IsStore?
				//writemem = 1;
				newState.EX.wrt_mem = 1;
			}
			else {
				//writemem = 0;
				newState.EX.wrt_mem = 0;
			}




			if ((opcode.to_ulong() == 43) || (opcode.to_ulong() == 2) || (opcode.to_ulong() == 4)) {//(IsStore? | IsBranch? | J-Type?)
				//WrtEnable = 0;
				newState.EX.wrt_enable = 0;
			}
			else {
				//WrtEnable = 1;
				newState.EX.wrt_enable = 1;
			}


			/*if (opcode.to_ulong() == 35) {									//IsLoad?
				myRF.ReadWrite(RdReg1, RdReg2, WrtReg, myDataMem.readdata, WrtEnable);
			}
			else {
				myRF.ReadWrite(RdReg1, RdReg2, WrtReg, myALU.ALUresult, WrtEnable);
			}*/

			if (ALUOP.to_ulong() == 1) {	//ALUOP="001"  addu/addiu
				newState.EX.alu_op = 1;
			}
			else if (ALUOP.to_ulong() == 3) {//ALUOP="011" subu/lw/sw
				newState.EX.alu_op = 0;
			}
			
			newState.EX.Read_data1 = myRF.readRF(newState.EX.Rs);
			newState.EX.Read_data2 = myRF.readRF(newState.EX.Rt);
			
			
			
			newState.EX.nop = 0;
		}
		else {
			newState.EX.nop = 1;
			newState.IF.PC = state.IF.PC.to_ulong();
		}
		
        
        /* --------------------- IF stage --------------------- */
		if (state.IF.nop == 0) {
			newState.ID.Instr = myInsMem.readInstr(state.IF.PC);
			if (newState.ID.Instr.to_ulong() != 4294967295) {
				newState.IF.PC = state.IF.PC.to_ulong() + 4;
				newState.IF.nop = 0;
				newState.ID.nop = 0;
			}
			else if (newState.ID.Instr.to_ulong() == 4294967295) {
				newState.ID.nop = 1;
				newState.IF.nop = 1;
				newState.IF.PC = state.IF.PC.to_ulong();
			}
			
			
		}
		else if(state.IF.nop == 1)
		{	
			newState.ID.Instr = myInsMem.readInstr(state.IF.PC);
			newState.IF.PC = state.IF.PC.to_ulong();
		}
		
		/* --------------------- Branch Solution --------------------- */
		if (branch==1) {// Is Branch? Assuming not taken
			newState.ID.nop=1;
			newState.IF.nop=0;
			newState.IF.PC = branchaddress.to_ulong();
			newState.EX.alu_op = 0;
			newState.EX.is_I_type = 0;
			newState.EX.rd_mem = 0;
			newState.EX.wrt_enable = 0;
			newState.EX.wrt_mem = 0;
			newState.EX.Imm=0;
			newState.EX.Read_data1=0;
			newState.EX.Read_data2=0;
			newState.EX.Wrt_reg_addr=0;
			newState.EX.Rs=0;
			newState.EX.Rt=0;
		}

		/* --------------------- Harzard & Solution --------------------- */
		
		if(newState.MEM.rd_mem==0){				//addu or sub or store 	EX-EX forwarding
			if(newState.MEM.Wrt_reg_addr==newState.EX.Rs){
				newState.EX.Read_data1=newState.MEM.ALUresult;
			}
			if(newState.MEM.Wrt_reg_addr==newState.EX.Rt){
				newState.EX.Read_data2=newState.MEM.ALUresult;
			}

		}

		if(newState.WB.Wrt_reg_addr==newState.EX.Rs){					//MEM-EX forwarding
			newState.EX.Read_data1=newState.WB.Wrt_data;
			}
		if(newState.WB.Wrt_reg_addr==newState.EX.Rt){
			newState.EX.Read_data2=newState.WB.Wrt_data;
			}

		if(newState.MEM.rd_mem==1){										//I-type Load before 
			printf("It is a stall\n");
			if(newState.MEM.Wrt_reg_addr==newState.EX.Rs){
				newState.EX.nop=1;		
				newState.ID.nop=0;
				newState.IF.nop=0;								//Stall
				newState.ID.Instr =0;
				newState.IF.PC = state.IF.PC.to_ulong();
				newState.ID.Instr = state.ID.Instr;
			}
			if(newState.MEM.Wrt_reg_addr==newState.EX.Rt){
				newState.EX.nop=1;
				newState.ID.nop=0;
				newState.IF.nop=0;
				newState.ID.Instr =0;
				newState.IF.PC = state.IF.PC.to_ulong();
				newState.ID.Instr = state.ID.Instr;
			}
		}
		/* --------------------- end --------------------- */
        if (state.IF.nop && state.ID.nop && state.EX.nop && state.MEM.nop && state.WB.nop)
            break;
        
        printState(newState, cycle); //print states after executing cycle 0, cycle 1, cycle 2 ... 
       
        state = newState; /*The end of the cycle and updates the current state with the values calculated in this cycle */ 
		
		cycle = cycle + 1;
    }
    
    myRF.outputRF(); // dump RF;	
	myDataMem.outputDataMem(); // dump data mem 
	
	return 0;
}