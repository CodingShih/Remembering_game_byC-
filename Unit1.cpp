//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "Unit1.h"
#include <jpeg.hpp>    // 可以讓C++選用jpeg檔的圖片
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TForm1 *Form1;
TImage *box[40];     //圖庫上限40張
TImage *boards[50];  //牌數5*5  *2(左邊Panel3和右邊Panel4)

int n=0;
int boardW,boardH;     // 牌的(Width)寬度和(Height)高度
int manyboxs;         // 宣告  產生Panel3、4上的圖
int scale,timerange; // 規模為多大，翻牌時間的限制
int finish=0;       // 翻牌正確的組數
//---------------------------------------------------------------------------
__fastcall TForm1::TForm1(TComponent* Owner)
        : TForm(Owner)
{
}
//---------------------------------------------------------------------------
void __fastcall TForm1::CLOSEClick(TObject *Sender)
{                       //結束 按鈕
  for (int i=0;i<n;i++)
  {                         //將動態物件回收，否則記憶體將一直被占用。
    delete box[i];
  }

  if(scale!=0)
  {
     for (int i=0;i<scale*scale*2;i++)
       delete boards[i];
  }
  Close();
}
//---------------------------------------------------------------------------
void __fastcall TForm1::STARTandNEWGAMEClick(TObject *Sender)
{
  finish=0;
  manyboxs=0;    //產生要顯示在Panel上的圖
  if(scale!=0)
  {
     for(int i=0;i<scale*scale*2;i++) 
       delete boards[i];         //換新的模式時，會先刪除上一個卡牌的動態物件
  }
  scale=ComboBox1->ItemIndex+2;
  boardW=Panel3->Width/scale;    //讓每張圖都能等高等寬
  boardH=Panel3->Height/scale;

  switch (timerange=ComboBox2->ItemIndex)
  {
    case 1 : timerange=60;  break;
    case 2 : timerange=120; break;
    case 3 : timerange=300; break;
  }

  for (int i=0;i<scale*scale;i++)
  {
    boards[i]=new TImage(Form1);
    boards[i]->Parent=Panel3;                // Panel3(左)
    boards[i]->Width=boardW;
    boards[i]->Height=boardH;
    boards[i]->Left= (i%scale)*boardW;
    boards[i]->Top= (i/scale)*boardH;
    boards[i]->Picture=Image1->Picture;// 屬性 Stretch開啟後可以將原本太大或太小
    boards[i]->Stretch=true;           // 的圖片比例延展至適合Image的大小。
    boards[i]->Hint=AnsiString(i);     // 存取每個位置各自的索引值(每個人的代號)
    boards[i]->OnClick=boardClick;

    boards[i+(scale*scale)]=new TImage(Form1);
    boards[i+(scale*scale)]->Parent=Panel4;  // Panel4(右)
    boards[i+(scale*scale)]->Width=boardW;
    boards[i+(scale*scale)]->Height=boardH;
    boards[i+(scale*scale)]->Left= (i%scale)*boardW;
    boards[i+(scale*scale)]->Top= (i/scale)*boardH;
    boards[i+(scale*scale)]->Picture=Image1->Picture;
    boards[i+(scale*scale)]->Stretch=true;
    boards[i+(scale*scale)]->Hint=AnsiString(i+(scale*scale)); //存取各自的索引值
    boards[i+(scale*scale)]->OnClick=boardClick;

    if (manyboxs<n)        //存取圖庫的索引值
    {
       boards[i]->Tag=manyboxs;
       boards[i+(scale*scale)]->Tag=manyboxs;
    }
    else
    {
       manyboxs=0;
       boards[i]->Tag=manyboxs;
       boards[i+(scale*scale)]->Tag=manyboxs;
    }
    manyboxs++;
  }

  randomize();
  int exchange;                 //設一個用來交換的變數
  int num;             // random number(設定用於打亂後的牌能找到同一索引值的圖)
  for (int i=0;i<5000;i++)
  {
    for (int j=0;j<scale*scale;j++)               //左邊洗牌
    {
      num=random(scale*scale);

      exchange=boards[j]->Tag;   //交換
      boards[j]->Tag=boards[num]->Tag;
      boards[num]->Tag=exchange;
    }

    for (int a=scale*scale*2;a<(scale*scale*2);a++)   //右邊洗牌
    {
      a=random(scale*scale)+scale*scale;

      exchange=boards[a]->Tag;
      boards[a]->Tag=boards[num]->Tag;
      boards[num]->Tag=exchange;
    }

  }
  Timer1->Enabled=true;    //開始後，讀取完ComboBox2開始倒數
}
//---------------------------------------------------------------------------
void __fastcall TForm1::PLUSPICTUREClick(TObject *Sender)
{
  if (OPD->Execute())        //加入圖片
  {
     for (int i=0;i<OPD->Files->Count;i++)
     {
        box[n]=new TImage(Form1);
        box[n]->Parent=Panel2;
        box[n]->Width=35;
        box[n]->Height=35;
        box[n]->Left=(n%10)*36;
        box[n]->Top=(n/10)*36;
        box[n]->Picture->LoadFromFile(OPD->Files->Strings[i]);  //打入此段以及
        box[n]->Stretch=true;                       //將在OPD的Options屬性裡的
        box[n]->Tag=n;                         //ofAllowMultiSelect開啟為true，
        box[n]->OnClick=chooseClick;           //便可以一次選取並加入多張圖。
        n++;
     }
  }

}
//---------------------------------------------------------------------------
void __fastcall TForm1::chooseClick(TObject *Sender)
{
  TImage *choose=(TImage *)Sender;
  int chooseTag=choose->Tag;
                                     //刪除圖片(滑鼠在想刪除的圖片上點擊即可)
  if (n>1 && chooseTag<n-1)
  {                                   //將要刪的圖後面一張圖複製到被刪的圖的位置
     for (int i=chooseTag;i<n-1;i++)   //因此後方如果無圖，將複製空白。
     {
        box[i]->Picture=box[i+1]->Picture;
     }
     delete box[--n];
  }
}
//---------------------------------------------------------------------------
void __fastcall TForm1::boardClick(TObject *Sender)
{
  TImage *board=(TImage *)Sender;
  int manyboxs=board->Tag;       // 讀取卡牌的Tag
  static int singlebox;          // 存入單數次的Tag
  static int singleboards;         // 存入單數次的Hint
  static bool singular=true;     // 翻牌動作為單數次，停下等待

  board->Picture=box[manyboxs]->Picture;
  Form1->Refresh();

  if (singular)
  {
     singular=false;
     singlebox=manyboxs;
     singleboards=board->Hint.ToInt();      // Hint將圖的位址對在一起
     if(board->Parent==Panel3)
     {
       Panel3->Enabled=false;
       Panel4->Enabled=true;             //翻左邊後，不能再翻左邊的牌
     }
     else                                //其他的情況
     {
       Panel3->Enabled=true;             //翻右邊後，下一個動作只能翻左邊
       Panel4->Enabled=false;
     }
  }
   else              //為雙數次時
  {
    if (singlebox!=manyboxs)    //圖庫單數次的Tag 不等於圖庫給的位址時
    {
      Sleep(1100);              //將花1.1秒判斷
      board->Picture=Image1->Picture;     //並蓋回牌面
      boards[singleboards]->Picture=Image1->Picture;
    }
    else
    {
      board->Tag=100;                       //將用不到的Tag拿來判斷翻正確的牌
      boards[singleboards]->Tag=100;
      board->Enabled=false;
      boards[singleboards]->Enabled=false;
      finish++;
    }

    singular=true;
    Panel3->Enabled=true;
    Panel4->Enabled=true;
  }

  if ( finish==scale*scale)
  {
      Timer1->Enabled=false;
      ShowMessage("恭喜完成!  太厲害了!!");
      Panel3->Enabled=false;
      Panel4->Enabled=false;
  }

}
//---------------------------------------------------------------------------
void __fastcall TForm1::Timer1Timer(TObject *Sender)
{                         //計時器
 if (timerange>0)
 {
    LA3->Caption=AnsiString(--timerange);   // --，先開始倒數再顯示
 }

 if (ComboBox2->ItemIndex==0)
 {
    Timer1->Enabled=false;
 }
 if(ComboBox2->ItemIndex!=0 && timerange==0) //如果不是無限時模式 且 時間倒數為0
    {
      ShowMessage("時間到!  遊戲結束!!");
      Panel3->Enabled=false;
      Panel4->Enabled=false;
      Timer1->Enabled=false;
    }
}
//---------------------------------------------------------------------------
void __fastcall TForm1::FormCreate(TObject *Sender)
{
   Timer1->Enabled=false;
}
//---------------------------------------------------------------------------
void __fastcall TForm1::TIPClick(TObject *Sender)
{
  for (int i=0;i<(scale*scale*2);i++)
  {                           // Tag=100 為以經翻正確的牌，就不用再動
    if (boards[i]->Tag!=100)  // Tag不=100就為還覆蓋著的牌
     boards[i]->Picture=box[(boards[i]->Tag)]->Picture;
  }
  Form1->Refresh();
  Sleep(1100);                // Image的picture存到card的picture
  for (int j=0;j<(scale*scale*2);j++)
  {
     if (boards[j]->Tag!=100)
     {
       boards[j]->Picture=Image1->Picture;
     }
  }

}
//---------------------------------------------------------------------------


void __fastcall TForm1::PAUSEClick(TObject *Sender)
{
  Timer1->Enabled=!Timer1->Enabled; // 按一下可以暫停倒數時間並讓Panel不可翻牌，
  Panel3->Enabled=!Panel3->Enabled; // 再按一次可讓時間繼續。
  Panel4->Enabled=!Panel4->Enabled;
}
//---------------------------------------------------------------------------

