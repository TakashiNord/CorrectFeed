object FileSelectDialog: TFileSelectDialog
  Left = 204
  Top = 386
  Width = 369
  Height = 253
  Caption = '��������'
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'MS Sans Serif'
  Font.Style = []
  OldCreateOrder = False
  PixelsPerInch = 96
  TextHeight = 13
  object Label1: TLabel
    Left = 8
    Top = 112
    Width = 267
    Height = 13
    Caption = '������� ������ ��� 2-� ����� ������������ �����:'
  end
  object Label2: TLabel
    Left = 40
    Top = 8
    Width = 309
    Height = 13
    Caption = '����  ����������� ��������� ����� ������ ��  ��� �����.'
  end
  object Label3: TLabel
    Left = 8
    Top = 24
    Width = 332
    Height = 13
    Caption = '������ ����� ����� ����� �� �� ���, ��� � �������� ���������,'
  end
  object Label4: TLabel
    Left = 8
    Top = 40
    Width = 274
    Height = 13
    Caption = '� �� ������ �� ����� ��������� �������� ��������.'
  end
  object Label5: TLabel
    Left = 8
    Top = 56
    Width = 335
    Height = 13
    Caption = '�� ������ ����� ����� ������� ������� �����, ������� �� ����'
  end
  object Label6: TLabel
    Left = 8
    Top = 72
    Width = 252
    Height = 13
    Caption = '������� ������� �� �����, ���� ��� ����������.'
  end
  object FileName: TEdit
    Left = 8
    Top = 136
    Width = 249
    Height = 21
    TabOrder = 0
  end
  object OKButton: TButton
    Left = 143
    Top = 184
    Width = 75
    Height = 25
    Caption = 'OK'
    TabOrder = 1
    OnClick = OKButtonClick
  end
  object Browse: TButton
    Left = 272
    Top = 136
    Width = 75
    Height = 25
    Caption = 'Browse...'
    TabOrder = 2
    OnClick = BrowseClick
  end
end
