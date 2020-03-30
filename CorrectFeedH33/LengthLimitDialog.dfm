object LengthLimit: TLengthLimit
  Left = 532
  Top = 197
  Width = 409
  Height = 162
  Caption = 'Разбивка ленты'
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
    Left = 16
    Top = 40
    Width = 301
    Height = 13
    Caption = 'Введите максимально допустимый размер ленты в байтах'
  end
  object Length: TEdit
    Left = 336
    Top = 32
    Width = 49
    Height = 21
    TabOrder = 0
    Text = '11800'
  end
  object OKButton: TButton
    Left = 75
    Top = 88
    Width = 75
    Height = 25
    Caption = 'OK'
    TabOrder = 1
    OnClick = OKButtonClick
  end
  object ButtonCancel: TButton
    Left = 240
    Top = 88
    Width = 75
    Height = 25
    Caption = 'Отмена'
    TabOrder = 2
    OnClick = ButtonCancelClick
  end
end
