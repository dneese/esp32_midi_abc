/* Функция Qb_PLAY проигрывает музыку из символьных строк стандарта АВС
аналогично оператору QBASIC PLAY
http://ru.wikipedia.org/wiki/ABC_(%D0%BD%D0%BE%D1%82%D0%BD%D0%B0%D1%8F_%D0%B7%D0%B0%D0%BF%D0%B8%D1%81%D1%8C)
Горкавчук Константин.
mailto:kgg0@bk.ru
http://gorkavchukkg.narod.ru
*/
//Нота	0 1     2 3   	4 5 6  	  7     8      	9	10  	11
//Нота	C C#/D-	D D#/E-	E F F#/G- G	G#/A-	A	A#/B?	B
int freq[7][12] = {
 {65, 69, 73, 78, 82, 87, 92, 98, 104, 110, 117, 123},                     //0 = Большая октава   
 {131, 139, 147, 156, 165, 175, 185, 196, 208, 220, 233, 247},             //1 = Малая октава
 {262, 277, 294, 311, 330, 349, 370, 392, 415, 440, 466, 494},             //2 = 1-я октава
 {523, 554, 587, 622, 659, 698, 740, 784, 831, 880, 932, 988},             //3 = 2-я октава
 {1047, 1109, 1175, 1245, 1319, 1397, 1480, 1568, 1661, 1760, 1865, 1976}, //4 = 3-я октава
 {2093, 2218, 2349, 2489, 2637, 2794, 2960, 3136, 3322, 3520, 3729, 3951}, //5 = 4-я октава
 {4186, 4435, 4699, 4978, 5274, 5588, 5920, 6272, 6645, 7040, 7459, 7902}, //6 = 5-я октава
};
int SoundPin=13;

void setup() {
  pinMode(SoundPin, OUTPUT);
  Serial.begin(9600);  
}

int extractNumber(int& myNumber, char Muz[], int& curPosition)
{
    int digitsNumber=0;
    int curDigit=0;
    myNumber=0;
    do
    {
        if ((Muz[curPosition]> 47) && (Muz[curPosition]<58)) // Коды ASCII цифр '0' == 48 , "9' == 57
        { 
            curDigit=Muz[curPosition]-48;
            digitsNumber++;
            myNumber=myNumber*10+curDigit;
        }
        else
        {
        return digitsNumber;
        }     
        curPosition++;
    }while(Muz[curPosition]!= '\0');
    return digitsNumber;
}

int pointsCount(char Muz[], int& curPosition)
{
    int pointsNumber=0;
    do
    {
        if (Muz[curPosition]== '.')
        { 
            pointsNumber++;
        }
        else
        {
        return pointsNumber;
        }     
        curPosition++;
    }while(Muz[curPosition]!= '\0');
    return pointsNumber;
}

void Qb_PLAY(char Muz[])
{
  static int generalOktava;   
  int oktava;   
  static int tempo=120; // Задание темпа или четвертных нот, которые исполняются в минуту. n от 32 до 255. По умолчанию 120
  int Nota=0; 
  int  curPosition, curNota4;
  unsigned long currentNotaPauseDuration;
  unsigned long currentNotaDuration; 
  unsigned long  pauseDuration; 
  int takt=240000/tempo; 
  bool isNota; 
  bool isPause; 
  int pointsNum=0;
  float generalNotaMultipl=0.875;
  static float NotaLong;
  float curMultipl;
  float tempFlo;
  float curPause;
  unsigned long tempLong;
  int i=0;
  do 
  {
   isNota=false; 
   isPause=false; 
   oktava=generalOktava;
   switch(Muz[i]){
         case '\0':{
             return; 
             }
             break;
         case 'C':{
             Nota=0;
             isNota=true; 
             }
             break;
         case 'D':{
             Nota=2;
             isNota=true; 
             }
             break;
         case 'E':{
             Nota=4;
             isNota=true;      
             }
             break;
         case 'F':{
             Nota=5;
             isNota=true;     
             }
             break;
         case 'G':{
             Nota=7;
             isNota=true;         
             }
             break;
         case 'A':{
             Nota=9;
             isNota=true;
             }
             break;
         case 'B':{
             Nota=11;
             isNota=true; 
             }
             break;   
         case 'N':{// Nнота	Играет определенную ноту (0 - 84) в диапазоне семи октав (0 - пауза).     
                 curPosition=i+1;
                 if (extractNumber(curNota4, Muz, curPosition)){
                     i=curPosition-1;
                     if (curNota4){
                         curNota4--;          
                         oktava=curNota4 / 12;                
                         Nota=curNota4 % 12;
                         isNota=true; 
                     }        
                     else{
                         isPause=true;
                     }
                 }                
             }
             break;   
         case 'O':{ //Oоктава	Задает текущую октаву (0 - 6). 
             curPosition=i+1;
             if (extractNumber(oktava, Muz, curPosition)){
                 i=curPosition-1;
                 generalOktava=oktava; 
                 }
             }
             break;          
         case '>':{
             generalOktava++;
             }
             break;  
         case '<':{
             generalOktava--;
             }
             break;
         case 'M':{   
              switch(Muz[i+1]){
                 case 'N':{ //MN	Нормаль. Каждая нота звучит 7/8 времени, заданного в команде L
                     generalNotaMultipl=0.875; //  =7/8
                     i++;
                     }
                     break;
                 case 'L':{ //ML	Легато. Каждая нота звучит полный интервал времени, заданного в команде L
                     generalNotaMultipl=1.0;
                     i++;
                     }
                     break;                 
                 case 'S':{ //MS	Стаккато. Каждая нота звучит 3/4 времени, заданного в команде L
                     generalNotaMultipl=0.75;  // =3/4
                     i++;
                     }
                     break;                 
                 case 'F':{ //MF Режим непосредственного исполнения. Т.е. на время проигрывания ноты программа приостанавливается. Используется по умолчанию
                     i++;   //Сдвигаем точку чтения и ничего не делаем.
                     }
                     break;   

                 case 'B':{ //MB проигрывние в буффер
                     i++;   //Сдвигаем точку чтения и ничего не делаем.
                     }
                     break;   
               }
             }
             break;             
         case 'L':{ //Lразмер	Задает длительность каждой ноты (1 - 64). L1 - целая нота, L2 - 1/2 ноты и т.д.    
             curPosition=i+1;
             if (extractNumber(curNota4, Muz, curPosition)){
                 i=curPosition-1;
                 tempFlo=float(curNota4);
                 NotaLong=1/tempFlo;
                 }             
             }
             break;
         case 'T':{ //Tтемп	Задает темп исполнения в четвертях в минуту (32-255).По умолчанию 120
             curPosition=i+1;
             if (extractNumber(tempo, Muz, curPosition)){
                i=curPosition-1;
                takt=240000/tempo; // миллисекунд на 1 целую ноту. 240000= 60 сек * 1000 мсек/сек *4 четвертей в ноте
                }             
             }
             break;
         case 'P':{ //Pпауза	Задает паузу (1 - 64). P1 - пауза в целую ноту, P2 - пауза в 1/2 ноты и т.д.
             curPosition=i+1;
             if (extractNumber(curNota4, Muz, curPosition)){
                 tempFlo=float(curNota4);
                 curPause=1/tempFlo;
                 i=curPosition-1;
                 isPause=true;
                 }
             }
             break;              
         case ' ':{ //Есть в некоторых текстах. Вероятно это пауза длительностью в текущую ноту
             curPause= NotaLong;
             isPause=true;
             }
             break;  
    }          
    if (isNota){
      switch(Muz[i+1]){
         case '#':{ // диез
             Nota++;
             i++;
             }
             break;
         case '+':{ // диез
             Nota++;
             i++;
             }
             break;                 
         case '-':{ // бемоль
           Nota--;
           i++;
          }
          break;   
       }
       curPosition=i+1;
       if (extractNumber(curNota4, Muz, curPosition)){
         currentNotaDuration=takt/curNota4;
         i=curPosition-1;
         }
     }
     if (oktava<0) oktava=0; 
     if (oktava>6) oktava=6; 
     if (isNota || isPause){ 
       curPosition=i+1;
       pointsNum=pointsCount(Muz, curPosition);  
       if (pointsNum) i=curPosition-1;   
       curMultipl=1.0;                
       for (int j=1; j<=pointsNum; j++) {
          curMultipl= curMultipl * 1.5;
       }   
       currentNotaPauseDuration=(takt*NotaLong);
     } 
     if (isNota){
        curMultipl=curMultipl*generalNotaMultipl;
        currentNotaDuration= (currentNotaPauseDuration*curMultipl); 
        if (Nota<0) Nota=0; 
        if (Nota>11) Nota=11; 
        tempLong= freq[oktava][Nota];
        tone(SoundPin,tempLong,currentNotaDuration);     
        delay(currentNotaPauseDuration);
     } 
     if (isPause){
         pauseDuration=takt*curPause*curMultipl;
         delay(pauseDuration);     
     } 
    i++;          
  } while (Muz[i]!= '\0');   
}

void loop() {
// // Jingle Bells

// Qb_PLAY ("MNT255L4O2DBAGL2D.L8DDL4DBAGL2E.L4EE>C<BAL1F+L4>DDC<A");
// Qb_PLAY ("L1BL4DBAGL2D.L8DDL4DBAGL2E.L4EE>C<BA>DDDDEDC<AL2G.P4L4B");
// Qb_PLAY ("BL2BL4BBL2BL4B>D<G.L8AL1BL4>CCC.L8CL4C<BBL8BBL4BAABL2A");
// Qb_PLAY (">DL4<BBL2BL4BBL2BL4B>D<G.L8AL1BL4>CCC.L8CL4C<BBL8BBL4>D");
// Qb_PLAY ("DC<AL1G");


// // REM We Wish You a Merry Christmas
// Qb_PLAY ("MNT150L4O3CFL8FGFEL4DDDGL8GAGFL4ECCAL8AA+AGL4FDL8CCL4D");
// Qb_PLAY ("GEL2FL4CFFFL2EL4EFEDL2CL4CAGF>C<CL8CCL4DGEL2FL4CFL8FG");
// Qb_PLAY ( "FEL4DDDGL8GAGFL4ECCAL8AA+AGL4FDL8CCL4DGEL2F");


// //  KATIUSHA

// Qb_PLAY ("MST255L2O2E.L4F+L2G.L4EGGF+EL2F+L4<BP4L2>F+.L4GL2A.L4F+");
// Qb_PLAY ("AAGF+L1EL2B>EDL4EDCC<BAL2BEP4>CL4<AL2B.L4GF+<B>GF+L1E");
// Qb_PLAY ("L2B>EDL4EDCC<BAL2BEP4>CL4<AL2B.L4GF+<B>GF+L1E");

// Bach 1
Qb_PLAY ("MNT150L8O3AL16>C<AL8EL16AEL8CL16ECL4<AL16EA>C<ABABAG+");
Qb_PLAY ("B>D<B>C<B>C<AL8>AL16>C<AL8EL16AEL8CL16ECL4<AL16>C<BL8>C");
Qb_PLAY ("L16C<BL8>CAL16C.L32DL8C<BL16>ED+L8EL16ED+L8E>CL16<E.L32F");
Qb_PLAY ("L8ED+L16<B>EGEF+EF+ED+F+AF+GF+GF+EGED+EAED+EBED+E>C<E");
Qb_PLAY ("D+E>C<BABGF+EL8GL32F+GL16F+L4EL8AL16>C<AL8EL16AEL8CL16E");
Qb_PLAY ("CL4<AL16EA>C<ABABAG+B>D<B>C<B>C<AL8>AL16>C<AL8EL16AEL8C");
Qb_PLAY ("L16ECL4<AL16>C<BL8>CL16C<BL8>CAL16C.L32DL8C<BL16>ED+L8E");
Qb_PLAY ("L16ED+L8E>CL16<E.L32FL8ED+L16<B>EGEF+EF+ED+F+AF+GF+GF+");
Qb_PLAY ("EGED+EAED+EBED+E>C<ED+E>C<BABGF+EL8GL32F+GL16F+L4EL8E");
Qb_PLAY ("L16GEL8<BL16>E<BL8GL16BGL4EL8AG+>DC+GL16FEL8FDFL16AFL8D");
Qb_PLAY ("L16FDL8<BL16>D<BL4GL8>CL16ECDCDC<B>DFDEDEDCEC<B>CFC<B");
Qb_PLAY (">CGC<B>CAC<B>CAGFGEDCL8EL32DEL16DL4CL16EDL8EL16EDL8E>C");
Qb_PLAY ("<EL64FL8EDL16DCL8DL16DCL8DBDL64EL8DCAL16>C<AGL4F.L16A");
Qb_PLAY ("FEL4D.L16FDCL4<A+L16A+AA+AL4G+L8FEAG+>DL16C<B>C<A>CEL8A");
Qb_PLAY ("EL16DC<B>CL4<AL8>EL16GEL8<BL16>E<BL8GL16BGL4EL8AG+>DC+");
Qb_PLAY ("GL16FEL8FDFL16AFL8DL16FDL8<BL16>D<BL4GL8>CL16ECDCDC<B");
Qb_PLAY (">DFDEDEDCEC<B>CFC<B>CGC<B>CAC<B>CAGFGEDCL8EL32DEL16DL4C");
Qb_PLAY ("L16EDL8EL16EDL8E>C<EL64FL8EDL16DCL8DL16DCL8DBDL64EL8D");
Qb_PLAY ("CAL16>C<AGL4F.L16AFEL4D.L16FDCL4<A+L16A+AA+AL4G+L8FEA");
Qb_PLAY ("G+>DL16C<B>C<A>CEL8AEL16DC<B>CL4<A");

//REM Moonlight Sonata 1

  Qb_PLAY("MLT100L8O1C+O4C+E<G+>C+E<G+>C+E<G+>C+EO1BO4C+E<G+>C+E");
  Qb_PLAY("<G+>C+E<G+>C+EO1AO4C+E<A>C+EO1F+O4DF+<A>DF+O1G+O4CF+<G+");
  Qb_PLAY(">C+EO1G+O4C+D+<F+>CD+O1C+O3G+>C+<G+>C+E<G+>C+EG+C+L16E");
  Qb_PLAY("G+P64L8G+D+F+<G+>D+F+<G+>D+F+G+D+L16F+G+P64L8G+C+E<G+");
  Qb_PLAY(">C+EAC+F+<A>C+F+G+<B>E<G+B>EF+<B>D+B<B>D+E<B>E<G+B>E<G+");
  Qb_PLAY("B>E<G+B>EO2E>B>E<GB>E<GB>EG<BL16>EGP64L8G<B>F<GB>F<GB");
  Qb_PLAY(">FG<BL16>FGP64L8GCE<GB>E<G>C+EF+C+EF+<B>D<F+B>DG<B>C+");
  Qb_PLAY("E<B>C+F+<B>D<F+B>DF+<A+>E<F+A+>C+O1BO4DF+<B>DF+<B>D+F+");
  Qb_PLAY("BD+F+>C<EG<E>EG<G>EGA+EGBD+F+<B>D+F+<B>D+F+BD+F+>C<EG");
  Qb_PLAY("<E>EG<G>EGA+EGBD+F+<B>D+F+BDF<B>DFBC+G+<B>C+G+AC+F+<A");
  Qb_PLAY(">C+F+G<B>D<GB>DF+<A>D+<F+A>D+C+<F+AC+F+A>C+<F+G+C+FG+");
  Qb_PLAY("F+A>C+<A>C+F+C+F+A>C+<F+L16A>C+P64L8C+<G+BC+G+BC+G+B>C+");
  Qb_PLAY("<G+L16B>C+P64L8C+<F+AC+F+A>C<F+A>C+<F+A>D+<F+G+D+F+G+");
  Qb_PLAY("D+F+G+>D+<F+G+>E<G+>C+<EG+>C+D+<F+A>C+<EA+>C<CD+G+CD+");
  Qb_PLAY("ACD+F+CD+O1G+O4CD+<G+>CD+<A>CD+<F+>CD+O1G+O4EG+>C+<EG+");
  Qb_PLAY(">E<EG+>C+<EG+O1G+O3EG+>C+<EG+>E<EG+>C+<EG+<G+>AF+>C<A");
  Qb_PLAY(">D+CF+D+AF+>CO3E>C+<G+>EC+G+E>C+<G+>EC+<G+C+GEA+G>C+<A+");
  Qb_PLAY(">EC+GEA+<F+>C<A>D+CF+D+AF+CAD+CF+AD+F+CD+<A>C<F+AD+F+");
  Qb_PLAY("CD+<A>C<F+AD+F+C+F+A<G+>F+G+AG+F+D+F+AC+F+A<G+>F+G+AG+");
  Qb_PLAY("F+DF+AC+F+A<G+>F+G+AG+F+DF+AC+F+ACF+G+AG+F+C+E>C+<C+D");
  Qb_PLAY(">C+<D+A>C+<D+A>C+<D+G+>C<D+F+>C<EG+>C+<G+>C+E<G+>C+EG+");
  Qb_PLAY("C+L16EG+P64L8G+D+F+<G+>D+F+<G+>D+F+G+D+L16F+G+P64L8G+");
  Qb_PLAY("C+E<G+>C+EAC+F+<A>C+F+G+<B>E<G+B>EF+<B>D+B<B>D+E<B>E<B");
  Qb_PLAY(">EG+<B>EG+BEL16G+BP64L8BF+A<B>F+A<B>F+ABF+L16ABP64L8B");
  Qb_PLAY("EG+<B>EG+>C<F+G+>C+<EG+F+G+>C+<EG+>D+<F+G+D+F+G+>E<G+");
  Qb_PLAY(">C+<EG+>C+D<F+ADF+A>C<F+G+<B>F+G+>C+<EG+C+EG+C+FG+>C+");
  Qb_PLAY("<FG+>D<F+AC+F+AC+F+A>C<F+A>C+<FG+C+FG+C+FG+>C+<FG+>D<F+");
  Qb_PLAY("AC+F+AC+F+A>C<F+A>C+<FG+C+FG+>C+<F+AC+F+ABF+A<B>F+A<B");
  Qb_PLAY(">F+ABEG+AEG+AD+F+G+D+F+G+C+EF+C+D+<G+>C+D+G+C+D+AC+D+");
  Qb_PLAY("G+C+E<G+>C+EG+CD+<G+>CD+C+<G+>C+<G+>C+E<G+>C+EO2G+O4C+");
  Qb_PLAY("L16EO2G+L8CO4D+F+<G+>D+F+<G+>D+F+O2G+O4D+L16F+O2G+L8C+");
  Qb_PLAY("O4EC+G+E>C+<G+>EC+O2G+O5EL16C+O2G+L8<G+O5D+<A<C>F+AD+");
  Qb_PLAY("F+<A<G+>G+L16F+<G+L8C+O4EC+G+E>C+<G+>EC+O2G+O5EL16C+O2G+");
  Qb_PLAY("L8<GO5CD+<A>C<F+AD+F+<A<G+>G+L16F+<G+L8C+>G+>C+EC+<G+");
  Qb_PLAY("C+EG+>C+<G+E<C+>C+EG+EC+<G+>C+<G+EG+EL2C+P64C+P64L1C+");
  Qb_PLAY("P4");
  delay(10000); // Пауза перед повтором


  Qb_PLAY("MNT200L16O1G+>C+EG+C+EG+>C+<EG+>C+E<G+>C+EG+C+EG+>C+<EG+>C+E<G+>C+EL8G+G+P16L16O1G+>CD+G+CD+G+>C<D+G+>CD+<G+CD+>G+CD+G+>C<D+G+>CD+<G+>CD+L8G+G+L16O2C+FG+>C+<FG+>C+F<G+>C+FG+C+FG+>C+<FG+>C+F<G+>C+FG+C+FG+L8>C+C+P16L16O2C+F+A>C+C+F+A>C+C+F+AL8>C+C+P16L16O2C+EG>C+C+EG>C+C+EGL8>C+C+CL16O3G+>G+<G+>G+<A+>G+CG+C+G+D+G+CG+D+G+C+G+F+G+EG+D+G+C+G+CG+<A>G<G+>G+<G+>G+<G+>G+<A+>G+CG+C+G+D+G+CG+D+G+C+G+F+G+EG+D+G+C+G+CG+<A>G<G+>G+<A>G<G+>G+<A>G<G+>G+<A>G<G+>G+<A>GL4G+L2O2G+.P16L16<G+>C+EG+C+EG+>C+<EG+>C+E<G+>C+EG+C+EG+>C+<EG+>C+EEG+>C+L8EEP16L16O2EG>C+E<G>C+EGC+EG>C+<EG>C+E<G>C+EGC+EG>C+<EG>C+L8EEP16L16O2D+A+>C+D+<A>C+D+A+C+D+A+>C+<D+A+>C+D+<A+>C+D+A+C+D+A+>C+<A+D+C+A+D+C+<A+L4BL2>D+L8<B.L16G+L32GG+A+L8G+.L4G.L8G>D+.L16<GL4A+G+.L8G+>D+.L16<G+L4BA+.L8A+>D+.L16<A+L8BL4O5D+D+L8D+<BG+G+L4GGL8G>D+<GA+L4G+G+L8G+>D+<G+BL4A+A+L8A+>D+<A+L2>CC+L4<A+L2B.G+AL64FL32G.L16FGFGL2G+.AL16AEF+G+AB>C+<BAEF+G+AB>C+<BAEF+G+AB>C+<BAEF+G+AB>C+DED+ED+EC+<BAL2G+L64BL32G+.L16A+G+A+G+A+G+A+L4G+<AL16AEF+G+AB>C+<BAEF+G+AB>C+<BAEF+G+AB>C+<BAEF+G+AB>C+<BAEF+G+AB>C+<BAEF+G+AB>C+DEF+G+AB>C+DFL2E<G+B<GL8G+BBBBBA+G+G>D+D+D+D+D+D+D+D+<BBBBBA+G+G>D+D+D+D+D+D+D+D+D+D+<BP8>EEC+P8D+D+<BP8>D+D+<A+B>BBBBBA+G+G>EEEEED+C+<BBBBBBA+G+G>EEEEED+C+<BBB>D+P8C+C+EP8<BB>D+P8<A+A+>D+P8<BB>D+P8<AA>C+P8<G+G+BP8GGA+L4G+P4P4L8<B.L16G+L4G>D+D+D+L32O2B>G+L8>D+.L2D+L8B.L16G+L4G>D+D+D+L32O3B>G+L8>D+.L4D+L32<C+GL8>D+.L4D+L32O3B>G+L8>D+.L4D+L32<C+GL8>D+.L4D+L16O2B>G+D+G+<B>G+D+G+<B>G+D+G+<B>G+D+G+CF+D+F+CF+D+F+CF+D+F+CF+D+F+EO1G+>C+EG+C+EG+>C+<EG+>C+E<G+>C+EG+C+EG+>C+<EG+>C+E<G+>C+EL8G+G+P16L16O1G+>CD+G+CD+G+>C<D+G+>CD+<G+CD+>G+CD+G+>C<D+G+>CD+<G+>CD+L8G+G+L16O2C+FG+>C+<FG+>C+F<G+>C+FG+C+FG+>C+<FG+>C+F<G+>C+FG+C+FG+L8>C+C+P16L16O2C+F+A>C+C+F+A>C+C+F+AL8>C+C+P16L16O2C+G+B>C+C+G+B>C+C+G+BL8>C+C+L4C+L2<C+L8<A.L16F+L32FF+G+L8F+.L4F.L8F>C+.L16<FL4G+F+.L8F+>C+.L16<F+L4AG+.L8G+>C+.L16<G+A>C+<A>C+<F+>C+<A>C+<F+>C+<A>C+<F+>C+<A>C+<G+>C+<B>C+<G+>C+<B>C+<G+>C+<B>C+<G+>C+<B>C+<F+>C+<A>C+<F+>C+<A>C+<F+>C+<A>C+<F+>C+<A>C+<F+>D<A>D<F+>D<A>D<A>DCD<A>DCD<G>D<B>D<G>D<B>D<G>D<B>D<G>D<B>D<A>DCD<A>DCD<A>DCD<A>DCD<G>D<B>D<G>D<B>D<G>D<B>D<G>D<B>D<G+>D<B>D<G+>C+<B>C+<G+>C+<B>C+<G+>C+<B>C+<F+>C+<A>C+<F+>C+<A>C+<F+>F+<A>F+<F+>F+<A>F+<F+>F+D+F+<F+>F+D+F+<F+>F+D+F+<F+>F+D+F+<E>EC+E<E>EC+E<E>EC+E<E>EC+E<D+>D+C+D+<D+>D+C+D+<D+>D+C+D+<D+>D+C+D+L4D+L2<G+L8AG+L4F+L8G+F+L4EL8F+EL4D+L8ED+L4C+L8D+C+L2<B>C+L4D+>G+.L8G+AG+F+F+G+F+EEF+ED+D+ED+C+C+D+C+L2CC+L32<CF+L8>D+.L2D+L4C+CCC+C+L32<CF+L8>D+.L2D+L8<E>C+<F+>C<F+>C<E>C+<E>C+<F+>D+<F+>D+<E>C+<E>C+L32O2A>C+D+AL8AL2A.L32<G+>CD+G+L8G+L2G+.P16L16O1G+>C+EG+C+EG+>C+<EG+>C+E<G+>C+EG+C+EG+>C+<EG+>C+E<G+>C+EL8G+G+P16L16O1G+>CD+G+CD+G+>C<D+G+>CD+<G+>CD+G+CD+G+>C<D+G+>CD+<G+>CD+L8G+G+P16L16O2C+FG+>C+<FG+>C+F<G+>C+FG+C+FG+>C+<FG+>C+F<G+>C+FG+C+FG+L8>C+C+P16L16O2C+F+A>C+C+F+A>C+C+F+AL8>C+C+P16L16O2C+EG>C+C+EG>C+C+EGL8>C+C+CL16O3G+>G+<G+>G+<A+>G+CG+C+G+D+G+CG+D+G+C+G+F+G+EG+D+G+C+G+CG+<A>G<G+>G+<G+>G+<G+>G+<A+>G+CG+C+G+D+G+CG+D+G+C+G+F+G+EG+D+G+C+G+CG+<A>G<G+>G+<A>G<G+>G+<A>G<G+>G+<A>G<G+>G+<A>GL4G+L2O2G+.P4>G+L8E.L16C+L32CC+D+L8C+.L4C.L8CG+.L16CL4D+C+.L8C+G+.L16C+L4ED+.L8D+G+.L16D+L8EL4>G+G+L8G+EC+C+L4CCL8CG+CD+L4C+C+L8C+G+C+EL4D+D+L8D+G+D+L2FF+L16C+D+C+D+L2E.C+DL16<A+>C<A+>CL2C+.DL16D<AB>C+DEF+ED<AB>C+DEF+ED<AB>C+DEF+ED<AB>C+DEF+GAG+BAGF+EDL2C+L64EL32C+.L16D+C+D+C+D+C+D+L4C+<DL16D<AB>C+DEF+ED<AB>C+DEF+ED<AB>C+DEF+G+F+D<AB>C+DEF+GAB>C+DEF+GG+L2AC+E<CL8C+EEEEED+C+CG+G+G+G+G+G+G+G+EEEEED+C+CG+G+G+G+G+G+G+G+G+G+EP16AAF+P16G+G+EP16G+G+D+G+>EEEEED+C+C>CCCCCC+D+E<EEEEED+C+C>CCCCCC+D+EC+C+<G+P16>C+C+<AP16G+G+EP16G+G+D+P16G+G+EP16F+F+DP16EEC+P16D+D+CL4C+P4P4L8<E.L16C+L4CG+G+G+L32<E>C+L8G+.L2G+L8>E.L16C+L4CG+G+G+L32<E>C+L8G+.L4G+L32<F+>CL8G+.L4G+L32<E>C+L8G+.L4G+L32<F+>CL8G+.L4G+L64<E>C+L32G+L16<EG+>C+E<G+>C+EG+C+EG+>C+<EG+>C+EC+<G+E>C+<G+EC+G+EC+<G+>EC+<G+E>C+<G+EC+G+EC+<G+>EC+<G+E>C+<G+EG+L4C+P4O5C+P4<C+");
  delay(10000); // Пауза перед повтором

  
  Qb_PLAY("MNT150L32O2EL4>EL16E.L32<E>E<E>E<EL4>EL16E.L32<E>E<E>E<E>EL16<D+DC+C<BA+AG+GF+FED+DC+L32>F+L4>F+L16F+.L32<F+>F+<F+>F+<F+L4>F+L16F+.L32<F+>F+<F+>F+<F+>F+L16<FED+DC+C<BA+AG+GF+FED+L32>GL4>GL16GL32O1B>F>G<F>G<FL4>GL16GL32O1B>F>G<F>GO1B>GFG+<B>AFA+<G>BF>CO1BO3C+<F>DO1GO3D+P32E<F>FP32F+O1BO3GP32G+O1GO3AP32BO1CO4C.P32O1GO3B.P32GA+P32BO1GO3A+P32AO1CO3G+.P32O1GO3G.P32GL16F+.L32O1GO3G.P32O1CO3A.P32O1GO3G+.P32GGP32G+O1GO3GP32F+O1CO3F.P32O1GO3E.P32GL16D+.L32O1GO3E.P32BG.P32O1GO3FP32FGL16C+.L32O1GO3D.P32BG.P32O1GO3FP32FGL16C+.L32O1GO3D.P32O1F>BP32>CO1EO3C+P32DO1DO3D+P32EO1CO3FP32F+BGP32G+AAP32BGL16A.L32BG.P32O1CO4C.P32O1GO3B.P32GA+P32BO1GO3A+P32AO1CO3G+.P32O1GO3G.P32GL16F+.L32O1GO3G.P32O1CO3A.P32O1GO3G+.P32GGP32G+O1GO3GP32F+O1CO3F.P32O1GO3E.P32GL16D+.L32O1GO3E.P32BD+.P32O1F+O3D+P32D+O1D+L16O3F+.L32O1F+>B.P32<EO3GP32AO1GO3GP32F+O1EL16O3E.L32O1GL16O3G.L32BL16B.L32O1BO3BO1BO3BO1BL16O3BP32L32O1BL16O3BP32L32O1BO3BO1A+O3BO1AO3BO1G+O3BO1GO3BO1F+O3BO1FO3BO1DP32CO4C.P32O1GO3B.P32GA+P32BO1GO3A+P32AO1CO3G+.P32O1GO3G.P32GL16F+.L32O1GO3G.P32O1CO3A.P32O1GO3G+.P32GGP32G+O1GO3GP32F+O1CO3F.P32O1GO3E.P32GL16D+.L32O1GO3E.P32BG.P32O1GO3FP32FGL16C+.L32O1GO3D.P32BG.P32O1GO3FP32FGL16C+.L32O1GO3D.P32O1F>BP32>CO1EO3C+P32DO1DO3D+P32EO1CO3FP32F+BGP32G+AAP32BGL16A.L32BG.P32O1CO4C.P32O1GO3B.P32GA+P32BO1GO3A+P32AO1CO3G+.P32O1GO3G.P32GL16F+.L32O1GO3G.P32O1CO3A.P32O1GO3G+.P32GGP32G+O1GO3GP32F+O1CO3F.P32O1GO3E.P32GL16D+.L32O1GO3E.P32O1DO3D.P32O1F+O3DP32DO1DL16O3A.L32O1F+O3D.P32O1C+O3D+.P32O1GO3D+P32D+O1C+L16O3A+.L32O1GP16DO3BP32>DO1GO4C+P32CO1DO3B.P32<C>A.P32O1BL16O3G.L32O1G>G<G>G<G>G<F>F<E>E<D>D<CL16>C.L32G>E<G>E<GL16>EL32O1G>GL16>E.L32O1CL16>C.L64<B>BL32>EO1C>C<C+>C+<D>DL64<D+>D+L32>EO1E>E<FL16>FL32GB>F<B>F<BL16>FL64O1E>EL32BL16>F.L32O1DL16>DL32G>DFDFDL16FL32<B>DL16F.L32O1AL16>A.L32>DFDFDL16FL32O1G>GL16>F.L32O1F>F<G>GL64<F>FL32>FO1E>E<D>D<E>EL64<F>FL32>FO1D>D<CL16>C.L32G>E<G>E<GL16>EL32<EGL16>E.L32GL16O1G.L32O3CECECL16EL32<G>CL16E.L32O1CL16>C.L32G>E<G>E<GL16EL32G<GL16O3E.L32O1CL16>C.L64B<BL32O3EO1C>C<D>D<E>EL64<F>FL32>EO1G>G<F+L16>F+.L32>CF+CF+CL16F+L32<A>CL16F+.L32O1CL16>CL32A>D+F+<A>F+O1BL16O3F+L32<ACL16>F+.L32O1EL16>E.L32>CGCGO1CL16>C.L32>EGEGO1DL16>D.L32>DGDGGL16O1G.L32O3FGFGO1CL16O3C.L32B<B<A>A<G>G<F>F<E>E<D>D<CL8>C.P32L32GL8<G.P32L32CL16>C.L32G>E<G>E<GL16>EL32O1G>GL16>E.L32O1CL16>C.L64<B>BL32>EO1C>C<C+>C+<D>DL64<D+>D+L32>EO1E>E<FL16>FL32GB>F<B>F<BL16>FL64O1F>FL32BL16>F.L32O1DL16>DL32G>DFDFDL16FL32<B>DL16F.L32O1AL16>A.L32>DFDFDL16FL32O1G>GL16>F.L32O1F>F<G>GL64<F>FL32>FO1E>E<D>D<E>EL64<F>FL32>FO1D>D<CL16>C.L32G>E<G>E<GL16>EL32<EGL16>E.L32GL16O1G.L32O3CECECL16EL32<G>CL16E.L32O1CL16>C.L32G>E<G>E<GL16EL32G<GL16O3E.L32O1CL16>C.L64B<BL32O3EO1C>C<D>D<E>EL64<F>FL32>EO1G>G<F+L16>F+.L32>CF+CF+CL16F+L32<A>CL16F+.L32O1CL16>CL32A>D+F+<A>F+O1BL16O3F+L32<ACL16>F+.L32O1E");
  delay(10000); // Пауза перед повтором
}




 
