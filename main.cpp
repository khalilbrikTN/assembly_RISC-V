#include<bits/stdc++.h>
using namespace std;
unordered_map<string, string> memory_locations; //variable name ---> address
unordered_map<string, string> data_values; //address ---> value
vector<string> program;
bitset<32> zero(0);
bitset<32> ra(0);
bitset<32> sp;
bitset<32> gp;
bitset<32> tp(0);
bitset<32> t0(0);
bitset<32> t1(0);
bitset<32> t2(0);
bitset<32> s0(0);
bitset<32> s1(0);
bitset<32> a0(0);
bitset<32> a1(0);
bitset<32> a2(0);
bitset<32> a3(0);
bitset<32> a4(0);
bitset<32> a5(0);
bitset<32> a6(0);
bitset<32> a7(0);
bitset<32> s2(0);
bitset<32> s3(0);
bitset<32> s4(0);
bitset<32> s5(0);
bitset<32> s6(0);
bitset<32> s7(0);
bitset<32> s8(0);
bitset<32> s9(0);
bitset<32> s10(0);
bitset<32> s11(0);
bitset<32> t3(0);
bitset<32> t4(0);
bitset<32> t5(0);
bitset<32> t6(0);
int PC;
int i=0;

string dec_to_binary(int decimal){
    string binaryStr = "";
    while (decimal > 0) {
        binaryStr = to_string(decimal % 2) + binaryStr;
        decimal = decimal / 2;
    }
    return binaryStr;
}

string dec_to_hex(int decimal){
    string hexStr = "";
    int hexValue = 0;
    int hexLen = 0;
    while (decimal > 0) {
        hexValue = decimal % 16;
        if (hexValue < 10) {
            hexStr = to_string(hexValue) + hexStr;
        }
        else {
            hexStr = char(hexValue - 10 + 'A') + hexStr;
        }
        decimal /= 16;
        hexLen++;
    }
    if (hexLen < 1) {
        hexStr = "0" + hexStr;
    }
    return hexStr;
}

void print_registers(){
    ofstream file("registers.csv", ios::app);
    if (!file.is_open()) {
        std::cerr << "Failed to open the file." << std::endl;
        return;
    }

    vector<int> registers(32);
    registers[0] = (int)zero.to_ulong();
    registers[1] = (int)ra.to_ulong();
    registers[2] = (int)sp.to_ulong();
    registers[3] = (int)gp.to_ulong();
    registers[4] = (int)tp.to_ulong();
    registers[5] = (int)t0.to_ulong();
    registers[6] = (int)t1.to_ulong();
    registers[7] = (int)t2.to_ulong();
    registers[8] = (int)s0.to_ulong();
    registers[9] = (int)s1.to_ulong();
    registers[10] = (int)a0.to_ulong();
    registers[11] =  (int)a1.to_ulong();
    registers[12] = (int)a2.to_ulong();
    registers[13] = (int)a3.to_ulong();
    registers[14] = (int)a4.to_ulong();
    registers[15] = (int)a5.to_ulong();
    registers[16] = (int)a6.to_ulong();
    registers[17] = (int)a7.to_ulong();
    registers[18] = (int)s2.to_ulong();
    registers[19] = (int)s3.to_ulong();
    registers[20] = (int)s4.to_ulong();
    registers[21] = (int)s5.to_ulong();
    registers[22] = (int)s6.to_ulong();
    registers[23] = (int)s7.to_ulong();
    registers[24] = (int)s8.to_ulong();
    registers[25] = (int)s9.to_ulong();
    registers[26] = (int)s10.to_ulong();
    registers[27] = (int)s11.to_ulong();
    registers[28] = (int)t3.to_ulong();
    registers[29] = (int)t4.to_ulong();
    registers[30] = (int)t5.to_ulong();
    registers[31] = (int)t6.to_ulong();
    for(int i=0; i<31; i++){
        file << registers[i] << ", ";
    }
    file << registers[31] << "\n";
}

void print_memory(){
    cout << "Memory in Decimal:\n";
    cout << "Address\t\t" << "value\n";
    for(auto i:data_values){
        cout << i.first << "\t\t" << i.second << "\n";
    }
    cout << "Memory in Binary:\n";
    cout << "Address\t\t" << "value\n";
    for(auto i:data_values){
        cout << dec_to_binary(stoi(i.first)) << "\t\t" << dec_to_binary(stoi(i.second)) << "\n";
    }
    cout << "Memory in HEX:\n";
    cout << "Address\t\t" << "value\n";
    for(auto i:data_values){
        cout << dec_to_hex(stoi(i.first)) << "\t\t" << dec_to_hex(stoi(i.second)) <<"\n";
    }
}

void MUL(bitset<32>* rd, const bitset<32>* rs1, const bitset<32>* rs2){
    int temp1 = (int)((*rs1).to_ulong());
    int temp2 = (int)((*rs2).to_ulong());
    int sum = temp1*temp2;
    bitset<32> temp(sum);
    for(int i=0; i<32; i++){
        (*rd)[i]=temp[i];
    }
    if(rd == &zero){
        (*rd) = 0 ;
    }
}

void LUI(bitset<32>*rd, int imm){
    bitset<32> temp(imm);
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

void AUIPC(bitset<32>* rd, int PC, int imm){
    int sum = PC+imm;
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
    (*rd) = PC;
    int tPC = stoi(memory_locations["PC"]);
    PC = stoi(memory_locations[label]);
    i = (PC-tPC)/4;
    if(rd == &zero){
        (*rd) = 0 ;
    }
    return;
}

void JALR(bitset<32>* rd, const bitset<32>* rs1, int offset){
    if(rd != &zero){
        (*rd)=PC;
    }
    PC = (int)(*rs1).to_ulong()+offset;
    int tPC = stoi(memory_locations["PC"]);
    i = (PC-tPC)/4;
    if(rd == &zero){
        (*rd) = 0 ;
    }
    return;
}

void BEQ(const bitset<32>* rs1, const bitset<32>* rs2, string label){
    if((*rs1)==(*rs2)){
        int tPC = stoi(memory_locations["PC"]);
        PC = stoi(memory_locations[label]);
        i = (PC-tPC)/4;
    }
}

void BNE(const bitset<32>* rs1, const bitset<32>* rs2, string label){
    if((*rs1)!=(*rs2)){
        int tPC = stoi(memory_locations["PC"]);
        PC = stoi(memory_locations[label]);
        i = (PC-tPC)/4;
    }
}

void BLT(const bitset<32>* rs1, const bitset<32>* rs2, string label){
    if((int)(*rs1).to_ulong()<(int)(*rs2).to_ulong()){
        int tPC = stoi(memory_locations["PC"]);
        PC = stoi(memory_locations[label]);
        i = (PC-tPC)/4;
    }
}

void BGE(const bitset<32>* rs1, const bitset<32>* rs2, string label){
    if((int)(*rs1).to_ulong()>=(int)(*rs2).to_ulong()){
        int tPC = stoi(memory_locations["PC"]);
        PC = stoi(memory_locations[label]);
        i = (PC-tPC)/4;
    }
}

void BLTU(const bitset<32>* rs1, const bitset<32>* rs2, string label){
    unsigned long first = (*rs1).to_ulong();
    unsigned long second = (*rs2).to_ulong();
    if(first<second){
        int tPC = stoi(memory_locations["PC"]);
        PC = stoi(memory_locations[label]);
        i = (PC-tPC)/4;
    }
}

void BGEU(const bitset<32>* rs1, const bitset<32>* rs2, string label){
    unsigned long first = (*rs1).to_ulong();
    unsigned long second = (*rs2).to_ulong();
    if(first>=second){
        int tPC = stoi(memory_locations["PC"]);
        PC = stoi(memory_locations[label]);
        i = (PC-tPC)/4;
    }
}

void LA(bitset<32>* rd, string variable){
    (*rd)=stoi(memory_locations[variable]);
    if(rd == &zero){
        (*rd) = 0 ;
    }
}

void LB(bitset<32>* rd, const bitset<32>* rs1, int offset){
    int temp = (int)(*rs1).to_ulong();
    temp+=offset;
    int value = stoi(data_values[to_string(temp)]);
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
    int value = stoi(data_values[to_string(temp)]);
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
    int value = stoi(data_values[to_string(temp)]);
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
    int value = stoi(data_values[to_string(temp)]);
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
    int value = stoi(data_values[to_string(temp)]);
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
    data_values[to_string(temp)]=to_string(value);
}

void SH(bitset<32>* rs1, const bitset<32>* rs2, int offset){
    int temp = (int)(*rs2).to_ulong();
    temp+=offset;
    bitset<32> tset(0);
    for(int i=0; i<16; i++){
        tset[i]=(*rs1)[i];
    }
    int value = (int)tset.to_ulong();
    data_values[to_string(temp)]=to_string(value);
}

void SW(bitset<32>* rs1, const bitset<32>* rs2, int offset){
    int temp = (int)(*rs2).to_ulong();
    temp+=offset;
    int value = (int)(*rs1).to_ulong();
    data_values[to_string(temp)]=to_string(value);
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

void ReadData(){
    string path = "./data1.txt";
    fstream fin;
    string line;
    fin.open(path, ios::in);
    while(std::getline(fin, line))
    {
        std::stringstream ss(line);
        string temp;
        vector<string> inputs;
        while(std::getline(ss, temp, ',')){
            inputs.push_back(temp);
        }
        memory_locations[inputs[0]] = inputs[1];
        int inc = 0;
        int address = stoi(inputs[1]);
        for(int i=2; i<inputs.size(); i++){
            int adr = address+inc;
            data_values[to_string(adr)] = inputs[i];
            inc+=4;
        }
    }
    fin.close();
}

void uploadProgram(){
    PC = stoi(data_values[memory_locations["PC"]]);
    bitset<32>tsp(stoi(data_values[memory_locations["sp"]]));
    for(int i=0; i<32; i++){
        sp[i]=tsp[i];
    }
    string path = "./program1.txt";
    fstream fin;
    string line;
    fin.open(path, ios::in);
    int inc = 0;
    while(std::getline(fin, line))
    {
        program.push_back(line);
        std::stringstream ss(line);
        string temp;
        vector<string> inputs;
        while(std::getline(ss, temp, ' ')){
            inputs.push_back(temp);
        }
        if(inputs.size()==1){
            inputs[0].pop_back();
            memory_locations[inputs[0]] = to_string(PC+(inc*4)); //keeping the labels' locations.
        }
        inc++;
    }
    fin.close();
}

void executeInstruction(vector<string> inputs){
    string instruction = inputs[0];
    print_registers();
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
        AUIPC(rd, PC, imm);
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

void executeProgram(){
    string line = program[0];
    while(line != "ECALL" && line != "EBREAK" && line != "FENCE"){
        std::stringstream ss(line);
        string temp;
        vector<string> inputs;
        while(std::getline(ss, temp, ' ')){
            inputs.push_back(temp);
        }
        if(inputs.size()==1){ //in case we encounter a label we go to the next line;
            i++;
            line = program[i];
            inputs[0].pop_back();
            PC = stoi(memory_locations[inputs[0]])+4;
            continue;
        }
        executeInstruction(inputs);
        i++;
        line = program[i];
        PC+=4;
    }
}

void clearFileContents(const std::string& filename) {
    // Open the file in write mode, which truncates the file
    std::ofstream file(filename, std::ios::out | std::ios::trunc);

    // Check if the file is open
    if (!file.is_open()) {
        std::cerr << "Failed to open the file." << std::endl;
        return;
    }

    // Close the file (it's already empty now)
    file.close();
}

int main(){
    clearFileContents("registers.csv");
    ReadData();
    uploadProgram();
    executeProgram();
    cout << (int)(a0).to_ulong() << endl;
    return 0;
}