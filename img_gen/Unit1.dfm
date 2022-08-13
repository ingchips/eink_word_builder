object Form1: TForm1
  Left = 0
  Top = 0
  Caption = 'Form1'
  ClientHeight = 642
  ClientWidth = 511
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -12
  Font.Name = 'Segoe UI'
  Font.Style = []
  TextHeight = 15
  object Image1: TImage
    Left = 32
    Top = 24
    Width = 200
    Height = 200
  end
  object Button1: TButton
    Left = 288
    Top = 247
    Width = 75
    Height = 25
    Caption = #31572#26696
    TabOrder = 0
    OnClick = Button1Click
  end
  object EditWord: TLabeledEdit
    Left = 288
    Top = 56
    Width = 201
    Height = 23
    EditLabel.Width = 26
    EditLabel.Height = 15
    EditLabel.Caption = #21333#35789
    TabOrder = 1
    Text = 'telephone'
  end
  object EditPron: TLabeledEdit
    Left = 288
    Top = 104
    Width = 201
    Height = 23
    EditLabel.Width = 26
    EditLabel.Height = 15
    EditLabel.Caption = #38899#26631
    TabOrder = 2
    Text = '['#712'tel'#618#716'f'#601#650'n]'
  end
  object EditProp: TLabeledEdit
    Left = 288
    Top = 152
    Width = 201
    Height = 23
    EditLabel.Width = 26
    EditLabel.Height = 15
    EditLabel.Caption = #35789#24615
    TabOrder = 3
    Text = 'n.'
  end
  object EditNote: TLabeledEdit
    Left = 288
    Top = 201
    Width = 201
    Height = 23
    EditLabel.Width = 26
    EditLabel.Height = 15
    EditLabel.Caption = #37322#20041
    TabOrder = 4
    Text = #30005#35805
  end
  object Button2: TButton
    Left = 384
    Top = 247
    Width = 75
    Height = 25
    Caption = #38382#39064
    TabOrder = 5
    OnClick = Button2Click
  end
  object Edit1: TEdit
    Left = 32
    Top = 230
    Width = 121
    Height = 23
    TabOrder = 6
    Text = '0.8'
  end
  object EditDir: TLabeledEdit
    Left = 32
    Top = 277
    Width = 200
    Height = 23
    EditLabel.Width = 38
    EditLabel.Height = 15
    EditLabel.Caption = 'Output'
    TabOrder = 7
    Text = 'c:/tmp/'
  end
  object Button3: TButton
    Left = 288
    Top = 278
    Width = 171
    Height = 25
    Caption = 'CSV '#25209#37327' ...'
    TabOrder = 8
    OnClick = Button3Click
  end
  object ProgressBar1: TProgressBar
    Left = 32
    Top = 320
    Width = 457
    Height = 17
    TabOrder = 9
  end
  object ListBox1: TListBox
    Left = 32
    Top = 352
    Width = 457
    Height = 137
    ItemHeight = 15
    TabOrder = 10
    OnClick = ListBox1Click
  end
  object Memo1: TMemo
    Left = 32
    Top = 501
    Width = 457
    Height = 130
    Lines.Strings = (
      'Memo1')
    ScrollBars = ssBoth
    TabOrder = 11
  end
  object OpenDialog1: TOpenDialog
    Left = 224
    Top = 248
  end
end
