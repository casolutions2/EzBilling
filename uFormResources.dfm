object frmResources: TfrmResources
  Left = 0
  Top = 0
  BorderStyle = bsDialog
  Caption = 'Planner Resources...'
  ClientHeight = 144
  ClientWidth = 537
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'Tahoma'
  Font.Style = []
  OldCreateOrder = False
  Position = poMainFormCenter
  OnClose = FormClose
  OnCreate = FormCreate
  PixelsPerInch = 96
  TextHeight = 13
  object Label1: TLabel
    Left = 16
    Top = 64
    Width = 82
    Height = 13
    Caption = 'Resource Name: '
  end
  object Label2: TLabel
    Left = 16
    Top = 96
    Width = 108
    Height = 13
    Caption = 'Resource Description: '
  end
  object ebResourceName: TEdit
    Left = 144
    Top = 61
    Width = 121
    Height = 21
    TabOrder = 0
  end
  object dbNavResources: TDBAdvNavigator
    Left = 0
    Top = 0
    Width = 537
    Height = 30
    AutoThemeAdapt = False
    DataSource = dmUniDac.dsResource
    VisibleButtons = [nbPrior, nbNext, nbInsert, nbEdit, nbPost, nbCancel]
    Align = alTop
    Color = clBtnFace
    ColorTo = clNone
    ColorDown = 14210002
    ColorDownTo = clNone
    ColorHot = 13289415
    ColorHotTo = clNone
    DeleteDisabled = True
    InsertDisabled = False
    GlyphSize = gsLarge
    GlyphCustomSize = 0
    Orientation = noHorizontal
    TabOrder = 1
    Version = '1.3.1.2'
    ExplicitWidth = 192
  end
  object MemoDescription: TMemo
    Left = 144
    Top = 88
    Width = 369
    Height = 48
    Lines.Strings = (
      'MemoDescription')
    TabOrder = 2
  end
  object BindSourceDB1: TBindSourceDB
    DataSource = dmUniDac.dsResource
    ScopeMappings = <>
    Left = 256
    Top = 72
  end
  object BindingsList1: TBindingsList
    Methods = <>
    OutputConverters = <>
    Left = 20
    Top = 5
    object LinkControlToFieldName: TLinkControlToField
      Category = 'Quick Bindings'
      DataSource = BindSourceDB1
      FieldName = 'Name'
      Control = ebResourceName
      Track = True
    end
    object LinkControlToFieldDescription: TLinkControlToField
      Category = 'Quick Bindings'
      DataSource = BindSourceDB1
      FieldName = 'Description'
      Control = MemoDescription
      Track = False
    end
  end
end
