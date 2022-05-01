//---------------------------------------------------------------------------

#ifndef Unit1H
#define Unit1H
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ExtCtrls.hpp>
#include <Dialogs.hpp>
#include <ExtDlgs.hpp>
#include <jpeg.hpp>
//---------------------------------------------------------------------------
class TForm1 : public TForm
{
__published:	// IDE-managed Components
        TPanel *Panel1;
        TGroupBox *GroupBox1;
        TLabel *LA1;
        TLabel *LA2;
        TLabel *LA3;
        TComboBox *ComboBox1;
        TComboBox *ComboBox2;
        TButton *STARTandNEWGAME;
        TButton *CLOSE;
        TButton *PLUSPICTURE;
        TOpenPictureDialog *OPD;
        TTimer *Timer1;
        TPanel *Panel2;
        TPanel *Panel3;
        TPanel *Panel4;
        TImage *Image1;
        TButton *TIP;
        TButton *PAUSE;
        void __fastcall CLOSEClick(TObject *Sender);
        void __fastcall STARTandNEWGAMEClick(TObject *Sender);
        void __fastcall PLUSPICTUREClick(TObject *Sender);
        void __fastcall chooseClick(TObject *Sender);
        void __fastcall boardClick(TObject *Sender);
        void __fastcall Timer1Timer(TObject *Sender);
        void __fastcall FormCreate(TObject *Sender);
        void __fastcall TIPClick(TObject *Sender);
        void __fastcall PAUSEClick(TObject *Sender);
private:	// User declarations
public:		// User declarations
        __fastcall TForm1(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TForm1 *Form1;
//---------------------------------------------------------------------------
#endif
