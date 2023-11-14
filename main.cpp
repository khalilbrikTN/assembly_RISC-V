#include <iostream>
#include <vector>
#include <unordered_map>
#include <bitset>
#include <fstream>
#include <sstream>

using namespace std;

unordered_map<string, string> varToAddressMap, addressToValueMap;
vector<string> instructions;
bitset<32> zero(0), ra(0), sp, gp, tp(0), t0(0), t1(0), t2(0), s0(0), s1(0), a0(0), a1(0), a2(0), a3(0), a4(0), a5(0), a6(0), a7(0), s2(0), s3(0), s4(0), s5(0), s6(0), s7(0), s8(0), s9(0), s10(0), s11(0), t3(0), t4(0), t5(0), t6(0);
int programCounter, index = 0;

string binaryFromDecimal(int num){
    string binaryRepresentation = "";
    while (num > 0) {
        binaryRepresentation = to_string(num % 2) + binaryRepresentation;
        num /= 2;
    }
    return binaryRepresentation;
}

string hexadecimalFromDecimal(int num){
    string hexRepresentation = "";
    int remainder;
    while (num > 0) {
        remainder = num % 16;
        if (remainder < 10) {
            hexRepresentation = to_string(remainder) + hexRepresentation;
        } else {
            hexRepresentation = static_cast<char>(remainder - 10 + 'A') + hexRepresentation;
        }
        num /= 16;
    }
    if (hexRepresentation.empty()) {
        hexRepresentation = "0";
    }
    return hexRepresentation;
}

void displayRegisters(){
    vector<int> regs(32); 
    regs[0] = static_cast<int>(zero.to_ulong());
    regs[1] = static_cast<int>(ra.to_ulong());
    regs[2] = static_cast<int>(sp.to_ulong());
    regs[3] = static_cast<int>(gp.to_ulong());
    regs[4] = static_cast<int>(tp.to_ulong());
    regs[5] = static_cast<int>(t0.to_ulong());
    regs[6] = static_cast<int>(t1.to_ulong());
    regs[7] = static_cast<int>(t2.to_ulong());
    regs[8] = static_cast<int>(s0.to_ulong());
    regs[9] = static_cast<int>(s1.to_ulong());
    regs[10] = static_cast<int>(a0.to_ulong());
    regs[11] = static_cast<int>(a1.to_ulong());
    regs[12] = static_cast<int>(a2.to_ulong());
    regs[13] = static_cast<int>(a3.to_ulong());
    regs[14] = static_cast<int>(a4.to_ulong());
    regs[15] = static_cast<int>(a5.to_ulong());
    regs[16] = static_cast<int>(a6.to_ulong());
    regs[17] = static_cast<int>(a7.to_ulong());
    regs[18] = static_cast<int>(s2.to_ulong());
    regs[19] = static_cast<int>(s3.to_ulong());
    regs[20] = static_cast<int>(s4.to_ulong());
    regs[21] = static_cast<int>(s5.to_ulong());
    regs[22] = static_cast<int>(s6.to_ulong());
    regs[23] = static_cast<int>(s7.to_ulong());
    regs[24] = static_cast<int>(s8.to_ulong());
    regs[25] = static_cast<int>(s9.to_ulong());
    regs[26] = static_cast<int>(s10.to_ulong());
    regs[27] = static_cast<int>(s11.to_ulong());
    regs[28] = static_cast<int>(t3.to_ulong());
    regs[29] = static_cast<int>(t4.to_ulong());
    regs[30] = static_cast<int>(t5.to_ulong());
    regs[31] = static_cast<int>(t6.to_ulong());

    cout << "Register\t\tDecimal\t\tBinary\t\tHex\n"; 
    for(int index = 0; index < regs.size(); ++index){
        cout << "x" << index << "\t\t" << regs[index] << "\t\t" << binaryFromDecimal(regs[index]) << "\t\t" << hexadecimalFromDecimal(regs[index])  << "\n"; 
    }  
}

void displayMemoryContents(){
    cout << "Memory in Decimal:\n";
    cout << "Address\t\t" << "Value\n";
    for(const auto& element : addressToValueMap){
        cout << element.first << "\t\t" << element.second << "\n";
    }

    cout << "Memory in Binary:\n";
    cout << "Address\t\t" << "Value\n";
    for(const auto& element : addressToValueMap){
        cout << binaryFromDecimal(stoi(element.first)) << "\t\t" << binaryFromDecimal(stoi(element.second)) << "\n";
    }

    cout << "Memory in Hexadecimal:\n";
    cout << "Address\t\t" << "Value\n";
    for(const auto& element : addressToValueMap){
        cout << hexadecimalFromDecimal(stoi(element.first)) << "\t\t" << hexadecimalFromDecimal(stoi(element.second)) << "\n";
    }
}

void MUL(bitset<32>* dest, const bitset<32>* source1, const bitset<32>* source2){
    int firstOperand = static_cast<int>(source1->to_ulong());
    int secondOperand = static_cast<int>(source2->to_ulong());
    int product = firstOperand * secondOperand; 
    bitset<32> result(product); 
    for(int idx = 0; idx < 32; idx++){
        (*dest)[idx] = result[idx];
    }
    if(dest == &zero){
        *dest = 0;
    }
}

void LUI(bitset<32>* dest, int immediate){
    bitset<32> result(immediate); 
    for(int idx = 31; idx >= 12; idx--){
        (*dest)[idx] = result[idx];
    }
    for(int idx = 11; idx >= 0; idx--){
        (*dest)[idx] = 0; 
    }
    if(dest == &zero){
        *dest = 0;
    }
}

void AUIPC(bitset<32>* rd, int programCounter, int imm){
    int sum = programCounter+imm; 
    bitset<32> temp(sum); 
    for(int i=31; i>=12; i--){
        (*rd)[i]=temp[i];
    }
    for(int i=11; i>=0; i--){
        (*rd)[i]=0; 
    }
    if(rd == &zero){
        (*rd) = 0 ;
    }
}

void AND(bitset<32>* rd, const bitset<32>* rs1, const bitset<32>* rs2){
    (*rd) = (*rs1) & (*rs2); 
    if(rd == &zero){
        (*rd) = 0 ;
    }
}

void OR(bitset<32>* rd, const bitset<32>* rs1, const bitset<32>* rs2){
    (*rd) = (*rs1) | (*rs2); 
    if(rd == &zero){
        (*rd) = 0 ;
    }
}

void XOR(bitset<32>* rd, const bitset<32>* rs1, const bitset<32>* rs2){
    (*rd) = (*rs1) ^ (*rs2);
    if(rd == &zero){
        (*rd) = 0 ;
    } 
}

void ANDI(bitset<32>* rd, const bitset<32>* rs1, int imm){
    bitset<32>temp(imm); 
    for(int i=0; i<32; i++){
        (*rd)[i]=(*rs1)[i] & temp[i];
    }
    if(rd == &zero){
        (*rd) = 0 ;
    }
}

void ORI(bitset<32>* rd, const bitset<32>* rs1, int imm){
    bitset<32>temp(imm); 
    for(int i=0; i<32; i++){
        (*rd)[i]=(*rs1)[i] | temp[i];
    }
    if(rd == &zero){
        (*rd) = 0 ;
    }
}

void XORI(bitset<32>* rd, const bitset<32>* rs1, int imm){
    bitset<32>temp(imm); 
    for(int i=0; i<32; i++){
        (*rd)[i]=(*rs1)[i] ^ temp[i];
    }
    if(rd == &zero){
        (*rd) = 0 ;
    }
}

void ADD(bitset<32>* rd, const bitset<32>* rs1, const bitset<32>* rs2){
    int temp1 = (int)((*rs1).to_ulong());
    int temp2 = (int)((*rs2).to_ulong());
    int sum = temp1+temp2; 
    bitset<32> temp(sum); 
    for(int i=0; i<32; i++){
        (*rd)[i]=temp[i];
    }
    if(rd == &zero){
        (*rd) = 0 ;
    }
}

void SUB(bitset<32>* rd, const bitset<32>* rs1, const bitset<32>* rs2){
    int temp1 = (int)((*rs1).to_ulong());
    int temp2 = (int)((*rs2).to_ulong());
    int diff = temp1-temp2; 
    bitset<32> temp(diff); 
    for(int i=0; i<32; i++){
        (*rd)[i]=temp[i];
    }
    if(rd == &zero){
        (*rd) = 0 ;
    }
}

void ADDI(bitset<32>* rd, const bitset<32>* rs1, int imm){
    int rs = (int)((*rs1).to_ulong());
    int sum = rs+imm; 
    bitset<32> temp(sum); 
    for(int i=0; i<32; i++){
        (*rd)[i]=temp[i];
    }
    if(rd == &zero){
        (*rd) = 0 ;
    }
}

void SRL(bitset<32>* rd, const bitset<32>* rs1, const bitset<32>* rs2){
    bitset<5> temp; 
    for(int i=0; i<5; i++){
        temp[i]= (*rs2)[i];
    }
    int shift_amount = temp.to_ulong(); 
    (*rd) = (*rs1) >> shift_amount; 
    if(rd == &zero){
        (*rd) = 0 ;
    }
}

void SRA(bitset<32>* rd, const bitset<32>* rs1, const bitset<32>* rs2){
    bitset<5> temp; 
    for(int i=0; i<5; i++){
        temp[i]= (*rs2)[i];
    }
    int shift_amount = temp.to_ulong(); 
    signed int rs = (int)(*rs1).to_ulong();
    (*rd) = (rs) >> shift_amount;
    if(rd == &zero){
        (*rd) = 0 ;
    }
}

void SLL(bitset<32>* rd, const bitset<32>* rs1, const bitset<32>* rs2){
    bitset<5> temp; 
    for(int i=0; i<5; i++){
        temp[i]= (*rs2)[i];
    }
    int shift_amount = temp.to_ulong(); 
    (*rd) = (*rs1) << shift_amount; 
    if(rd == &zero){
        (*rd) = 0 ;
    }
}

void SLT(bitset<32>* rd, const bitset<32>* rs1, const bitset<32>* rs2){
    int first = (int)(*rs1).to_ulong();
    int second = (int)(*rs2).to_ulong();
    (*rd) = (first<second); 
    if(rd == &zero){
        (*rd) = 0 ;
    }
}

void SLTU(bitset<32>* rd, const bitset<32>* rs1, const bitset<32>* rs2){
    unsigned int first = (*rs1).to_ulong();
    unsigned int second = (*rs2).to_ulong();
    (*rd) = (first<second); 
    if(rd == &zero){
        (*rd) = 0 ;
    }
}

void SLTI(bitset<32>* rd, const bitset<32>* rs1, int imm){
    int first = (int)(*rs1).to_ulong();
    bitset<12> temp(imm); 
    int t = (int)temp.to_ulong();
    (*rd) = (first<t); 
    if(rd == &zero){
        (*rd) = 0 ;
    }
}

void SLTIU(bitset<32>* rd, const bitset<32>* rs1, int imm){
    int first = (int)(*rs1).to_ulong();
    bitset<12> temp(imm); 
    unsigned int t = temp.to_ulong();
    (*rd) = (first<t); 
    if(rd == &zero){
        (*rd) = 0 ;
    }
}

void SLLI(bitset<32>* rd, const bitset<32>* rs1, int imm){
    bitset<5> temp(imm); 
    int shift_amount = temp.to_ulong(); 
    (*rd) = (*rs1) << shift_amount; 
    if(rd == &zero){
        (*rd) = 0 ;
    }
}

void SRLI(bitset<32>* rd, const bitset<32>* rs1, int imm){
    bitset<5> temp(imm); 
    int shift_amount = temp.to_ulong(); 
    (*rd) = (*rs1) >> shift_amount; 
    if(rd == &zero){
        (*rd) = 0 ;
    }
}

void SRAI(bitset<32>* rd, const bitset<32>* rs1, int imm){
    bitset<5> temp(imm); 
    int shift_amount = temp.to_ulong(); 
    signed int rs = (int)(*rs1).to_ulong();
    (*rd) = (rs) >> shift_amount;
    if(rd == &zero){
        (*rd) = 0 ;
    }
}

void JAL(bitset<32>* rd, string label){
    (*rd) = programCounter; 
    int tPC = stoi(varToAddressMap["programCounter"]); 
    programCounter = stoi(varToAddressMap[label]);
    index = (programCounter-tPC)/4;
    if(rd == &zero){
        (*rd) = 0 ;
    }
    return;
}

void JALR(bitset<32>* rd, const bitset<32>* rs1, int offset){
    if(rd != &zero){
        (*rd)=programCounter;
    }
    programCounter = (int)(*rs1).to_ulong()+offset;
    int tPC = stoi(varToAddressMap["programCounter"]); 
    index = (programCounter-tPC)/4;
    if(rd == &zero){
        (*rd) = 0 ;
    }
    return;
}

void BEQ(const bitset<32>* rs1, const bitset<32>* rs2, string label){
    if((*rs1)==(*rs2)){
        int tPC = stoi(varToAddressMap["programCounter"]); 
        programCounter = stoi(varToAddressMap[label]);
        index = (programCounter-tPC)/4;
    }
}

void BNE(const bitset<32>* rs1, const bitset<32>* rs2, string label){
    if((*rs1)!=(*rs2)){
        int tPC = stoi(varToAddressMap["programCounter"]); 
        programCounter = stoi(varToAddressMap[label]);
        index = (programCounter-tPC)/4;
    }
}

void BLT(const bitset<32>* rs1, const bitset<32>* rs2, string label){
    if((int)(*rs1).to_ulong()<(int)(*rs2).to_ulong()){
        int tPC = stoi(varToAddressMap["programCounter"]); 
        programCounter = stoi(varToAddressMap[label]);
        index = (programCounter-tPC)/4;
    }
}

void BGE(const bitset<32>* rs1, const bitset<32>* rs2, string label){
    if((int)(*rs1).to_ulong()>=(int)(*rs2).to_ulong()){
        int tPC = stoi(varToAddressMap["programCounter"]); 
        programCounter = stoi(varToAddressMap[label]);
        index = (programCounter-tPC)/4;
    }
}

void BLTU(const bitset<32>* rs1, const bitset<32>* rs2, string label){
    unsigned long first = (*rs1).to_ulong();
    unsigned long second = (*rs2).to_ulong();
    if(first<second){
        int tPC = stoi(varToAddressMap["programCounter"]); 
        programCounter = stoi(varToAddressMap[label]);
        index = (programCounter-tPC)/4;
    }
}

void BGEU(const bitset<32>* rs1, const bitset<32>* rs2, string label){
    unsigned long first = (*rs1).to_ulong();
    unsigned long second = (*rs2).to_ulong();
    if(first>=second){
        int tPC = stoi(varToAddressMap["programCounter"]); 
        programCounter = stoi(varToAddressMap[label]);
        index = (programCounter-tPC)/4;
    }
}

void LA(bitset<32>* rd, string variable){
    (*rd)=stoi(varToAddressMap[variable]);
    if(rd == &zero){
        (*rd) = 0 ;
    }
}

void LB(bitset<32>* rd, const bitset<32>* rs1, int offset){
    int temp = (int)(*rs1).to_ulong();
    temp+=offset; 
    int value = stoi(addressToValueMap[to_string(temp)]);
    (*rd)=0; 
    bitset<32> tset(value); 
    for(int i=0; i<8; i++){
        (*rd)[i] = tset[i];
    }
    if(tset[7]==1){
        for(int i=8; i<32; i++){
            (*rd)[i] = 1; 
        }
    }
    if(rd == &zero){
        (*rd) = 0 ;
    }
}

void LH(bitset<32>* rd, const bitset<32>* rs1, int offset){
    int temp = (int)(*rs1).to_ulong();
    temp+=offset; 
    int value = stoi(addressToValueMap[to_string(temp)]);
    (*rd)=0; 
    bitset<32> tset(value); 
    for(int i=0; i<16; i++){
        (*rd)[i] = tset[i];
    }
    if(tset[15]==1){
        for(int i=16; i<32; i++){
            (*rd)[i] = 1; 
        }
    }
    if(rd == &zero){
        (*rd) = 0 ;
    }
}

void LBU(bitset<32>* rd, const bitset<32>* rs1, int offset){
    int temp = (int)(*rs1).to_ulong();
    temp+=offset; 
    int value = stoi(addressToValueMap[to_string(temp)]);
    (*rd)=0; 
    bitset<32> tset(value); 
    for(int i=0; i<8; i++){
        (*rd)[i] = tset[i];
    }
    if(rd == &zero){
        (*rd) = 0 ;
    }
}

void LHU(bitset<32>* rd, const bitset<32>* rs1, int offset){
    int temp = (int)(*rs1).to_ulong();
    temp+=offset; 
    int value = stoi(addressToValueMap[to_string(temp)]);
    (*rd)=0; 
    bitset<32> tset(value); 
    for(int i=0; i<16; i++){
        (*rd)[i] = tset[i];
    }
    if(rd == &zero){
        (*rd) = 0 ;
    }
}

void LW(bitset<32>* rd, const bitset<32>* rs1, int offset){
    int temp = (int)(*rs1).to_ulong();
    temp+=offset; 
    int value = stoi(addressToValueMap[to_string(temp)]); 
    bitset<32> tset(value); 
    (*rd)=tset; 
    if(rd == &zero){
        (*rd) = 0 ;
    }
}

void SB(bitset<32>* rs1, const bitset<32>* rs2, int offset){
    int temp = (int)(*rs2).to_ulong();
    temp+=offset; 
    bitset<32> tset(0); 
    for(int i=0; i<8; i++){
        tset[i]=(*rs1)[i];
    }
    int value = (int)tset.to_ulong();
    addressToValueMap[to_string(temp)]=to_string(value);
}

void SH(bitset<32>* rs1, const bitset<32>* rs2, int offset){
    int temp = (int)(*rs2).to_ulong();
    temp+=offset; 
    bitset<32> tset(0); 
    for(int i=0; i<16; i++){
        tset[i]=(*rs1)[i];
    }
    int value = (int)tset.to_ulong();
    addressToValueMap[to_string(temp)]=to_string(value);
}

void SW(bitset<32>* source1, const bitset<32>* source2, int offset){
    int temp = static_cast<int>(source2->to_ulong());
    temp += offset; 
    int value = stoi(addressToValueMap[to_string(temp)]);
    addressToValueMap[to_string(temp)] = to_string(value);
}

bitset<32>* getRegister(string operand){
    if(operand == "zero" || operand == "x0"){
        return &zero; 
    }
    if(operand == "ra" || operand == "x1"){
        return &ra; 
    }
    if(operand == "sp" || operand == "x2"){
        return &sp; 
    }
    if(operand == "gp" || operand == "x3"){
        return &gp; 
    }
    if(operand == "tp" || operand == "x4"){
        return &tp; 
    }
    if(operand == "t0" || operand == "x5"){
        return &t0; 
    }
    if(operand == "t1" || operand == "x6"){
        return &t1; 
    }
    if(operand == "t2" || operand == "x7"){
        return &t2; 
    }
    if(operand == "s0" || operand == "x8"){
        return &s0; 
    }
    if(operand == "s1" || operand == "x9"){
        return &s1; 
    }
    if(operand == "a0" || operand == "x10"){
        return &a0; 
    }
    if(operand == "a1" || operand == "x11"){
        return &a1; 
    }
    if(operand == "a2" || operand == "x12"){
        return &a2; 
    }
    if(operand == "a3" || operand == "x13"){
        return &a3; 
    }
    if(operand == "a4" || operand == "x14"){
        return &a4; 
    }
    if(operand == "a5" || operand == "x15"){
        return &a5; 
    }
    if(operand == "a6" || operand == "x16"){
        return &a6; 
    }
    if(operand == "a7" || operand == "x17"){
        return &a7; 
    }
    if(operand == "s2" || operand == "x18"){
        return &s2; 
    }
    if(operand == "s3" || operand == "x19"){
        return &s3; 
    }
    if(operand == "s4" || operand == "x20"){
        return &s4; 
    }
    if(operand == "s5" || operand == "x21"){
        return &s5; 
    }
    if(operand == "s6" || operand == "x22"){
        return &s6; 
    }
    if(operand == "s7" || operand == "x23"){
        return &s7; 
    }
    if(operand == "s8" || operand == "x24"){
        return &s8; 
    }
    if(operand == "s9" || operand == "x25"){
        return &s9; 
    }
    if(operand == "s10" || operand == "x26"){
        return &s10; 
    }
    if(operand == "s11" || operand == "x27"){
        return &s11; 
    }
    if(operand == "t3" || operand == "x28"){
        return &t3; 
    }
    if(operand == "t4" || operand == "x29"){
        return &t4; 
    }
    if(operand == "t5" || operand == "x30"){
        return &t5; 
    }
    if(operand == "t6" || operand == "x31"){
        return &t6; 
    }
    return NULL;
}

void loadDataFromFile(){
    cout << "Select Data File (e.g., 'data1.txt'): ";
    string filename;
    cin >> filename;
    ifstream fileStream;
    fileStream.open("./" + filename, ios::in); // Opens file from the current directory

    if (!fileStream.is_open()) {
        cerr << "Error: File cannot be opened or does not exist." << endl;
        return;
    }

    string record;
    while (getline(fileStream, record)) {
        stringstream recordStream(record);
        string field;
        vector<string> recordFields; 
        while (getline(recordStream, field, ',')) {
            recordFields.push_back(field);
        }
        varToAddressMap[recordFields[0]] = recordFields[1]; 

        int offset = 0; 
        int baseAddress = stoi(recordFields[1]); 
        for (size_t i = 2; i < recordFields.size(); ++i) {
            int currentAddress = baseAddress + offset; 
            addressToValueMap[to_string(currentAddress)] = recordFields[i];
            offset += 4; 
        }
    }
    fileStream.close();
}

void loadProgram(){
    programCounter = stoi(addressToValueMap[varToAddressMap["programCounter"]]);
    bitset<32> stackPointer(stoi(addressToValueMap[varToAddressMap["sp"]]));
    for(int i = 0; i < 32; i++){
        sp[i] = stackPointer[i]; 
    }

    cout << "Select Program File (e.g., 'program1.txt'): ";
    string filename;
    cin >> filename;
    ifstream fileStream;
    fileStream.open("./" + filename, ios::in); // Opens file from the current directory

    if (!fileStream.is_open()) {
        cerr << "Error: File cannot be opened or does not exist." << endl;
        return;
    }

    string instruction;
    int lineCount = 0;
    while (getline(fileStream, instruction)) {
        instructions.push_back(instruction); 
        stringstream instructionStream(instruction);
        string word;
        vector<string> instructionComponents; 
        while (getline(instructionStream, word, ' ')) {
            instructionComponents.push_back(word);
        }
        if(instructionComponents.size() == 1){
            instructionComponents[0].pop_back();
            varToAddressMap[instructionComponents[0]] = to_string(programCounter + (lineCount * 4)); // Keeping the labels' locations.
        }
        lineCount++; 
    }
    fileStream.close();
}

void executeInstruction(vector<string> inputs){
    string instruction = inputs[0];
    if(instruction=="mul" || instruction == "MUL"){
        inputs[1].pop_back();
        bitset<32>* rd = getRegister(inputs[1]);
        inputs[2].pop_back();
        bitset<32>* rs1 = getRegister(inputs[2]);
        bitset<32>* rs2 = getRegister(inputs[3]);
        MUL(rd, rs1, rs2);
        return; 
    }
    if(instruction=="sw" || instruction == "SW"){
        inputs[1].pop_back();
        bitset<32>* rs1 = getRegister(inputs[1]);
        string str_offset = "";
        for(int j=0; j<inputs[2].length(); j++){
            if(inputs[2][j]-'0' >=0 && inputs[2][j]-'0' <10){
                str_offset += inputs[2][j];
            }else{
                break;
            }
        }
        inputs[2].pop_back();
        inputs[2].erase(0, str_offset.length()+1);
        bitset<32>* rs2 = getRegister(inputs[2]);
        SW(rs1, rs2 , stoi(str_offset));
        return; 
    }
    if(instruction=="sh" || instruction == "SH"){
        inputs[1].pop_back();
        bitset<32>* rs1 = getRegister(inputs[1]);
        string str_offset = "";
        for(int j=0; j<inputs[2].length(); j++){
            if(inputs[2][j]-'0' >=0 && inputs[2][j]-'0' <10){
                str_offset += inputs[2][j];
            }else{
                break;
            }
        }
        inputs[2].pop_back();
        inputs[2].erase(0, str_offset.length()+1);
        bitset<32>* rs2 = getRegister(inputs[2]);
        SH(rs1, rs2 , stoi(str_offset));
        return; 
    }
    if(instruction=="sb" || instruction == "SB"){
        inputs[1].pop_back();
        bitset<32>* rs1 = getRegister(inputs[1]);
        string str_offset = "";
        for(int j=0; j<inputs[2].length(); j++){
            if(inputs[2][j]-'0' >=0 && inputs[2][j]-'0' <10){
                str_offset += inputs[2][j];
            }else{
                break;
            }
        }
        inputs[2].pop_back();
        inputs[2].erase(0, str_offset.length()+1);
        bitset<32>* rs2 = getRegister(inputs[2]);
        SB(rs1, rs2 , stoi(str_offset));
        return; 
    }
    if(instruction=="lw" || instruction == "LW"){
        inputs[1].pop_back();
        bitset<32>* rd = getRegister(inputs[1]);
        string str_offset = "";
        for(int j=0; j<inputs[2].length(); j++){
            if(inputs[2][j]-'0' >=0 && inputs[2][j]-'0' <10){
                str_offset += inputs[2][j];
            }else{
                break;
            }
        }
        inputs[2].pop_back();
        inputs[2].erase(0, str_offset.length()+1);
        bitset<32>* rs1 = getRegister(inputs[2]);
        LW(rd, rs1 , stoi(str_offset));
        return; 
    }
    if(instruction=="lhu" || instruction == "LHU"){
        inputs[1].pop_back();
        bitset<32>* rd = getRegister(inputs[1]);
        string str_offset = "";
        for(int j=0; j<inputs[2].length(); j++){
            if(inputs[2][j]-'0' >=0 && inputs[2][j]-'0' <10){
                str_offset += inputs[2][j];
            }else{
                break;
            }
        }
        inputs[2].pop_back();
        inputs[2].erase(0, str_offset.length()+1);
        bitset<32>* rs1 = getRegister(inputs[2]);
        LHU(rd, rs1 , stoi(str_offset));
        return; 
    }
    if(instruction=="lbu" || instruction == "LBU"){
        inputs[1].pop_back();
        bitset<32>* rd = getRegister(inputs[1]);
        string str_offset = "";
        for(int j=0; j<inputs[2].length(); j++){
            if(inputs[2][j]-'0' >=0 && inputs[2][j]-'0' <10){
                str_offset += inputs[2][j];
            }else{
                break;
            }
        }
        inputs[2].pop_back();
        inputs[2].erase(0, str_offset.length()+1);
        bitset<32>* rs1 = getRegister(inputs[2]);
        LBU(rd, rs1 , stoi(str_offset));
        return; 
    }
    if(instruction=="lh" || instruction == "LH"){
        inputs[1].pop_back();
        bitset<32>* rd = getRegister(inputs[1]);
        string str_offset = "";
        for(int j=0; j<inputs[2].length(); j++){
            if(inputs[2][j]-'0' >=0 && inputs[2][j]-'0' <10){
                str_offset += inputs[2][j];
            }else{
                break;
            }
        }
        inputs[2].pop_back();
        inputs[2].erase(0, str_offset.length()+1);
        bitset<32>* rs1 = getRegister(inputs[2]);
        LH(rd, rs1 , stoi(str_offset));
        return; 
    }
    if(instruction=="lb" || instruction == "LB"){
        inputs[1].pop_back();
        bitset<32>* rd = getRegister(inputs[1]);
        string str_offset = "";
        for(int j=0; j<inputs[2].length(); j++){
            if(inputs[2][j]-'0' >=0 && inputs[2][j]-'0' <10){
                str_offset += inputs[2][j];
            }else{
                break;
            }
        }
        inputs[2].pop_back();
        inputs[2].erase(0, str_offset.length()+1);
        bitset<32>* rs1 = getRegister(inputs[2]);
        LB(rd, rs1 , stoi(str_offset));
        return; 
    }
    if(instruction=="bgeu" || instruction == "BGEU"){
        inputs[1].pop_back();
        bitset<32>* rs1 = getRegister(inputs[1]);
        inputs[2].pop_back();
        bitset<32>* rs2 = getRegister(inputs[2]);
        BGEU(rs1, rs2, inputs[3]);
        return; 
    }
    if(instruction=="bltu" || instruction == "BLTU"){
        inputs[1].pop_back();
        bitset<32>* rs1 = getRegister(inputs[1]);
        inputs[2].pop_back();
        bitset<32>* rs2 = getRegister(inputs[2]);
        BLTU(rs1, rs2, inputs[3]);
        return; 
    }
    if(instruction=="bge" || instruction == "BGE"){
        inputs[1].pop_back();
        bitset<32>* rs1 = getRegister(inputs[1]);
        inputs[2].pop_back();
        bitset<32>* rs2 = getRegister(inputs[2]);
        BGE(rs1, rs2, inputs[3]);
        return; 
    }
    if(instruction=="blt" || instruction == "BLT"){
        inputs[1].pop_back();
        bitset<32>* rs1 = getRegister(inputs[1]);
        inputs[2].pop_back();
        bitset<32>* rs2 = getRegister(inputs[2]);
        BLT(rs1, rs2, inputs[3]);
        return; 
    }
    if(instruction=="bne" || instruction == "BNE"){
        inputs[1].pop_back();
        bitset<32>* rs1 = getRegister(inputs[1]);
        inputs[2].pop_back();
        bitset<32>* rs2 = getRegister(inputs[2]);
        BNE(rs1, rs2, inputs[3]);
        return; 
    }
    if(instruction=="beq" || instruction == "BEQ"){
        inputs[1].pop_back();
        bitset<32>* rs1 = getRegister(inputs[1]);
        inputs[2].pop_back();
        bitset<32>* rs2 = getRegister(inputs[2]);
        BEQ(rs1, rs2, inputs[3]);
        return; 
    }
    if(instruction=="la" || instruction == "LA"){
        inputs[1].pop_back();
        bitset<32>* rd = getRegister(inputs[1]);
        LA(rd, inputs[2]);
        return; 
    }
    if(instruction=="jalr" || instruction == "JALR"){
        inputs[1].pop_back();
        bitset<32>* rd = getRegister(inputs[1]);
        string str_offset = "";
        for(int j=0; j<inputs[2].length(); j++){
            if(inputs[2][j]-'0' >=0 && inputs[2][j]-'0' <10){
                str_offset += inputs[2][j];
            }else{
                break;
            }
        }
        inputs[2].pop_back();
        inputs[2].erase(0, str_offset.length()+1);
        bitset<32>* rs1 = getRegister(inputs[2]);
        JALR(rd, rs1 , stoi(str_offset));
        return; 
    }
    if(instruction=="jal" || instruction == "JAL"){
        inputs[1].pop_back();
        bitset<32>* rd = getRegister(inputs[1]);
        JAL(rd, inputs[2]);
        return; 
    }
    if(instruction=="srai" || instruction == "SRAI"){
        inputs[1].pop_back();
        bitset<32>* rd = getRegister(inputs[1]);
        inputs[2].pop_back();
        bitset<32>* rs1 = getRegister(inputs[2]);
        int imm = stoi(inputs[3]);
        SRAI(rd, rs1, imm);
        return; 
    }
    if(instruction=="srli" || instruction == "SRLI"){
        inputs[1].pop_back();
        bitset<32>* rd = getRegister(inputs[1]);
        inputs[2].pop_back();
        bitset<32>* rs1 = getRegister(inputs[2]);
        int imm = stoi(inputs[3]);
        SRLI(rd, rs1, imm);
        return; 
    }
    if(instruction=="slli" || instruction == "SLLI"){
        inputs[1].pop_back();
        bitset<32>* rd = getRegister(inputs[1]);
        inputs[2].pop_back();
        bitset<32>* rs1 = getRegister(inputs[2]);
        int imm = stoi(inputs[3]);
        SLLI(rd, rs1, imm);
        return; 
    }
    if(instruction=="sltiu" || instruction == "SLTIU"){
        inputs[1].pop_back();
        bitset<32>* rd = getRegister(inputs[1]);
        inputs[2].pop_back();
        bitset<32>* rs1 = getRegister(inputs[2]);
        int imm = stoi(inputs[3]);
        SLTIU(rd, rs1, imm);
        return; 
    }
    if(instruction=="slti" || instruction == "SLTI"){
        inputs[1].pop_back();
        bitset<32>* rd = getRegister(inputs[1]);
        inputs[2].pop_back();
        bitset<32>* rs1 = getRegister(inputs[2]);
        int imm = stoi(inputs[3]);
        SLTI(rd, rs1, imm);
        return; 
    }
    if(instruction=="sltu" || instruction == "SLTU"){
        inputs[1].pop_back();
        bitset<32>* rd = getRegister(inputs[1]);
        inputs[2].pop_back();
        bitset<32>* rs1 = getRegister(inputs[2]);
        bitset<32>* rs2 = getRegister(inputs[3]);
        SLTU(rd, rs1, rs2);
        return; 
    }
    if(instruction=="slt" || instruction == "SLT"){
        inputs[1].pop_back();
        bitset<32>* rd = getRegister(inputs[1]);
        inputs[2].pop_back();
        bitset<32>* rs1 = getRegister(inputs[2]);
        bitset<32>* rs2 = getRegister(inputs[3]);
        SLT(rd, rs1, rs2);
        return; 
    }
    if(instruction=="sll" || instruction == "SLL"){
        inputs[1].pop_back();
        bitset<32>* rd = getRegister(inputs[1]);
        inputs[2].pop_back();
        bitset<32>* rs1 = getRegister(inputs[2]);
        bitset<32>* rs2 = getRegister(inputs[3]);
        SLL(rd, rs1, rs2);
        return; 
    }
    if(instruction=="sra" || instruction == "SRA"){
        inputs[1].pop_back();
        bitset<32>* rd = getRegister(inputs[1]);
        inputs[2].pop_back();
        bitset<32>* rs1 = getRegister(inputs[2]);
        bitset<32>* rs2 = getRegister(inputs[3]);
        SRA(rd, rs1, rs2);
        return; 
    }
    if(instruction=="srl" || instruction == "SRL"){
        inputs[1].pop_back();
        bitset<32>* rd = getRegister(inputs[1]);
        inputs[2].pop_back();
        bitset<32>* rs1 = getRegister(inputs[2]);
        bitset<32>* rs2 = getRegister(inputs[3]);
        SRL(rd, rs1, rs2);
        return; 
    }
    if(instruction=="lui" || instruction == "LUI"){
        inputs[1].pop_back();
        bitset<32>* rd = getRegister(inputs[1]);
        int imm = stoi(inputs[2]); 
        LUI(rd, imm);
        return;
    }
    if(instruction=="auipc" || instruction == "AUIPC"){
        inputs[1].pop_back();
        bitset<32>* rd = getRegister(inputs[1]);
        int imm = stoi(inputs[2]); 
        AUIPC(rd, programCounter, imm);
        return; 
    }
    if(instruction=="and" || instruction == "AND"){
        inputs[1].pop_back();
        bitset<32>* rd = getRegister(inputs[1]);
        inputs[2].pop_back();
        bitset<32>* rs1 = getRegister(inputs[2]);
        bitset<32>* rs2 = getRegister(inputs[3]);
        AND(rd, rs1, rs2);
        return; 
    }
    if(instruction=="or" || instruction == "OR"){
        inputs[1].pop_back();
        bitset<32>* rd = getRegister(inputs[1]);
        inputs[2].pop_back();
        bitset<32>* rs1 = getRegister(inputs[2]);
        bitset<32>* rs2 = getRegister(inputs[3]);
        OR(rd, rs1, rs2);
        return; 
    }
    if(instruction=="xor" || instruction == "XOR"){
        inputs[1].pop_back();
        bitset<32>* rd = getRegister(inputs[1]);
        inputs[2].pop_back();
        bitset<32>* rs1 = getRegister(inputs[2]);
        bitset<32>* rs2 = getRegister(inputs[3]);
        XOR(rd, rs1, rs2);
        return; 
    }
    if(instruction=="andi" || instruction == "ANDI"){
        inputs[1].pop_back();
        bitset<32>* rd = getRegister(inputs[1]);
        inputs[2].pop_back();
        bitset<32>* rs1 = getRegister(inputs[2]);
        int imm = stoi(inputs[3]);
        ANDI(rd, rs1, imm);
        return; 
    }
    if(instruction=="ori" || instruction == "ORI"){
        inputs[1].pop_back();
        bitset<32>* rd = getRegister(inputs[1]);
        inputs[2].pop_back();
        bitset<32>* rs1 = getRegister(inputs[2]);
        int imm = stoi(inputs[3]);
        ORI(rd, rs1, imm);
        return; 
    }
    if(instruction=="xori" || instruction == "XORI"){
        inputs[1].pop_back();
        bitset<32>* rd = getRegister(inputs[1]);
        inputs[2].pop_back();
        bitset<32>* rs1 = getRegister(inputs[2]);
        int imm = stoi(inputs[3]);
        XORI(rd, rs1, imm);
        return; 
    }
    if(instruction=="add" || instruction == "ADD"){
        inputs[1].pop_back();
        bitset<32>* rd = getRegister(inputs[1]);
        inputs[2].pop_back();
        bitset<32>* rs1 = getRegister(inputs[2]);
        bitset<32>* rs2 = getRegister(inputs[3]);
        ADD(rd, rs1, rs2);
        return; 
    }
    if(instruction=="sub" || instruction == "SUB"){
        inputs[1].pop_back();
        bitset<32>* rd = getRegister(inputs[1]);
        inputs[2].pop_back();
        bitset<32>* rs1 = getRegister(inputs[2]);
        bitset<32>* rs2 = getRegister(inputs[3]);
        SUB(rd, rs1, rs2);
        return; 
    }
    if(instruction=="addi" || instruction == "ADDI"){
        inputs[1].pop_back();
        bitset<32>* rd = getRegister(inputs[1]);
        inputs[2].pop_back();
        bitset<32>* rs1 = getRegister(inputs[2]);
        int imm = stoi(inputs[3]);
        ADDI(rd, rs1, imm);
        return; 
    }
}

void runProgram() {
    string currentLine = instructions[0];
    int instructionIndex = 0;
    while(currentLine != "ECALL" && currentLine != "EBREAK" && currentLine != "FENCE"){ 
        stringstream lineStream(currentLine);
        string word;
        vector<string> commandParts; 
        while(getline(lineStream, word, ' ')){
            commandParts.push_back(word);
        }
        if(commandParts.size() == 1){ // Skip label and advance to the next line
            instructionIndex++;
            currentLine = instructions[instructionIndex];
            commandParts[0].pop_back(); 
            programCounter = stoi(varToAddressMap[commandParts[0]]) + 4; 
            continue;
        }
        executeInstruction(commandParts); 
        instructionIndex++;
        currentLine = instructions[instructionIndex];
        programCounter += 4; 
    }
}

int main() {
    loadDataFromFile(); // Loads data from a file
    loadProgram(); // Loads program from a file
    runProgram(); // Executes the program
    cout << "Final value of a0: " << (int)(a0).to_ulong() << endl; 
    return 0;
}
