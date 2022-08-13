//---------------------------------------------------------------------------

#include <vcl.h>
#include <stdint.h>
#include <algorithm>
#include <vector>
#include <math.h>
#pragma hdrstop

#include "Unit1.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TForm1 *Form1;
//---------------------------------------------------------------------------

using namespace std;
#define DISP_W       200
#define DISP_H       200

__fastcall TForm1::TForm1(TComponent* Owner)
	: TForm(Owner)
{
    
}

//---------------------------------------------------------------------------
void __fastcall TForm1::Button1Click(TObject *Sender)
{
	DrawA();

	GenData();
}
//---------------------------------------------------------------------------

double lightness(std::vector<double>&rgb) {
	double maxval, minval;

	maxval = *max_element(rgb.begin(), rgb.end());
	minval = *min_element(rgb.begin(), rgb.end());

	return (maxval + minval) / 2;
}

uint8_t Color2Bit(TColor c)
{
	vector<double> rgb;
	rgb.push_back((c & 0x0000ff) / 256.);
	rgb.push_back(((c & 0x00ff00) >> 8) / 256.);
	rgb.push_back(((c & 0xff0000) >> 16) / 256.);

	return lightness(rgb) >= StrToFloat(Form1->Edit1->Text) ? 1 : 0;
}

void run_length_encode(const vector<uint8_t> &input, vector<uint8_t> &output)
{
	uint8_t last = input[0];
	uint8_t c = 0;
	for (int i = 1; i < input.size(); i++) {
		if (input[i] == last) {
			if (c == 0xff) {
				output.push_back(last);
				output.push_back(c);
				c = 0;
			}
			else
                c++;
		}
		else
		{
            output.push_back(last);
			output.push_back(c);
			last = input[i];
            c = 0;
        }
	}
    output.push_back(last);
    output.push_back(c);
}

void TForm1::GenData(vector<uint8_t> &output)
{
	for (int i = 0; i < DISP_H; i++) {
		for (int j = 0; j < DISP_W / 8; j++) {
			uint8_t v = 0;
			for (int k = 0; k < 8; k++) {
				int x = j * 8 + k;
				v = (v << 1) | Color2Bit(Image1->Canvas->Pixels[x][i]);
			}
			output.push_back(v);
		}
	}
}

void TForm1::GenData(void)
{
	vector<uint8_t> output;
	vector<uint8_t> encode;
	GenData(output);
	//run_length_encode(output, encode);
    ShowData(output);
}

void TForm1::ShowData(vector<uint8_t> &bytes)
{
	Memo1->Lines->Clear();
	String s;
	for (int i = 0; i < bytes.size(); i++) {
		if ((i > 0) && ((i % (DISP_W / 8)) == 0)) {
			Memo1->Lines->Add(s);
			s = "";
		}

		s = s + IntToStr(bytes[i]) + ",";
	}

    Memo1->Lines->Add(s);
}

void TForm1::DrawA(void)
{
    int h = 0;
	Image1->Canvas->Brush->Color = clWhite;
	Image1->Canvas->FillRect(TRect(0, 0, DISP_W, DISP_H));

	Image1->Canvas->Font->Name = "Times New Roman";
	Image1->Canvas->Font->Size = 18;
	Image1->Canvas->TextOut(2, 2, EditWord->Text);
	h += 28;

	if (EditPron->Text.Length() > 0) {
		Image1->Canvas->Font->Name = "Calibri";
		Image1->Canvas->Font->Size = 16;
		Image1->Canvas->TextOut(2, h, EditPron->Text);
		h += 28;
	}
	else
        h += 4;

	Image1->Canvas->MoveTo(2, h);
	Image1->Canvas->LineTo(DISP_W - 2, h);
	h += 4;

	AnsiString s = EditProp->Text.Length() > 0 ? EditProp->Text + " " + EditNote->Text : EditNote->Text;
	Image1->Canvas->Font->Name = "SimSong";
    Image1->Canvas->Font->Size = 18;
	TRect rect(2, h, DISP_W - 2, DISP_H - 2);

	::DrawTextA(Image1->Canvas->Handle, s.c_str(), -1, &rect, DT_LEFT | DT_WORDBREAK | DT_NOPREFIX);
}

void TForm1::DrawQ(void)
{
	int h = 30;
	Image1->Canvas->Brush->Color = clWhite;
	Image1->Canvas->FillRect(TRect(0, 0, DISP_W, DISP_H));

	Image1->Canvas->Font->Name = "Times New Roman";
	Image1->Canvas->Font->Size = 18;
	Image1->Canvas->TextOut(2, h, EditWord->Text);
	h += 28;

	Image1->Canvas->Pen->Width = 2;
	Image1->Canvas->Rectangle(10, 145, 90, 175);
	Image1->Canvas->Rectangle(110, 145, 190, 175);

   //	Image1->Canvas->FillRect(TRect(90, 150, 50, 50));

	Image1->Canvas->Font->Name = "SimSong";
	Image1->Canvas->Font->Size = 15;
	Image1->Canvas->TextOut(15, 150,  "[1] »á ");
	Image1->Canvas->TextOut(115, 150, "[2]²»»á");
}

void __fastcall TForm1::Button2Click(TObject *Sender)
{
	DrawQ();
	GenData();
}
//---------------------------------------------------------------------------

void SaveData(String s, vector<uint8_t> &data)
{
	auto f = new TFileStream(s, fmCreate | fmOpenWrite);
    f->Write(data.data(), data.size());
	f->Free();
}

void __fastcall TForm1::Button3Click(TObject *Sender)
{
	if (!OpenDialog1->Execute())
		return;

	vector<uint8_t> q, a;

	auto l = new TStringList;
	l->LoadFromFile(OpenDialog1->FileName);
	auto w = new TStringList;
    w->Delimiter = ',';
	w->StrictDelimiter = true;

	ProgressBar1->Max = l->Count;

	ListBox1->Items->Assign(l);

	for (int i = 0; i < l->Count; i++) {
		ProgressBar1->Position = i;

		w->DelimitedText = (*l)[i];

		EditWord->Text = (*w)[0];
		EditPron->Text = (*w)[1];
		EditProp->Text = (*w)[2];
		EditNote->Text = (*w)[3];

		DrawQ();
		GenData(q);

		DrawA();
		GenData(a);

		Application->ProcessMessages();
		//break;
	}

	SaveData(EditDir->Text + "A.img", a);
	SaveData(EditDir->Text + "Q.img", q);

	w->Free();
	l->Free();
}
//---------------------------------------------------------------------------

void __fastcall TForm1::ListBox1Click(TObject *Sender)
{
	if (ListBox1->ItemIndex < 0) return;

	auto w = new TStringList;
	w->Delimiter = ',';
    w->StrictDelimiter = true;
	w->DelimitedText = (*ListBox1->Items)[ListBox1->ItemIndex];

	EditWord->Text = (*w)[0];
	EditPron->Text = (*w)[1];
	EditProp->Text = (*w)[2];
	EditNote->Text = (*w)[3];

	DrawA();
}
//---------------------------------------------------------------------------

