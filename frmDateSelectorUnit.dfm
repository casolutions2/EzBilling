object frmDateSelector: TfrmDateSelector
  Left = 0
  Top = 0
  BorderStyle = bsDialog
  Caption = 'Select a date...'
  ClientHeight = 102
  ClientWidth = 233
  Color = clMenuBar
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'Tahoma'
  Font.Style = []
  GlassFrame.SheetOfGlass = True
  OldCreateOrder = False
  Position = poOwnerFormCenter
  PixelsPerInch = 96
  TextHeight = 13
  object Label1: TLabel
    Left = 24
    Top = 24
    Width = 30
    Height = 13
    Caption = 'Date: '
  end
  object dtpSlctdDate: TAdvDateTimePicker
    Left = 72
    Top = 17
    Width = 137
    Height = 21
    Date = 42239.429837962960000000
    Format = ''
    Time = 42239.429837962960000000
    DoubleBuffered = True
    Kind = dkDate
    ParentDoubleBuffered = False
    TabOrder = 0
    TabStop = True
    BorderStyle = bsSingle
    Ctl3D = True
    DateTime = 42239.429837962960000000
    Version = '1.2.2.2'
    LabelFont.Charset = DEFAULT_CHARSET
    LabelFont.Color = clWindowText
    LabelFont.Height = -11
    LabelFont.Name = 'Tahoma'
    LabelFont.Style = []
  end
  object btnOK: TButton
    Left = 32
    Top = 56
    Width = 75
    Height = 25
    Caption = 'OK'
    Default = True
    ModalResult = 1
    TabOrder = 1
    OnClick = btnOKClick
  end
  object btnCancel: TButton
    Left = 134
    Top = 56
    Width = 75
    Height = 25
    Cancel = True
    Caption = 'Cancel'
    ModalResult = 2
    TabOrder = 2
    OnClick = btnCancelClick
  end
end
