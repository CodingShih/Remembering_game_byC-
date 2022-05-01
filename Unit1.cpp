//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "Unit1.h"
#include <jpeg.hpp>    // �i�H��C++���jpeg�ɪ��Ϥ�
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TForm1 *Form1;
TImage *box[40];     //�Ϯw�W��40�i
TImage *boards[50];  //�P��5*5  *2(����Panel3�M�k��Panel4)

int n=0;
int boardW,boardH;     // �P��(Width)�e�שM(Height)����
int manyboxs;         // �ŧi  ����Panel3�B4�W����
int scale,timerange; // �W�Ҭ��h�j�A½�P�ɶ�������
int finish=0;       // ½�P���T���ռ�
//---------------------------------------------------------------------------
__fastcall TForm1::TForm1(TComponent* Owner)
        : TForm(Owner)
{
}
//---------------------------------------------------------------------------
void __fastcall TForm1::CLOSEClick(TObject *Sender)
{                       //���� ���s
  for (int i=0;i<n;i++)
  {                         //�N�ʺA����^���A�_�h�O����N�@���Q�e�ΡC
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
  manyboxs=0;    //���ͭn��ܦbPanel�W����
  if(scale!=0)
  {
     for(int i=0;i<scale*scale*2;i++) 
       delete boards[i];         //���s���Ҧ��ɡA�|���R���W�@�ӥd�P���ʺA����
  }
  scale=ComboBox1->ItemIndex+2;
  boardW=Panel3->Width/scale;    //���C�i�ϳ��൥�����e
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
    boards[i]->Parent=Panel3;                // Panel3(��)
    boards[i]->Width=boardW;
    boards[i]->Height=boardH;
    boards[i]->Left= (i%scale)*boardW;
    boards[i]->Top= (i/scale)*boardH;
    boards[i]->Picture=Image1->Picture;// �ݩ� Stretch�}�ҫ�i�H�N�쥻�Ӥj�ΤӤp
    boards[i]->Stretch=true;           // ���Ϥ���ҩ��i�ܾA�XImage���j�p�C
    boards[i]->Hint=AnsiString(i);     // �s���C�Ӧ�m�U�۪����ޭ�(�C�ӤH���N��)
    boards[i]->OnClick=boardClick;

    boards[i+(scale*scale)]=new TImage(Form1);
    boards[i+(scale*scale)]->Parent=Panel4;  // Panel4(�k)
    boards[i+(scale*scale)]->Width=boardW;
    boards[i+(scale*scale)]->Height=boardH;
    boards[i+(scale*scale)]->Left= (i%scale)*boardW;
    boards[i+(scale*scale)]->Top= (i/scale)*boardH;
    boards[i+(scale*scale)]->Picture=Image1->Picture;
    boards[i+(scale*scale)]->Stretch=true;
    boards[i+(scale*scale)]->Hint=AnsiString(i+(scale*scale)); //�s���U�۪����ޭ�
    boards[i+(scale*scale)]->OnClick=boardClick;

    if (manyboxs<n)        //�s���Ϯw�����ޭ�
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
  int exchange;                 //�]�@�ӥΨӥ洫���ܼ�
  int num;             // random number(�]�w�Ω󥴶ë᪺�P����P�@���ޭȪ���)
  for (int i=0;i<5000;i++)
  {
    for (int j=0;j<scale*scale;j++)               //����~�P
    {
      num=random(scale*scale);

      exchange=boards[j]->Tag;   //�洫
      boards[j]->Tag=boards[num]->Tag;
      boards[num]->Tag=exchange;
    }

    for (int a=scale*scale*2;a<(scale*scale*2);a++)   //�k��~�P
    {
      a=random(scale*scale)+scale*scale;

      exchange=boards[a]->Tag;
      boards[a]->Tag=boards[num]->Tag;
      boards[num]->Tag=exchange;
    }

  }
  Timer1->Enabled=true;    //�}�l��AŪ����ComboBox2�}�l�˼�
}
//---------------------------------------------------------------------------
void __fastcall TForm1::PLUSPICTUREClick(TObject *Sender)
{
  if (OPD->Execute())        //�[�J�Ϥ�
  {
     for (int i=0;i<OPD->Files->Count;i++)
     {
        box[n]=new TImage(Form1);
        box[n]->Parent=Panel2;
        box[n]->Width=35;
        box[n]->Height=35;
        box[n]->Left=(n%10)*36;
        box[n]->Top=(n/10)*36;
        box[n]->Picture->LoadFromFile(OPD->Files->Strings[i]);  //���J���q�H��
        box[n]->Stretch=true;                       //�N�bOPD��Options�ݩʸ̪�
        box[n]->Tag=n;                         //ofAllowMultiSelect�}�Ҭ�true�A
        box[n]->OnClick=chooseClick;           //�K�i�H�@������å[�J�h�i�ϡC
        n++;
     }
  }

}
//---------------------------------------------------------------------------
void __fastcall TForm1::chooseClick(TObject *Sender)
{
  TImage *choose=(TImage *)Sender;
  int chooseTag=choose->Tag;
                                     //�R���Ϥ�(�ƹ��b�Q�R�����Ϥ��W�I���Y�i)
  if (n>1 && chooseTag<n-1)
  {                                   //�N�n�R���ϫ᭱�@�i�Ͻƻs��Q�R���Ϫ���m
     for (int i=chooseTag;i<n-1;i++)   //�]�����p�G�L�ϡA�N�ƻs�ťաC
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
  int manyboxs=board->Tag;       // Ū���d�P��Tag
  static int singlebox;          // �s�J��Ʀ���Tag
  static int singleboards;         // �s�J��Ʀ���Hint
  static bool singular=true;     // ½�P�ʧ@����Ʀ��A���U����

  board->Picture=box[manyboxs]->Picture;
  Form1->Refresh();

  if (singular)
  {
     singular=false;
     singlebox=manyboxs;
     singleboards=board->Hint.ToInt();      // Hint�N�Ϫ���}��b�@�_
     if(board->Parent==Panel3)
     {
       Panel3->Enabled=false;
       Panel4->Enabled=true;             //½�����A����A½���䪺�P
     }
     else                                //��L�����p
     {
       Panel3->Enabled=true;             //½�k���A�U�@�Ӱʧ@�u��½����
       Panel4->Enabled=false;
     }
  }
   else              //�����Ʀ���
  {
    if (singlebox!=manyboxs)    //�Ϯw��Ʀ���Tag ������Ϯw������}��
    {
      Sleep(1100);              //�N��1.1��P�_
      board->Picture=Image1->Picture;     //�û\�^�P��
      boards[singleboards]->Picture=Image1->Picture;
    }
    else
    {
      board->Tag=100;                       //�N�Τ��쪺Tag���ӧP�_½���T���P
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
      ShowMessage("���ߧ���!  �ӼF�`�F!!");
      Panel3->Enabled=false;
      Panel4->Enabled=false;
  }

}
//---------------------------------------------------------------------------
void __fastcall TForm1::Timer1Timer(TObject *Sender)
{                         //�p�ɾ�
 if (timerange>0)
 {
    LA3->Caption=AnsiString(--timerange);   // --�A���}�l�˼ƦA���
 }

 if (ComboBox2->ItemIndex==0)
 {
    Timer1->Enabled=false;
 }
 if(ComboBox2->ItemIndex!=0 && timerange==0) //�p�G���O�L���ɼҦ� �B �ɶ��˼Ƭ�0
    {
      ShowMessage("�ɶ���!  �C������!!");
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
  {                           // Tag=100 ���H�g½���T���P�A�N���ΦA��
    if (boards[i]->Tag!=100)  // Tag��=100�N�����л\�۪��P
     boards[i]->Picture=box[(boards[i]->Tag)]->Picture;
  }
  Form1->Refresh();
  Sleep(1100);                // Image��picture�s��card��picture
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
  Timer1->Enabled=!Timer1->Enabled; // ���@�U�i�H�Ȱ��˼Ʈɶ�����Panel���i½�P�A
  Panel3->Enabled=!Panel3->Enabled; // �A���@���i���ɶ��~��C
  Panel4->Enabled=!Panel4->Enabled;
}
//---------------------------------------------------------------------------

