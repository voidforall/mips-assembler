#include "mipsasm.h"

int pc,ra; //global variable
QMap<QString, int> label;
QMap<int, QString> dislabel;

QMap<QString,QString> register_map =
{
    {"$zero", "00000"},
    {"$at", "00001"},
    {"$v0", "00010"},
    {"$v1", "00011"},
    {"$a0", "00100"},
    {"$a1", "00101"},
    {"$a2", "00110"},
    {"$a3", "00111"},
    {"$t0", "01000"},
    {"$t1", "01001"},
    {"$t2", "01010"},
    {"$t3", "01011"},
    {"$t4", "01100"},
    {"$t5", "01101"},
    {"$t6", "01110"},
    {"$t7", "01111"},
    {"$s0", "10000"},
    {"$s1", "10001"},
    {"$s2", "10010"},
    {"$s3", "10011"},
    {"$s4", "10100"},
    {"$s5", "10101"},
    {"$s6", "10110"},
    {"$s7", "10111"},
    {"$t8", "11000"},
    {"$t9", "11001"},
    {"$k0", "11010"},
    {"$k1", "11011"},
    {"$gp", "11100"},
    {"$sp", "11101"},
    {"$fp", "11110"},
    {"$ra", "11111"},
    {"$RO", "00000"},
    {"$R1", "00001"},
    {"$R2", "00010"},
    {"$R3", "00011"},
    {"$R4", "00100"},
    {"$R5", "00101"},
    {"$R6", "00110"},
    {"$R7", "00111"},
    {"$R8", "01000"},
    {"$R9", "01001"},
    {"$R10", "01010"},
    {"$R11", "01011"},
    {"$R12", "01100"},
    {"$R13", "01101"},
    {"$R14", "01110"},
    {"$R15", "01111"},
    {"$R16", "10000"},
    {"$R17", "10001"},
    {"$R18", "10010"},
    {"$R19", "10011"},
    {"$R20", "10100"},
    {"$R21", "10101"},
    {"$R22", "10110"},
    {"$R23", "10111"},
    {"$R24", "11000"},
    {"$R25", "11001"},
    {"$R26", "11010"},
    {"$R27", "11011"},
    {"$R28", "11100"},
    {"$R29", "11101"},
    {"$R30", "11110"},
    {"$R31", "11111"},
    {"$rO", "00000"},
    {"$r1", "00001"},
    {"$r2", "00010"},
    {"$r3", "00011"},
    {"$r4", "00100"},
    {"$r5", "00101"},
    {"$r6", "00110"},
    {"$r7", "00111"},
    {"$r8", "01000"},
    {"$r9", "01001"},
    {"$r10", "01010"},
    {"$r11", "01011"},
    {"$r12", "01100"},
    {"$r13", "01101"},
    {"$r14", "01110"},
    {"$r15", "01111"},
    {"$r16", "10000"},
    {"$r17", "10001"},
    {"$r18", "10010"},
    {"$r19", "10011"},
    {"$r20", "10100"},
    {"$r21", "10101"},
    {"$r22", "10110"},
    {"$r23", "10111"},
    {"$r24", "11000"},
    {"$r25", "11001"},
    {"$r26", "11010"},
    {"$r27", "11011"},
    {"$r28", "11100"},
    {"$r29", "11101"},
    {"$r30", "11110"},
    {"$r31", "11111"},
};

QMap<QString,QString> dregister_map = //disassembly regiseter map
{
    {"00000", "$zero"},
    {"00001", "$at"},
    {"00010", "$v0"},
    {"00011", "$v1"},
    {"00100", "$a0"},
    {"00101", "$a1"},
    {"00110", "$a2"},
    {"00111", "$a3"},
    {"01000", "$t0"},
    {"01001", "$t1"},
    {"01010", "$t2"},
    {"01011", "$t3"},
    {"01100", "$t4"},
    {"01101", "$t5"},
    {"01110", "$t6"},
    {"01111", "$t7"},
    {"10000", "$s0"},
    {"10001", "$s1"},
    {"10010", "$s2"},
    {"10011", "$s3"},
    {"10100", "$s4"},
    {"10101", "$s5"},
    {"10110", "$s6"},
    {"10111", "$s7"},
    {"11000", "$t8"},
    {"11001", "$t9"},
    {"11010", "$k0"},
    {"11011", "$k1"},
    {"11100", "$gp"},
    {"11101", "$sp"},
    {"11110", "$fp"},
    {"11111", "$ra"},
};

QMap<QString, QPair<QString, QString>> instruction_map = {
    {"add",  {"R", "000000ssssstttttddddd00000100000"}},//R-STD
    {"addu", {"R", "000000ssssstttttddddd00000100001"}},//R-STD
    {"sub",  {"R", "000000ssssstttttddddd00000100010"}},//R-STD
    {"subu", {"R", "000000ssssstttttddddd00000100011"}},//R-STD
    {"and",  {"R", "000000ssssstttttddddd00000100100"}},//R-STD
    {"or",   {"R", "000000ssssstttttddddd00000100101"}},//R-STD
    {"xor",  {"R", "000000ssssstttttddddd00000100110"}},//R-STD
    {"nor",  {"R", "000000ssssstttttddddd00000100111"}},//R-STD
    {"sll",  {"RA","00000000000tttttdddddaaaaa000000"}},//R-TDA
    {"srl",  {"RA","00000000000tttttdddddaaaaa000010"}},//R-TDA
    {"sra",  {"RA","00000000000tttttdddddaaaaa000011"}},//R-TDA
    {"sllv", {"R", "000000ssssstttttddddd00000000100"}},//R-STD
    {"srlv", {"R", "000000ssssstttttddddd00000000110"}},//R-STD
    {"srav", {"R", "000000ssssstttttddddd00000000111"}},//R-STD
    {"slt",  {"R", "000000ssssstttttddddd00000101010"}},//R-STD
    {"sltu", {"R", "000000ssssstttttddddd00000101011"}},//R-STD
    {"mult", {"RST","000000sssssttttt0000000000011000"}},//R-ST
    {"multu",{"RST","000000sssssttttt0000000000011001"}},//R-ST
    {"div",  {"RST","000000sssssttttt0000000000011010"}},//R-ST
    {"divu", {"RST","000000sssssttttt0000000000011011"}},//R-ST
    {"jr",   {"RS","000000sssss000000000000000001000"}},//R-S
    {"jalr", {"RSD","000000sssss00000ddddd00000001001"}},//R-SD
    {"syscall", {"syscall","00000000000000000000000000001100"}},//R-system call
    {"break",{"break","000000iiiiiiiiiiiiiiiiiiii001101"}},//R-break
    {"mfhi", {"RD","0000000000000000ddddd00000010000"}},//R-D
    {"mflo", {"RD","0000000000000000ddddd00000010010"}},//R-D
    {"mthi", {"RS","000000sssss000000000000000010001"}},//R-S
    {"mtlo", {"RS","000000sssss000000000000000010011"}},//R-S
    {"addi", {"I","001000ssssstttttiiiiiiiiiiiiiiii"}},//I-STI
    {"addiu",{"I","001001ssssstttttiiiiiiiiiiiiiiii"}},//I-STI
    {"ori",  {"I","001101ssssstttttiiiiiiiiiiiiiiii"}},//I-STI
    {"andi", {"I","001100ssssstttttiiiiiiiiiiiiiiii"}},//I-STI
    {"xori", {"I","001110ssssstttttiiiiiiiiiiiiiiii"}},//I-STI
    {"lui",  {"lui","00111100000tttttiiiiiiiiiiiiiiii"}},//I-TI
    {"slti", {"I","001010ssssstttttiiiiiiiiiiiiiiii"}},//I-STI
    {"sltiu",{"I","001001ssssstttttiiiiiiiiiiiiiiii"}},//I-STI
    {"lw",   {"I","100011ssssstttttiiiiiiiiiiiiiiii"}},//I-STI
    {"lb",   {"I","100000ssssstttttiiiiiiiiiiiiiiii"}},//I-STI
    {"lbu",  {"I","100100ssssstttttiiiiiiiiiiiiiiii"}},//I-STI
    {"lh",   {"I","100001ssssstttttiiiiiiiiiiiiiiii"}},//I-STI
    {"lhu",  {"I","100101ssssstttttiiiiiiiiiiiiiiii"}},//I-STI
    {"sw",   {"I","101011ssssstttttiiiiiiiiiiiiiiii"}},//I-STI
    {"sh",   {"I","101001ssssstttttiiiiiiiiiiiiiiii"}},//I-STI
    {"sb",   {"I","101000ssssstttttiiiiiiiiiiiiiiii"}},//I-STI
    {"beq",  {"I","000100ssssstttttiiiiiiiiiiiiiiii"}},//I-STI
    {"bne",  {"I","000101ssssstttttiiiiiiiiiiiiiiii"}},//I-STI
    {"bgez", {"SI","000001sssss00001iiiiiiiiiiiiiiii"}},//I-SI
    {"bgtz", {"SI","000111sssss00000iiiiiiiiiiiiiiii"}},//I-SI
    {"blez", {"SI","000110sssss00000iiiiiiiiiiiiiiii"}},//I-SI
    {"bltz", {"SI","000001sssss00000iiiiiiiiiiiiiiii"}},//I-SI
    {"j",    {"J","000010iiiiiiiiiiiiiiiiiiiiiiiiii"}},//J-I
    {"jal",  {"J","000011iiiiiiiiiiiiiiiiiiiiiiiiii"}},//J-I
    {"mfc0", {"mfc0","01000000000tttttddddd00000000000"}},//Move from coprocessor
    {"mtc0", {"mtc0","01000000100tttttddddd00000000000"}},//Move to coprocessor
    {"eret", {"eret","01000010000000000000000000011000"}},//Exception Return
};

QMap<QString, QPair<QString, QString>> dinstruction_map = { //instruction table of disassembly
    {"000000ssssstttttddddd00000100000",{"R","add"}},//R-STD
    {"000000ssssstttttddddd00000100001",{"R","addu"}},//R-STD
    {"000000ssssstttttddddd00000100010",{"R","sub"}},//R-STD
    {"000000ssssstttttddddd00000100011",{"R","subu"}},//R-STD
    {"000000ssssstttttddddd00000100100",{"R","and"}},//R-STD
    {"000000ssssstttttddddd00000100101",{"R","or"}},//R-STD
    {"000000ssssstttttddddd00000100110",{"R","xor"}},//R-STD
    {"000000ssssstttttddddd00000100111",{"R","nor"}},//R-STD
    {"00000000000tttttdddddaaaaa000000",{"RA","sll"}},//R-TDA
    {"00000000000tttttdddddaaaaa000010",{"RA","srl"}},//R-TDA
    {"00000000000tttttdddddaaaaa000011",{"RA","sra"}},//R-TDA
    {"000000ssssstttttddddd00000000100",{"R","sllv"}},//R-STD
    {"000000ssssstttttddddd00000000110",{"R","srlv"}},//R-STD
    {"000000ssssstttttddddd00000000111",{"R","srav"}},//R-STD
    {"000000ssssstttttddddd00000101010",{"R","slt"}},//R-STD
    {"000000ssssstttttddddd00000101011",{"R","sltu"}},//R-STD
    {"000000sssssttttt0000000000011000",{"RST","mult"}},//R-ST
    {"000000sssssttttt0000000000011001",{"RST","multu"}},//R-ST
    {"000000sssssttttt0000000000011010",{"RST","div"}},//R-ST
    {"000000sssssttttt0000000000011011",{"RST","divu"}},//R-ST
    {"000000sssss000000000000000001000",{"jr","jr"}},//R-S
    {"000000sssss00000ddddd00000001001",{"jalr","jalr"}},//R-SD
    {"00000000000000000000000000001100",{"syscall","syscall"}},//R-system call
    {"000000iiiiiiiiiiiiiiiiiiii001101",{"break","break"}},//R-break
    {"0000000000000000ddddd00000010000",{"RD","mfhi"}},//R-D
    {"0000000000000000ddddd00000010010",{"RD","mflo"}},//R-D
    {"000000sssss000000000000000010001",{"RS","mthi"}},//R-S
    {"000000sssss000000000000000010011",{"RS","mtlo"}},//R-S
    {"001000ssssstttttiiiiiiiiiiiiiiii",{"I","addi"}},//I-STI
    {"001001ssssstttttiiiiiiiiiiiiiiii",{"I","addiu"}},//I-STI
    {"001101ssssstttttiiiiiiiiiiiiiiii",{"I","ori"}},//I-STI
    {"001100ssssstttttiiiiiiiiiiiiiiii",{"I","andi"}},//I-STI
    {"001110ssssstttttiiiiiiiiiiiiiiii",{"I","xori"}},//I-STI
    {"00111100000tttttiiiiiiiiiiiiiiii",{"lui","lui"}},//I-TI
    {"001010ssssstttttiiiiiiiiiiiiiiii",{"I","slti"}},//I-STI
    {"001001ssssstttttiiiiiiiiiiiiiiii",{"I","sltiu"}},//I-STI
    {"100011ssssstttttiiiiiiiiiiiiiiii",{"I","lw"}},//I-STI
    {"100000ssssstttttiiiiiiiiiiiiiiii",{"I","lb"}},//I-STI
    {"100100ssssstttttiiiiiiiiiiiiiiii",{"I","lbu"}},//I-STI
    {"100001ssssstttttiiiiiiiiiiiiiiii",{"I","lh"}},//I-STI
    {"100101ssssstttttiiiiiiiiiiiiiiii",{"I","lhu"}},//I-STI
    {"101011ssssstttttiiiiiiiiiiiiiiii",{"I","sw"}},//I-STI
    {"101001ssssstttttiiiiiiiiiiiiiiii",{"I","sh"}},//I-STI
    {"101000ssssstttttiiiiiiiiiiiiiiii",{"I","sb"}},//I-STI
    {"000100ssssstttttiiiiiiiiiiiiiiii",{"I","beq"}},//I-STI
    {"000101ssssstttttiiiiiiiiiiiiiiii",{"I","bne"}},//I-STI
    {"000001sssss00001iiiiiiiiiiiiiiii",{"SI","bgez"}},//I-SI
    {"000111sssss00000iiiiiiiiiiiiiiii",{"SI","bgtz"}},//I-SI
    {"000110sssss00000iiiiiiiiiiiiiiii",{"SI","blez"}},//I-SI
    {"000001sssss00000iiiiiiiiiiiiiiii",{"SI","bltz"}},//I-SI
    {"000010iiiiiiiiiiiiiiiiiiiiiiiiii",{"J","j"}},//J-I
    {"000011iiiiiiiiiiiiiiiiiiiiiiiiii",{"J","jal"}},//J-I
    {"01000000000tttttddddd00000000000",{"mfc0","mfc0"}},//Move from coprocessor
    {"01000000100tttttddddd00000000000",{"mtc0","mtc0"}},//Move to coprocessor
    {"01000010000000000000000000011000",{"eret","eret"}},//Exception Return
};

//先按行读入汇编代码
QStringList asm_in(QString &input_text)
{
    QStringList asm_code = input_text.split('\n');
    return asm_code;
}

QStringList pseudo_inst = {
    "#BaseAddre","#DataAddre","DB","DW","DD","EQU","db","dw","dd","equ"
};

//返回第一次扫描后的汇编代码（处理过label和伪指令）
QStringList scan(QStringList& input_text)
{
    QStringList asm_scanned;
    QStringList temp;
    asm_scanned.clear();
    int label_addr = 0;
    QListIterator<QString> itr(input_text);
    pc = 0;

    //判断伪指令
    QString pseudo_type;

    while (itr.hasNext())
    {
        QString currentLine  = itr.next();
        temp = currentLine.split(QRegExp(QString("[: ,]")),QString::SkipEmptyParts);
        if(!temp.empty())
            pseudo_type = temp.at(0);//取at时一定要有约束，不然index会out of range
        //判断指令是否合法（!）留坑
        if(currentLine.endsWith(":") && !pseudo_inst.contains(pseudo_type)){ //把label名及对应地址加入到记录中
            currentLine = currentLine.left(currentLine.length() - 1);
            label_addr = pc + 4;
            label.insert(currentLine,label_addr);
        }
        else if( pseudo_inst.contains(pseudo_type) ){ //存在合法的伪指令并处理伪指令
            if(pseudo_type == "#BaseAddre" && temp.count() > 1){ //设置初始地址(但后续的要比当前大)
                if(temp.at(1).toInt(nullptr,16) > pc){
                    pc = temp.at(1).toInt(nullptr,16);
                }
            }
        }
        else{
            currentLine = currentLine.left(currentLine.indexOf(';'));//同时忽略指令后的comment
            currentLine.replace(QString(";"),QString(""));
            pc += 4;
            asm_scanned.append(currentLine);//如非伪指令或label，则正常转为bin
        }
    }
    return asm_scanned;
}


//汇编过程的主要函数（逐行assembly）
QString asm2bin(QString asm_code){
    QString bin;
    QString type;
    QString rs,rt,rd;
    QString shift_amount,imm16,target;
    QString temp, label_addr; //label变量
    if(asm_code.isEmpty()){
        return "";
    }
    QStringList asm_split = asm_code.split(QRegExp("(\\s|,)"), QString::SkipEmptyParts);
    if(asm_split.size() > 4){ //split后超过四项，检测出语法错误
        return "";
    }
    if(instruction_map.contains(asm_split.at(0))){
        type = instruction_map[asm_split.at(0)].first;
        bin = instruction_map[asm_split.at(0)].second;
        pc += 4;
    }
    else{
        return "";
    }
    for(int i = 0; i < asm_split.size(); i++){//check asm_split中是否有label,如有label,则查表label替换为立即数
        temp = asm_split.at(i);
        if(label.contains(temp)){
            label_addr = QString::number(label[temp],10);
            //label_addr是绝对地址, 在branch时改为计算出offset
            if(type == "I" || type == "SI"){
                label_addr = QString::number((label[temp] - pc - 4)/4,10);
            }
            else if(type == "J"){
                label_addr = QString::number((label[temp] - 4) / 4,10);
            }
            asm_split.replace(i, label_addr);
        }
    }
    if(type == "R"){
        rd = register_map[asm_split.at(1)];
        rs = register_map[asm_split.at(2)];
        rt = register_map[asm_split.at(3)];
        setRegD(rd, bin);
        setRegS(rs, bin);
        setRegT(rt, bin);
    }
    else if(type == "RST"){
        rs = register_map[asm_split.at(1)];
        rt = register_map[asm_split.at(2)];
        setRegS(rs, bin);
        setRegT(rt, bin);
    }
    else if(type == "RA"){
        rd = register_map[asm_split.at(1)];
        rt = register_map[asm_split.at(2)];
        shift_amount = asm_split.at(3);
        setRegD(rd, bin);
        setRegT(rt, bin);
        setShift(shift_amount, bin);
    }
    else if(type == "RS"){
        rs = register_map[asm_split.at(1)];
        setRegS(rs, bin);
    }
    else if(type == "RSD"){
        rd = register_map[asm_split.at(1)];
        rs = register_map[asm_split.at(2)];
        setRegD(rd, bin);
        setRegS(rs, bin);
    }
    else if(type == "RD"){
        rd = register_map[asm_split.at(1)];
        setRegD(rd, bin);
    }
    else if(type == "I"){
        if(asm_split.at(0) == "lw" || asm_split.at(0) == "sw" || asm_split.at(0) == "lb"
                || asm_split.at(0) == "lbu" || asm_split.at(0) == "lh" || asm_split.at(0) == "lhu"
                || asm_split.at(0) == "sh" || asm_split.at(0) == "sb"){
            QStringList temp;
            temp = asm_split.at(2).split(QRegExp(QString("[())]")),QString::SkipEmptyParts);
            rt = register_map[asm_split.at(1)];
            imm16 = temp.at(0);
            rs = register_map[temp.at(1)];
        }
        else{
            rt = register_map[asm_split.at(2)];
            rs = register_map[asm_split.at(1)];
            imm16 = asm_split.at(3);
        }
        setRegT(rt, bin);
        setRegS(rs, bin);
        setImm(imm16, bin);
    }
    else if(type == "SI"){
        rs = register_map[asm_split.at(1)];
        imm16 = asm_split.at(2);
        setRegS(rs, bin);
        setImm(imm16, bin);
    }
    else if(type == "J"){
        target = asm_split.at(1);
        setTarget(target, bin);
    }
    else if(type == "syscall"){}
    else if(type == "break"){
        target = asm_split.at(1);
        QString target26 = QString("%1").arg(target.toInt(), 20, 2, QChar('0'));
        bin.replace(6, 26, target26);
    }
    else if(type == "lui"){
        rt = register_map[asm_split.at(1)];
        imm16 = asm_split.at(2);
        setRegT(rs, bin);
        setImm(imm16, bin);
    }
    else if(type == "mfc0" || type == "mtc0"){
        rt = register_map[asm_split.at(1)];
        rd = register_map[asm_split.at(2)];
        setRegT(rt, bin);
        setRegD(rd, bin);
    }
    else if(type == "eret" || type == "syscall"){
        return bin;
    }
    else{
        //No Instruction
    }
    return bin;
}

//反汇编之前先扫描一次，如果有需要的label则生成并存储在表dislabel里,同时将输入处理为二进制串
QStringList creatLabel(QString& input_text){
    int index = 0;
    pc = 0; //BaseAddress = 0
    QStringList bin_code;
    QStringList hex_code;
    QString opcode;
    int pc_offset;
    if(!input_text.startsWith("memory")){//.bin
        bin_code = input_text.split("\n",QString::SkipEmptyParts);
        QList<QString>::Iterator it = bin_code.begin(),itend = bin_code.end();
        int i = 0;
        for (;it != itend; it++,i++){
            pc += 4;
            opcode = bin_code.at(i).mid(0,6);
            if(opcode == "000100" || opcode == "000101" || opcode == "000001"
                || opcode == "000111" || opcode == "000110" || opcode == "000001"){//branch inst
                pc_offset = pc + bin2int(bin_code.at(i).mid(16,16)) * 4;
                if(!dislabel.contains(pc_offset)){//create label
                    dislabel.insert(pc_offset,"label_" + QString::number(index,10));
                    index++;
                }
            }
            else if(opcode == "000010" || opcode == "000011"){//j/jal inst
                if(!dislabel.contains(bin2int(bin_code.at(i).mid(6,26)))){//create label
                    dislabel.insert(bin2int(bin_code.at(i).mid(6,26)),"label_" + QString::number(index,10));
                    index++;
                }
            }
        }
    }

    else{   //.coe
        input_text = input_text.mid(input_text.indexOf("vector=") + 7);
        input_text.remove(QRegExp("\\s"));
        input_text.remove('\n');
        hex_code = input_text.split(",",QString::SkipEmptyParts);
        QString temp;
        QList<QString>::Iterator it = hex_code.begin(),itend = hex_code.end();
        int i = 0;
        for (;it != itend; it++,i++){
            pc += 4;
            temp = hex2bin(hex_code.at(i));
            bin_code.insert(i,temp);
            opcode = temp.mid(0,6);
            if(opcode == "000100" || opcode == "000101" || opcode == "000001"
                || opcode == "000111" || opcode == "000110" || opcode == "000001"){//branch inst
                pc_offset = pc + bin2int(bin_code.at(i).mid(16,16)) * 4;
                if(!dislabel.contains(pc_offset)){//create label
                    qDebug() << bin_code.at(i).mid(16,16);
                    qDebug() << bin2int(bin_code.at(i).mid(16,16));
                    qDebug() << pc_offset << "label_" + QString::number(index,10);
                    dislabel.insert(pc_offset,"label_" + QString::number(index,10));
                    index++;
                }
            }
            else if(opcode == "000010" || opcode == "000011"){//j/jal inst
                if(!dislabel.contains(bin2int(temp.mid(6,26)))){//create label
                    qDebug() << bin2int(temp.mid(6,26)) << "label_" + QString::number(index,10);
                    dislabel.insert(bin2int(temp.mid(6,26)),"label_" + QString::number(index,10));
                    index++;
                }
            }
        }
    }
    return bin_code;
}

//反汇编操作
QString bin2asm(QString bin_code){ //默认bin_code统一为二进制串，如果coe则在另一个函数中先预处理+分配label
    QString opcode;
    QString asm_code;
    QString rs,rd,rt,sa,imm16,funct,target;
    QString type;

    if(bin_code.length() != 32){
        return "Invalid binary instruciton.";
    }
    opcode = bin_code.mid(0,6);//get the opcode of the input instruction
    rs = bin_code.mid(6,5);
    rt = bin_code.mid(11,5);
    rd = bin_code.mid(16,5);
    sa = bin_code.mid(20,5);
    funct = bin_code.mid(26,6);
    imm16 = bin_code.mid(16,16);
    target = bin_code.mid(6,26);
    pc += 4;
    if(opcode == "000000"){//R-type instruction
        if(funct.mid(0,3) == "100" || funct == "000100" || funct == "000110"
                || funct == "000111" || funct == "101010" || funct == "101011"){//R-STD inst
            bin_code.replace(6,5,"sssss");
            bin_code.replace(11,5,"ttttt");
            bin_code.replace(16,5,"ddddd");
            if(!dinstruction_map.contains(bin_code)){
                return "Invalid binary instruction.";
            }
            else{
                type = dinstruction_map[bin_code].second;
                asm_code = type + " " + dregister_map[rd] + "," + dregister_map[rs] + "," + dregister_map[rt];
            }
        }
        else if(funct == "000000" || funct == "000010" || funct == "000011"){//R-TDA shift inst
            bin_code.replace(6,5,"00000");
            bin_code.replace(11,5,"ttttt");
            bin_code.replace(16,5,"ddddd");
            bin_code.replace(21,5,"aaaaa");
            type = dinstruction_map[bin_code].second;
            asm_code = type + " " + dregister_map[rd] + "," + dregister_map[rt] + "," + imm16.toInt();
        }
        else if(funct.mid(0,4) == "0110"){//R-ST inst
            bin_code.replace(6,5,"sssss");
            bin_code.replace(11,5,"ttttt");
            if(!dinstruction_map.contains(bin_code)){
                return "Invalid binary instruction.";
            }
            else{
                type = dinstruction_map[bin_code].second;
                asm_code = type + " " + dregister_map[rs] + "," + dregister_map[rt];
            }
        }
        else if(funct == "001000" || funct == "010001" || funct == "010011"){//R-S inst
            bin_code.replace(6,5,"sssss");
            if(!dinstruction_map.contains(bin_code)){
                return "Invalid binary instruction.";
            }
            else{
                type = dinstruction_map[bin_code].second;
                asm_code = type + " " + dregister_map[rs];
            }
        }
        else if(funct == "010000" || funct == "010010"){//R-D inst
            bin_code.replace(16,5,"ddddd");
            if(!dinstruction_map.contains(bin_code)){
                return "Invalid binary instruction.";
            }
            else{
                type = dinstruction_map[bin_code].second;
                asm_code = type + " " + dregister_map[rd];
            }
        }
        else if(funct == "001001"){//R-SD jalr
            bin_code.replace(6,5,"sssss");
            bin_code.replace(16,5,"ddddd");
            if(!dinstruction_map.contains(bin_code)){
                return "Invalid binary instruction.";
            }
            else{
                type = dinstruction_map[bin_code].second;
                asm_code = type + " " + dregister_map[rd] + "," + dregister_map[rs];
            }
        }
        else if(funct == "001100")//syscall
            return "syscall";
        else if(funct == "001101")//break
            return "break " + bin_code.mid(6,20).toInt();
        else return "Invalid binary instruction.";
    }

    else if(opcode == "001000" || opcode == "001010"){ //I-type signed STI inst
        bin_code.replace(6,5,"sssss");
        bin_code.replace(11,5,"ttttt");
        bin_code.replace(16,16,"iiiiiiiiiiiiiiii");
        type = dinstruction_map[bin_code].second;
        asm_code = type + " " + dregister_map[rt] + "," + dregister_map[rs] + "," + QString::number(bin2int(imm16),10);
    }
    else if(opcode == "001001" || opcode == "001101" || opcode == "0011000"
            || opcode == "001110" || opcode == "001001"){// I-type unsigned STI inst
        bin_code.replace(6,5,"sssss");
        bin_code.replace(11,5,"ttttt");
        bin_code.replace(16,16,"iiiiiiiiiiiiiiii");
        type = dinstruction_map[bin_code].second;
        asm_code = type + " " + dregister_map[rt] + "," + dregister_map[rs] + "," + QString::number(bin2int(imm16),10);
    }
    else if(opcode == "100000" || opcode == "100011" || opcode == "100100"
            || opcode == "100001" || opcode == "100101" || opcode == "101011"
            || opcode == "101001" || opcode == "101000"){//load/store
        bin_code.replace(6,5,"sssss");
        bin_code.replace(11,5,"ttttt");
        bin_code.replace(16,16,"iiiiiiiiiiiiiiii");
        type = dinstruction_map[bin_code].second;
        asm_code = type + " " + dregister_map[rt] + "," + QString::number(bin2int(imm16),10) + "(" + dregister_map[rs] + ")";
    }
    else if(opcode == "001111"){//lui TI-type inst
        bin_code.replace(11,5,"ttttt");
        bin_code.replace(16,16,"iiiiiiiiiiiiiiii");
        type = dinstruction_map[bin_code].second;
        qDebug() << imm16 << bin2int(imm16);
        asm_code = type + " " + dregister_map[rt] + "," + QString::number(bin2int(imm16),10);
    }
    else if(opcode == "000100" || opcode == "000101"){//beq or bne I-STI
        bin_code.replace(6,5,"sssss");
        bin_code.replace(11,5,"ttttt");
        bin_code.replace(16,16,"iiiiiiiiiiiiiiii");
        type = dinstruction_map[bin_code].second;
        asm_code = type + " " + dregister_map[rs] + "," + dregister_map[rt] + "," + dislabel[pc + bin2int(imm16)*4];
    }
    else if(opcode == "000001" || opcode == "000111" || opcode == "000110"
            || opcode == "000001"){ //bgez/bgtz/blez/bltz I-SI
        bin_code.replace(6,5,"sssss");
        bin_code.replace(16,16,"iiiiiiiiiiiiiiii");
        type = dinstruction_map[bin_code].second;
        asm_code = type + " " + dregister_map[rs] + "," + dislabel[pc + bin2int(imm16)*4];
    }
    else if(opcode == "000010" || opcode =="000011"){//J-type inst
        bin_code.replace(6,26,"iiiiiiiiiiiiiiiiiiiiiiiiii");
        type = dinstruction_map[bin_code].second;
        asm_code = type + " " + dislabel[bin2int(target)];
    }
    else if(opcode == "010000"){//mfc0 or mtc0
        bin_code.replace(11,5,"ttttt");
        bin_code.replace(16,5,"ddddd");
        if(!dinstruction_map.contains(bin_code)){
            return "Invalid binary instruction.";
        }
        else{
            type = dinstruction_map[bin_code].second;
            asm_code = type + " " + dregister_map[rd] + "," + dregister_map[rt];
        }
    }
    else if(opcode == "010000"){//eret
        return "01000010000000000000000000011000";
    }
    else{
        return "Invalid binary instruction.";
    }
    return asm_code;
}

QMap<QString, QString> hexmap = {
    {"0","0000"},
    {"1","0001"},
    {"2","0010"},
    {"3","0011"},
    {"4","0100"},
    {"5","0101"},
    {"6","0110"},
    {"7","0111"},
    {"8","1000"},
    {"9","1001"},
    {"a","1010"},
    {"b","1011"},
    {"c","1100"},
    {"d","1101"},
    {"e","1110"},
    {"f","1111"},
};

QString hex2bin(QString hex_code){
    QString bin_code;
    for(int i = 0; i < 8; i++){
        bin_code += hexmap[hex_code.at(i)];
    }
    return bin_code;
}

int bin2int(QString bin_code){
    int result = 0;
    int flag = 0;
    QString temp;
    if(bin_code.at(0) == '1'){
        flag = 1;
        for(int i = 0; i < bin_code.length(); i++){
            if(bin_code.at(i) == '0')
                temp += '1';
            else temp += '0';
        }
        bin_code = temp;
    }
    qDebug() << bin_code;
    for(int i = 0; i < bin_code.length(); i++){
        if(bin_code.at(i) == '1')
            result += pow(2,bin_code.length() - 1 - i);
    }
    if(flag == 1)
        result = -result - 1;
    return result;
}

void setRegS(const QString& rs, QString& machineCode){
    machineCode.replace(6, 5, rs);
}

void setRegT(const QString& rt, QString& machineCode){
    machineCode.replace(11, 5, rt);
}

void setRegD(const QString& rd, QString& machineCode){
    machineCode.replace(16, 5, rd);
}

void setShift(const QString& shift, QString& machineCode){
    QString shift_bin = QString("%1").arg(shift.toInt(), 5, 2, QChar('0'));
    machineCode.replace(21, 5, shift_bin);
}

void setImm(const QString& imm, QString& machineCode){
    QString imm16 = QString("%1").arg(imm.toInt(), 16, 2, QChar('0'));
    if(imm16.startsWith('1')){
        imm16 = imm16.right(16);
    }
    machineCode.replace(16, 16, imm16);
}

void setTarget(const QString& target, QString& machineCode){
    QString target26 = QString("%1").arg(target.toInt(), 26, 2, QChar('0'));
    machineCode.replace(6, 26, target26);
}

