#include "spimcore.h"

/* ALU */ALU, instruction_decode, ALU_operations, rw_memory, instruction_fetch.
/* 10 Points */
void ALU(unsigned A,unsigned B,char ALUControl,unsigned *ALUresult,char *Zero) //Trevor
{
  // ADD
  if (ALUControl == 0) { 
    // Get ALUResult
    *ALUresult = A + B;
  }
  
  //Subtract
  if (ALUControl == 1) { 
    // Get ALUResult
    *ALUresult = A - B; 
  }
  
  // Set less than - signed
  if (ALUControl == 2) { 
    // Get ALUResult
    if ((int)A < (int)B) {
      *ALUresult = 1;
    }
    else {
      *ALUresult = 0;
    }
  }

  // Set less than - unsigned
  if (ALUControl == 3) { 
    // Get ALUResult
    if (A < B) {
      *ALUresult = 1;
    }
    else {
      *ALUresult = 0;
    }
  }

  // Perform And operation
  if (ALUControl == 4) { 
    // Get ALUResult
    *ALUresult = A & B;

  }

  // Perform Or operation
  if (ALUControl == 5) { 
    // Get ALUResult
    *ALUresult = A | B;
  }

  // Shift Left
  if (ALUControl == 6) { 
    // Get ALUResult
    *ALUresult << 16;

  }

  // Not
  if (ALUControl == 7) { 
    // Get ALUResult
    *ALUresult ~A;

  }

  // If ALUResult == 0 return Var ZERO = 1, if anything else var is zero
  if (*ALUresult == 0) {
      *Zero = 1;
  }
  else {
      *Zero = 0;
  }
}

/* instruction fetch */
/* 10 Points */
int instruction_fetch(unsigned PC,unsigned *Mem,unsigned *instruction) // Trevor
{

	return 0;
}

/* instruction partition */
/* 10 Points */
void instruction_partition(unsigned instruction, unsigned *op, unsigned *r1,unsigned *r2, unsigned *r3, unsigned *funct, unsigned *offset, unsigned *jsec) {
  //should be fully functional; see note about shamt
  *op = instruction >> 26;  //instruction [31-26]

  *jsec = (instruction << 6) >> 6;  //instruction [25-0]
  
  *r1 = (instruction << 6) >> 27;  //instruction [25-21]
  *r2 = (instruction << 11) >> 27;  //instruction [20-16]
  *r3 = (instruction << 16) >> 27;  //instruction [15-11]
      
  unsigned shamt = (instruction << 21) >> 27;  //instruction [10-6]    idek if we need this for this assignment, since it doesnt exists in the 
                                                                        //datapath signals in spimcore.c but i figured id add it and we can assign it to an actual value if needed
  *funct = (instruction >> 26) << 26;  //instruction [5-0]
  *offset = (instruction << 16) >> 16;
}

/* instruction decode */
/* 15 Points */
int instruction_decode(unsigned op, struct_controls *controls) //Trevor 
{
  switch op {
    case 0:  //R-Type
      controls -> RegDst = 1;
      controls -> Jump = 0;
      controls -> Branch = 0;
      controls -> MemRead = 0;
      controls -> MemToReg = 0;
      controls -> ALUOp = ;
      controls -> MemWrite = 0;
      controls -> ALUSrc = ;
      controls -> RegWrite = 1;
      break;
    case 2:  //j  jump
      controls -> RegDst = 0;
      controls -> Jump = 1;
      controls -> Branch = 0;
      controls -> MemRead = 0;
      controls -> MemToReg = 0;
      controls -> ALUOp = 0;
      controls -> MemWrite = 0;
      controls -> ALUSrc = 0;
      controls -> RegWrite = 0;
      break;
    case 4:  //beq  branch on equal
      controls -> RegDst = 0;
      controls -> Jump = ;
      controls -> Branch = 1;
      controls -> MemRead = 0;
      controls -> MemToReg = 0;
      controls -> ALUOp = ;
      controls -> MemWrite = ;
      controls -> ALUSrc = ;
      controls -> RegWrite = ;
      break;
    case 8:  //addi  add immediate
      controls -> RegDst = ;
      controls -> Jump = ;
      controls -> Branch = ;
      controls -> MemRead = ;
      controls -> MemToReg = ;
      controls -> ALUOp = ;
      controls -> MemWrite = ;
      controls -> ALUSrc = ;
      controls -> RegWrite = ;
      break;
    case 10:  //slti  set less than immediate
      controls -> RegDst = ;
      controls -> Jump = ;
      controls -> Branch = ;
      controls -> MemRead = ;
      controls -> MemToReg = ;
      controls -> ALUOp = ;
      controls -> MemWrite = ;
      controls -> ALUSrc = ;
      controls -> RegWrite = ;
      break;
    case 11:  //sltiu  set less than immediate upper
      controls -> RegDst = ;
      controls -> Jump = ;
      controls -> Branch = ;
      controls -> MemRead = ;
      controls -> MemToReg = ;
      controls -> ALUOp = ;
      controls -> MemWrite = ;
      controls -> ALUSrc = ;
      controls -> RegWrite = ;
      break;
    case 15:  //lui  load upper immediate
      controls -> RegDst = ;
      controls -> Jump = ;
      controls -> Branch = ;
      controls -> MemRead = ;
      controls -> MemToReg = ;
      controls -> ALUOp = ;
      controls -> MemWrite = ;
      controls -> ALUSrc = ;
      controls -> RegWrite = ;
      break;
    case 35:  //lw  load word
      controls -> RegDst = ;
      controls -> Jump = ;
      controls -> Branch = ;
      controls -> MemRead = ;
      controls -> MemToReg = ;
      controls -> ALUOp = ;
      controls -> MemWrite = ;
      controls -> ALUSrc = ;
      controls -> RegWrite = ;
      break;
    case 43:  //sw  store word
      controls -> RegDst = ;
      controls -> Jump = ;
      controls -> Branch = ;
      controls -> MemRead = ;
      controls -> MemToReg = ;
      controls -> ALUOp = ;
      controls -> MemWrite = ;
      controls -> ALUSrc = ;
      controls -> RegWrite = ;
      break;
    default:
      return 1;
  }
	return 0;
}

/* Read Register */
/* 5 Points */
void read_register(unsigned r1,unsigned r2,unsigned *Reg,unsigned *data1,unsigned *data2) {
  *data1 = Reg[r1];
  *data2 = Reg[r2];
  //that... feels too easy though....
}

/* Sign Extend */
/* 10 Points */
void sign_extend(unsigned offset,unsigned *extended_value) {
	if (offset <= 0b1000000000000000) {
		*extended_value = offset;
	} else {
		*extended_value = offset + 0xFFFF0000;
	}
}

/* ALU operations */
/* 10 Points */
int ALU_operations(unsigned data1,unsigned data2,unsigned extended_value,unsigned funct,char ALUOp,char ALUSrc,unsigned *ALUresult,char *Zero) // Trevor
{

	return 0;
}

/* Read / Write Memory */
/* 10 Points */
int rw_memory(unsigned ALUresult,unsigned data2,char MemWrite,char MemRead,unsigned *memdata,unsigned *Mem) // Trevor
{
  
	return 0;
}

/* Write Register */
/* 10 Points */
void write_register(unsigned r2,unsigned r3,unsigned memdata,unsigned ALUresult,char RegWrite,char RegDst,char MemtoReg,unsigned *Reg) {

}

/* PC update */
/* 10 Points */
void PC_update(unsigned jsec,unsigned extended_value,char Branch,char Jump,char
Zero,unsigned *PC) {

}

