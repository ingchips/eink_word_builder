//---------------------------------------------------------------------------

#ifndef Unit1H
#define Unit1H
//---------------------------------------------------------------------------
#include <System.Classes.hpp>
#include <Vcl.Controls.hpp>
#include <Vcl.StdCtrls.hpp>
#include <Vcl.Forms.hpp>
#include <Vcl.ExtCtrls.hpp>
#include <Vcl.Mask.hpp>
#include <Vcl.Dialogs.hpp>
#include <Vcl.ComCtrls.hpp>
#include <vector>
using namespace std;
//---------------------------------------------------------------------------
class TForm1 : public TForm
{
__published:	// IDE-managed Components
	TImage *Image1;
	TButton *Button1;
	TLabeledEdit *EditWord;
	TLabeledEdit *EditPron;
	TLabeledEdit *EditProp;
	TLabeledEdit *EditNote;
	TButton *Button2;
	TEdit *Edit1;
	TLabeledEdit *EditDir;
	TOpenDialog *OpenDialog1;
	TButton *Button3;
	TProgressBar *ProgressBar1;
	TListBox *ListBox1;
	TMemo *Memo1;
	void __fastcall Button1Click(TObject *Sender);
	void __fastcall Button2Click(TObject *Sender);
	void __fastcall Button3Click(TObject *Sender);
	void __fastcall ListBox1Click(TObject *Sender);
private:	// User declarations
	void GenData(vector<uint8_t> &output);
	void ShowData(vector<uint8_t> &bytes);
	void GenData();
	void DrawQ();
    void DrawA();
public:		// User declarations
	__fastcall TForm1(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TForm1 *Form1;
//---------------------------------------------------------------------------
#endif
