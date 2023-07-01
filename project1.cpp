#ifndef __PROJECT1_CPP__
#define __PROJECT1_CPP__

#include "project1.h"
#include <vector>
#include <string>
#include <map>
#include <typeinfo>
#include <iostream>
#include <sstream>
#include <fstream>

std::map<std::string, int> labelsMap;      //this is the hashMap that will store all labels
std::map<std::string, int> staticMap; //this is the hashMap that will store all static memory labels

int main(int argc, char* argv[]) {
    if (argc < 4) // Checks that at least 3 arguments are given in command line
    {
        std::cerr << "Expected Usage:\n ./assemble infile1.asm infile2.asm ... infilek.asm staticmem_outfile.bin instructions_outfile.bin\n" << std::endl;
        exit(1);
    }
    //Prepare output files
    std::ofstream inst_outfile, static_outfile;
    static_outfile.open(argv[argc - 2], std::ios::binary);
    inst_outfile.open(argv[argc - 1], std::ios::binary);
    std::vector<std::string> instructions;
    std::vector<std::string> staticVector; // a vector  to store all static memory values
    
    //For each input file:
    int count2 = 0;       // this tracks the line numbers starting from 0
    int count1 = 0;       // this tracks the static memory line numbers starting from 0
    for (int i = 1; i < argc - 2; i++) {
        std::ifstream infile(argv[i]); //  open the input file for reading
        if (!infile) { // if file can't be opened, need to let the user know
            std::cerr << "Error: could not open file: " << argv[i] << std::endl;
            exit(1);
        }
        std::string str;
        int inGlobalMain = 0;      // this sees if we are in the .globl main
        while (getline(infile, str)){ //Read a line from the file
            str = clean(str); // remove comments and whitespace
            if (str == "") { //Ignore empty lines
                continue;
            }

            if (str == ".text"){ 
                inGlobalMain++;
                continue;
            }
            int notThere = -1; // 18446744073709551615;  
            if (inGlobalMain == 1 && str.find(".") == notThere) { // this is where we handle instructions and instruction labels
                if (str.find(":") != notThere) {        //-1 is the return value if " " is not found in str
                    int sep1 = str.find(":");
                    std::string str1 = str.substr(0,sep1);
                    labelsMap[str1] = count2;     //this adds the label and its line number to the labelsMap hashMap
                    // std::cout << labelsMap[str] << '\n';     // this prints out the positions of the labels in the hashMap
                }
                else{
                    instructions.push_back(str); // TODO This will need to change for labels
                    count2 ++;
                }
            }
            else { // for static memory labels
                if (str.find(": .") != notThere) { 
                    int sep2 = str.find(": .");
                    std::string str2 = str.substr(0,sep2);  // str2 is the static label

                    int bts = count(str.begin(), str.end(), ' ')-1;    // number of spaces between each number. 1 space by default after ': .word'
                    bts = bts*4;        // we calculate the number of bits by multiplying spaces by 4.

                    staticMap[str2] = count1;
                    count1 = count1 + bts;     // this is the count for number of bits used in static memory

                    // now we put static values in vector
                    std::string static_Val = str.substr(sep2+3);  // string_Values is the content after static label but it includes the 'asciiz' or 'word'
                    int sep3 = static_Val.find(" ");
                    std::string static_Value = static_Val.substr(sep3+1); // string_Value is the content after static label. No 'asciiz' or 'word'
                    std::vector<std::string> values_list = split(static_Value, " ");  // we split the values in each label and put in values_list vector
                    for (std::string stValue : values_list) {
                        staticVector.push_back(stValue);    // add each value to the staticVector vector
                    }
                }
            }
        }
        staticMap["__STATICMEMORYEND__"]=count1;
        infile.close();
    }

    /** Phase 2
     * Process all static memory, output to static memory file
     */
    int numVal = 0;
    for(std::string value1 : staticVector) {
        if (labelsMap.find(value1) == labelsMap.end()) {
            numVal = stoi(value1); //this converts the string into an integer
        }
        else {
            numVal = labelsMap[value1]*4; // get the line number of the function in bits
        }
        write_binary(numVal, static_outfile);
    }


    int instruction_index = 0; //instruction index
    //std::string inst = "-";
    //int sep = inst.find(" ");
   // write_binary(process_addi(inst.substr(sep+1)),inst_outfile);

    for(std::string inst : instructions) {
        int sep = inst.find(" ");
        std::string inst_type = inst.substr(0,sep);
        if (inst_type == "add") {
            write_binary(process_add(inst.substr(sep+1)), inst_outfile);
        }
        if (inst_type == "sub"){
            write_binary(process_sub(inst.substr(sep+1)),inst_outfile);
        }
        if (inst_type == "addi"){
            write_binary(process_addi(inst.substr(sep+1)),inst_outfile);
        }
        if (inst_type == "mult"){
            write_binary(process_mult(inst.substr(sep+1)),inst_outfile);
        }
        if (inst_type == "div"){
            write_binary(process_div(inst.substr(sep+1)),inst_outfile);
        }
         if (inst_type == "mflo"){
            write_binary(process_mflo(inst.substr(sep+1)),inst_outfile);
        }
         if (inst_type == "mfhi"){
            write_binary(process_mfhi(inst.substr(sep+1)),inst_outfile);
        }
          if (inst_type == "sll"){
            write_binary(process_sll(inst.substr(sep+1)),inst_outfile);
        }
         if (inst_type == "srl"){
            write_binary(process_srl(inst.substr(sep+1)),inst_outfile);
        }
         if (inst_type == "lw"){
            inst.pop_back();
            write_binary(process_lw(inst.substr(sep+1)),inst_outfile);
        }
         if (inst_type == "sw"){
            inst.pop_back();
            write_binary(process_sw(inst.substr(sep+1)),inst_outfile);
        }
          if (inst_type == "slt"){
            write_binary(process_slt(inst.substr(sep+1)),inst_outfile);
        }
        if (inst_type == "beq"){
            write_binary(process_beq((inst.substr(sep+1)),instruction_index),inst_outfile);
        }
         if (inst_type == "bne"){
            std::vector<std::string> reg_list1 = split(inst, WHITESPACE+",");
            write_binary(process_bne((inst.substr(sep+1)),instruction_index),inst_outfile);
        }
         if (inst_type == "j"){
            write_binary(process_j((inst.substr(sep+1))),inst_outfile);
        }
          if (inst_type == "jal"){
            write_binary(process_jal((inst.substr(sep+1))),inst_outfile);
        }
         if (inst_type == "jr"){
            write_binary(process_jr(inst.substr(sep+1)),inst_outfile);
        }
         if (inst_type == "jalr"){
            write_binary(process_jalr(inst.substr(sep+1)),inst_outfile);
        }
        if (inst_type == "la"){
            write_binary(process_la((inst.substr(sep+1))),inst_outfile);
        }
        if (inst_type == "syscall"){
            write_binary((26<<11)+12,inst_outfile);
        }

        //other instructions
        if (inst_type == "move"){
            write_binary(process_move((inst.substr(sep+1))),inst_outfile);
        }
        if (inst_type == "or"){
            write_binary(process_or((inst.substr(sep+1))),inst_outfile);
        }
        if (inst_type == "and"){
            write_binary(process_and((inst.substr(sep+1))),inst_outfile);
        }
        if (inst_type == "ori"){
            write_binary(process_ori((inst.substr(sep+1))),inst_outfile);
        }
        if (inst_type == "nor"){
            write_binary(process_nor((inst.substr(sep+1))),inst_outfile);
        }
        if (inst_type == "xor"){
            write_binary(process_xor((inst.substr(sep+1))),inst_outfile);
        }
        if (inst_type == "lui"){
            write_binary(process_lui((inst.substr(sep+1))),inst_outfile);
        }
        if (inst_type == "xori"){
            write_binary(process_xori((inst.substr(sep+1))),inst_outfile);
        }
        if (inst_type == "andi"){
            write_binary(process_andi((inst.substr(sep+1))),inst_outfile);
        }
        if (inst_type == "not"){
            write_binary(process_not((inst.substr(sep+1))),inst_outfile);
        }
        if (inst_type == "sltu"){
            write_binary(process_sltu((inst.substr(sep+1))),inst_outfile);
        }
        //other pseudo-instructions

         if (inst_type == "sgt"){
            write_binary(process_sgt((inst.substr(sep+1))),inst_outfile);
        }
         if (inst_type == "sge"){
            write_binary(process_sge((inst.substr(sep+1))),inst_outfile);
        }
         if (inst_type == "sle"){
            write_binary(process_sle((inst.substr(sep+1))),inst_outfile);
        }
         if (inst_type == "seq"){
            write_binary(process_seq((inst.substr(sep+1))),inst_outfile);
        }
         if (inst_type == "sne"){
            write_binary(process_sne((inst.substr(sep+1))),inst_outfile);
        }
         if (inst_type == "mov"){
            write_binary(process_move((inst.substr(sep+1))),inst_outfile);
        }
         if (inst_type == "abs"){
            write_binary(process_abs((inst.substr(sep+1))),inst_outfile);
        }
         if (inst_type == "not"){
            write_binary(process_not((inst.substr(sep+1))),inst_outfile);
        }
         if (inst_type == "not"){
            write_binary(process_not((inst.substr(sep+1))),inst_outfile);
        }
         if (inst_type == "not"){
            write_binary(process_not((inst.substr(sep+1))),inst_outfile);
        }
         if (inst_type == "not"){
            write_binary(process_not((inst.substr(sep+1))),inst_outfile);
        }
         if (inst_type == "not"){
            write_binary(process_not((inst.substr(sep+1))),inst_outfile);
        }
         if (inst_type == "not"){
            write_binary(process_not((inst.substr(sep+1))),inst_outfile);
        }

        
        instruction_index++;
    }
}

//method for add instructions
int process_add(const std::string &inst) { //opcode = 0, funccode = 32
    std::vector<std::string> reg_list = split(inst, WHITESPACE+",");
    return process_Rtype(0, registers[reg_list[1]], registers[reg_list[2]], registers[reg_list[0]], 0, 32); 
}
//method to handle sub instructions
int process_sub(const std::string &inst) { //opcode = 0, funccode = 34
    std::vector<std::string> reg_list = split(inst, WHITESPACE+",");
    return process_Rtype(0, registers[reg_list[1]], registers[reg_list[2]], registers[reg_list[0]], 0, 34); 
}
int process_addi(const std::string &inst) { //opcode = 8
    std::vector<std::string> reg_list = split(inst, WHITESPACE+",");
    int imm = stoi(reg_list[2]);
    if (imm < 0){
        imm += 65536;
    }
    return process_Itype(8, registers[reg_list[1]], registers[reg_list[0]],imm); 
}
int process_mult(const std::string &inst) { //opcode = 0, funccode = 24
    std::vector<std::string> reg_list = split(inst, WHITESPACE+",");
    return process_Rtype(0, registers[reg_list[0]], registers[reg_list[1]],0, 0, 24); 
}
int process_div(const std::string &inst) { //opcode = 0, funccode = 26
    std::vector<std::string> reg_list = split(inst, WHITESPACE+",");
    return process_Rtype(0, registers[reg_list[0]], registers[reg_list[1]],0, 0, 26); 
}
int process_mflo(const std::string &inst) { //opcode = 0, funccode = 18
    std::vector<std::string> reg_list = split(inst, WHITESPACE+",");
    return process_Rtype(0,0,0,registers[reg_list[0]], 0, 18); 
}
int process_mfhi(const std::string &inst) { //opcode = 0, funccode = 16
    std::vector<std::string> reg_list = split(inst, WHITESPACE+",");
    return process_Rtype(0,0,0,registers[reg_list[0]], 0, 16); 
}
int process_sll(const std::string &inst) { //opcode = 0, funccode = 0
    std::vector<std::string> reg_list = split(inst, WHITESPACE+",");
    return process_Rtype(0, 0,registers[reg_list[1]], registers[reg_list[0]],stoi(reg_list[2]),0); 
}
int process_srl(const std::string &inst) { //opcode = 0, funccode = 0
    std::vector<std::string> reg_list = split(inst, WHITESPACE+",");
    return process_Rtype(0, 0,registers[reg_list[1]], registers[reg_list[0]],stoi(reg_list[2]),2); 
}
int process_lw(const std::string &inst) {
    std::vector<std::string> reg_list = split(inst, WHITESPACE+",(");
    return process_Itype(35     ,registers[reg_list[2]], registers[reg_list[0]],stoi(reg_list[1])); 
}
int process_sw(const std::string &inst) {
    std::vector<std::string> reg_list = split(inst, WHITESPACE+",(");
    return process_Itype(43,registers[reg_list[2]], registers[reg_list[0]],stoi(reg_list[1])); 
}
int process_slt(const std::string &inst) { //opcode = 0, funccode = 42
    std::vector<std::string> reg_list = split(inst, WHITESPACE+",");
    return process_Rtype(0,registers[reg_list[1]], registers[reg_list[2]], registers[reg_list[0]], 0, 42); 
}
int process_beq(const std::string &inst,int label_index) { //opcode = 4, funccode = 4
    std::vector<std::string> reg_list = split(inst, WHITESPACE+",");
    int position = labelsMap[reg_list[2]] - label_index -1;
    if (position < 0){
        position += 65536;
    }
    return process_Itype(4,registers[reg_list[0]], registers[reg_list[1]],position); 
}
int process_bne(const std::string &inst,int label_index) { 
    std::vector<std::string> reg_list = split(inst, WHITESPACE+",");
    int position = labelsMap[reg_list[2]] - label_index -1;
    if (position < 0){
        position += 65536;
    }
    return process_Itype(5, registers[reg_list[0]], registers[reg_list[1]],position); 
}
int process_j(const std::string &inst) { //opcode = 2
    std::vector<std::string> reg_list = split(inst, WHITESPACE+",");
    return process_Jtype(2,labelsMap[reg_list[0]]); 
}
int process_jal(const std::string &inst) { //opcode = 2
    std::vector<std::string> reg_list = split(inst, WHITESPACE+",");
    return process_Jtype(3,labelsMap[reg_list[0]]); 
}
int process_jr(const std::string &inst) { //opcode = 0, funccode = 8
    std::vector<std::string> reg_list = split(inst, WHITESPACE+",");
    return process_Rtype(0,registers[reg_list[0]],0,0,0,8); 
}
int process_jalr(const std::string &inst) { //opcode = 0, funccode = 8
    std::vector<std::string> reg_list = split(inst, WHITESPACE+",");
    return process_Rtype(0,registers[reg_list[0]],0,registers["$ra"],0,9); 
}
int process_la(const std::string &inst) { 
    std::vector<std::string> reg_list = split(inst, WHITESPACE+",");
    return process_Itype(8,0,registers[reg_list[0]],staticMap[reg_list[1]]); 
}

//other bonus instructions
int process_move(const std::string &inst){
    std::vector<std::string> reg_list = split(inst, WHITESPACE+",");
    return process_Rtype(0,registers[reg_list[1]],0,registers[reg_list[0]],0,36);
}
int process_or(const std::string &inst){
    std::vector<std::string> reg_list = split(inst, WHITESPACE+",");
    return process_Rtype(0,registers[reg_list[1]],registers[reg_list[2]],registers[reg_list[2]],0,37);
}
int process_and(const std::string &inst){
    std::vector<std::string> reg_list = split(inst, WHITESPACE+",");
    return process_Rtype(0,registers[reg_list[1]],registers[reg_list[2]],registers[reg_list[0]],0,36);
}
int process_ori(const std::string &inst){
    std::vector<std::string> reg_list = split(inst, WHITESPACE+",");
    int position  = registers[reg_list[2]];
    if (position < 0){
        position += 65536;
    }
    return process_Itype(13,registers[reg_list[1]],registers[reg_list[0]],position);
}
int process_nor(const std::string &inst){
    std::vector<std::string> reg_list = split(inst, WHITESPACE+",");
    return process_Rtype(0,registers[reg_list[1]],registers[reg_list[2]],registers[reg_list[0]],0,39);
}
int process_xor(const std::string &inst){
    std::vector<std::string> reg_list = split(inst, WHITESPACE+",");
    return process_Rtype(0,registers[reg_list[1]],registers[reg_list[2]],registers[reg_list[0]],0,38);
}
int process_lui(const std::string &inst){
    std::vector<std::string> reg_list = split(inst, WHITESPACE+",");
    int position  = registers[reg_list[1]];
    if (position < 0){
        position += 65536;
    }
    return process_Itype(15,0,registers[reg_list[0]],position);
}
int process_xori(const std::string &inst){
    std::vector<std::string> reg_list = split(inst, WHITESPACE+",");
    int position  = registers[reg_list[2]];
    if (position < 0){
        position += 65536;
    }
    return process_Itype(14,registers[reg_list[1]],registers[reg_list[0]],position);
}
int process_andi(const std::string &inst){
    std::vector<std::string> reg_list = split(inst, WHITESPACE+",");
    int position  = registers[reg_list[2]];
    if (position < 0){
        position += 65536;
    }
    return process_Itype(12,registers[reg_list[1]],registers[reg_list[0]],position);
}
int process_not(const std::string &inst){
    std::vector<std::string> reg_list = split(inst, WHITESPACE+",");
    return process_Rtype(0,registers[reg_list[1]],registers[reg_list[1]],registers[reg_list[0]],0,39);
}
int process_sltu(const std::string &inst){
    std::vector<std::string> reg_list = split(inst, WHITESPACE+",");
    return process_Rtype(0,registers[reg_list[1]],registers[reg_list[2]],registers[reg_list[0]],0,43);
}

//other pseudo-instructions
int process_sgt(const std::string &inst) { //opcode = 0, funccode = 42
    std::vector<std::string> reg_list = split(inst, WHITESPACE+",");
    return process_Rtype(0, registers[reg_list[2]], registers[reg_list[1]], registers[reg_list[0]], 0, 42); 
}
int process_sge(const std::string &inst) { //opcode = 0, funccode => behaves like sltu but signed
    std::vector<std::string> reg_list = split(inst, WHITESPACE+",");
    return process_Rtype(0, registers[reg_list[1]], registers[reg_list[2]], registers[reg_list[0]], 0, 43); 
}
int process_sle(const std::string &inst) { //opcode = 0, funccode = 58
    std::vector<std::string> reg_list = split(inst, WHITESPACE+",");
    return process_Rtype(0, registers[reg_list[2]], registers[reg_list[1]], registers[reg_list[0]], 0, 43); 
}

int process_seq(const std::string &inst) { //opcode = 0, funccode = 42
    std::vector<std::string> reg_list = split(inst, WHITESPACE+",");
    return process_Rtype(0, registers[reg_list[1]], registers[reg_list[2]], registers[reg_list[0]], 0, 50); 
}

int process_sne(const std::string &inst) { //opcode = 0, funccode = 54
    std::vector<std::string> reg_list = split(inst, WHITESPACE+",");
    return process_Rtype(0, registers[reg_list[1]], registers[reg_list[2]], registers[reg_list[0]], 0, 54); 
}

int process_abs(const std::string &inst) { //opcode = 0, funccode = 42
    std::vector<std::string> reg_list = split(inst, WHITESPACE+",");
    return process_Rtype(0,0, registers[reg_list[1]], registers[reg_list[0]], 0, 5); 
}

//helper functions
//check number of bits


/*

*/

// handling all types
int process_Rtype(int opcode, int rs, int rt, int rd, int shftamt, int funccode) {
    return (opcode << 26) + (rs << 21) + (rt << 16) + (rd << 11) + (shftamt << 6) + funccode;
}
int process_Itype(int opcode, int rs, int rt, int imm) {
    return (opcode << 26) + (rs << 21) + (rt << 16)+ imm;
}
int process_Jtype(int opcode, int addr){
    return (opcode << 26) + (addr);
}
#endif