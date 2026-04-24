#include "spimcore.h"

/* ALU */
/* 10 Points */
void ALU(unsigned A,unsigned B,char ALUControl,unsigned *ALUresult,char *Zero)
{
  // ADD
  if (ALUControl == 0) { 
    // Get ALUResult
    *ALUresult = A + B;
  }
  
  //Subtract
  else if (ALUControl == 1) { 
    // Get ALUResult
    *ALUresult = A - B; 
  }
  
  // Set less than - signed
  else if (ALUControl == 2) { 
    // Get ALUResult
    if ((int)A < (int)B) {
      *ALUresult = 1;
    }
    else {
      *ALUresult = 0;
    }
  }

  // Set less than - unsigned
  else if (ALUControl == 3) { 
    // Get ALUResult
    if (A < B) {
      *ALUresult = 1;
    }
    else {
      *ALUresult = 0;
    }
  }

  // Perform And operation
  else if (ALUControl == 4) { 
    // Get ALUResult
    *ALUresult = A & B;
  }

  // Perform Or operation
  else if (ALUControl == 5) { 
    // Get ALUResult
    *ALUresult = A | B;
  }

  // Shift Left
  else if (ALUControl == 6) { 
    // Get ALUResult
    *ALUresult << 16;
  }

  // Not
  else if (ALUControl == 7) { 
    // Get ALUResult
    *ALUresult = ~A;
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
int instruction_fetch(unsigned PC,unsigned *Mem,unsigned *instruction) {
	if ((int)PC < 0/* || (PC >> 2) >= MEMSIZE*/)	return 1;	//address out of bounds
	*instruction = MEM(PC);
	return 0;
}

/* instruction partition */
/* 10 Points */
void instruction_partition(unsigned instruction, unsigned *op, unsigned *r1,unsigned *r2, unsigned *r3, unsigned *funct, unsigned *offset, unsigned *jsec) {
  *op = instruction >> 26;  //instruction [31-26]

  *jsec = (instruction << 6) >> 6;  //instruction [25-0]
  
  *r1 = (instruction << 6) >> 27;  //instruction [25-21]
  *r2 = (instruction << 11) >> 27;  //instruction [20-16]
  *r3 = (instruction << 16) >> 27;  //instruction [15-11]
	
  *funct = (instruction >> 26) << 26;  //instruction [5-0]
  *offset = (instruction << 16) >> 16;	//instruction [15-0]
}

/* instruction decode */
/* 15 Points */
int instruction_decode(unsigned op, struct_controls *controls)  {
	//set base controls to zero
	controls -> RegDst = 0;
    controls -> Jump = 0;
	controls -> Branch = 0;
    controls -> MemRead = 0;
    controls -> MemtoReg = 0;
    controls -> ALUOp = 0;
    controls -> MemWrite = 0;
    controls -> ALUSrc = 0;
    controls -> RegWrite = 0;

	//assign proper controls per opcode
	switch (op) {
		case 0:  //R-Type
			controls -> RegDst = 1;
	        controls -> ALUOp = 7;
    	    controls -> RegWrite = 1;
	        break;
		case 2:  //j  jump
			controls -> Jump = 1;
		    break;
		case 4:  //beq  branch on equal
		    controls -> Branch = 1;
		    break;
		case 8:  //addi  add immediate
		    controls -> ALUSrc = 1;
		    controls -> RegWrite = 1;
		    break;
		case 10:  //slti  set less than immediate
		    controls -> ALUOp = 2;
		    controls -> ALUSrc = 1;
		    controls -> RegWrite = 1;
		    break;
		case 11:  //sltiu  set less than immediate unsigned
		    controls -> ALUOp = 3;
		    controls -> ALUSrc = 1;
		    controls -> RegWrite = 1;
		    break;
		case 15:  //lui  load upper immediate
		    controls -> ALUSrc = 1;
		    controls -> RegWrite = 1;
		    break;
		case 35:  //lw  load word
		    controls -> MemRead = 1;
		    controls -> MemtoReg = 1;
		    controls -> ALUSrc = 1;
		    controls -> RegWrite = 1;
			break;
		case 43:  //sw  store word
		    controls -> MemWrite = 1;
		    controls -> ALUSrc = 1;
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
}

/* Sign Extend */
/* 10 Points */
void sign_extend(unsigned offset,unsigned *extended_value) {
	//if number is positive, no additional logic needed
	if (offset <= 0x8000) {
		*extended_value = offset;
	} else {	//if number is negative, add leading ones to retain value
		*extended_value = offset + 0xFFFF0000;
	}
}

/* ALU operations */
/* 10 Points */
int ALU_operations(unsigned data1,unsigned data2,unsigned extended_value,unsigned funct,char ALUOp,char ALUSrc,unsigned *ALUresult,char *Zero) {	
	if ( ALUOp < 0 || ALUOp > 7 )	return 1;	//I don't think that can happen though
	
	if ( !( ALUOp == 0 || funct == 0b0100000 || funct == 0b0100010 || funct == 0b0100100 || funct == 0b0100101 || funct == 0b0101010 || funct == 0b0101011 ) )
		return 1;	//invalid funct code 
   
	ALU( data1, ( !ALUSrc ) ? data2 : extended_value, ALUOp, ALUresult, Zero);
	return 0;
}

/* Read / Write Memory */
/* 10 Points */
int rw_memory(unsigned ALUresult,unsigned data2,char MemWrite,char MemRead,unsigned *memdata,unsigned *Mem) {
	if ((int)ALUresult < 0/* || (ALUresult >> 2) >= MEMSIZE*/)	return 1;	//address out of bounds
	
	if ( MemRead ) *memdata = MEM(ALUresult);	//load from memory
	
	if ( MemWrite ) MEM(ALUresult) = data2;	//store to memory
	return 0;
}

/* Write Register */
/* 10 Points */
void write_register(unsigned r2,unsigned r3,unsigned memdata,unsigned ALUresult,char RegWrite,char RegDst,char MemtoReg,unsigned *Reg) {
	if (RegWrite)
		Reg[ (RegDst) ? r3 : r2 ] = (MemtoReg) ? memdata : ALUresult;
}

/* PC update */
/* 10 Points */
void PC_update(unsigned jsec,unsigned extended_value,char Branch,char Jump,char Zero,unsigned *PC) {
	*PC += 4;
	if (Jump) {
		*PC = (*PC & 0xF0000000) + (jsec << 2);	//first 4 bits of pc, jsec, 2 bits of 0
	} else {
		if (Branch && Zero)
			*PC += (extended_value << 2);
	}
}

