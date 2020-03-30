object Form6: TForm6
  Left = 339
  Top = 306
  Width = 695
  Height = 545
  ActiveControl = Button1
  Caption = 'Сравнение файлов....'
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
    Top = 480
    Width = 113
    Height = 16
    Caption = 'Всего кадров ='
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -13
    Font.Name = 'MS Sans Serif'
    Font.Style = [fsBold]
    ParentFont = False
  end
  object StringGridFiles: TStringGrid
    Left = 8
    Top = 8
    Width = 673
    Height = 457
    ColCount = 2
    DefaultColWidth = 256
    FixedCols = 0
    RowCount = 10000
    Options = [goFixedVertLine, goVertLine, goHorzLine, goRangeSelect, goColSizing]
    TabOrder = 0
    ColWidths = (
      295
      256)
  end
  object Button1: TButton
    Left = 288
    Top = 472
    Width = 113
    Height = 33
    Caption = 'OK'
    TabOrder = 1
    OnClick = Button1Click
  end
  object Num: TEdit
    Left = 128
    Top = 480
    Width = 57
    Height = 21
    ReadOnly = True
    TabOrder = 2
    Text = '0'
  end
end
