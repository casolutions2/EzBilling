object rptModule: TrptModule
  OldCreateOrder = False
  Height = 328
  Width = 557
  object qryRunSheet: TUniQuery
    Connection = dmUniDac.uCon
    Left = 24
    Top = 24
  end
  object qryPriceAdjustHistory: TUniQuery
    Connection = dmUniDac.uCon
    SQL.Strings = (
      'SELECT * FROM vwPriceAdjustHistory')
    Left = 136
    Top = 24
  end
  object frxDBDataset1: TfrxDBDataset
    UserName = 'frxDBDataset1'
    CloseDataSource = False
    DataSet = qryRunSheet
    BCDToCurrency = False
    Left = 24
    Top = 80
  end
  object frxPDFExport1: TfrxPDFExport
    UseFileCache = True
    ShowProgress = True
    OverwritePrompt = False
    DataOnly = False
    PrintOptimized = False
    Outline = False
    Background = False
    HTMLTags = True
    Quality = 95
    Transparency = False
    Author = 'FastReport'
    Subject = 'FastReport PDF export'
    ProtectionFlags = [ePrint, eModify, eCopy, eAnnot]
    HideToolbar = False
    HideMenubar = False
    HideWindowUI = False
    FitWindow = False
    CenterWindow = False
    PrintScaling = False
    PdfA = False
    Left = 320
    Top = 80
  end
  object frxRunSheet: TfrxReport
    Version = '5.6.2'
    DotMatrixReport = False
    EngineOptions.DoublePass = True
    IniFile = '\Software\Fast Reports'
    PreviewOptions.Buttons = [pbPrint, pbLoad, pbSave, pbExport, pbZoom, pbFind, pbOutline, pbPageSetup, pbTools, pbEdit, pbNavigator, pbExportQuick]
    PreviewOptions.Zoom = 1.000000000000000000
    PrintOptions.Printer = 'Default'
    PrintOptions.PrintOnSheet = 0
    ReportOptions.Author = 'CA Solutions (QLD) Pty Ltd'
    ReportOptions.CreateDate = 42341.641882800900000000
    ReportOptions.Description.Strings = (
      'David Walsh Gas Pty Ltd'#11
      'EzGasBilling Application'#11
      'Designed and Programmed by CA Solutions (QLD) Pty Ltd'
      'www.ca-solutions.com')
    ReportOptions.LastChange = 42998.904421307870000000
    ScriptLanguage = 'PascalScript'
    StoreInDFM = False
    Left = 24
    Top = 136
  end
  object frxPricingHistory: TfrxReport
    Version = '5.6.2'
    DotMatrixReport = False
    IniFile = '\Software\Fast Reports'
    PreviewOptions.Buttons = [pbPrint, pbLoad, pbSave, pbExport, pbZoom, pbFind, pbOutline, pbPageSetup, pbTools, pbEdit, pbNavigator, pbExportQuick]
    PreviewOptions.Zoom = 1.000000000000000000
    PrintOptions.Printer = 'Default'
    PrintOptions.PrintOnSheet = 0
    ReportOptions.CreateDate = 42454.923341724500000000
    ReportOptions.LastChange = 42998.904656805560000000
    ScriptLanguage = 'PascalScript'
    StoreInDFM = False
    Left = 136
    Top = 136
  end
  object frxDBDataset2: TfrxDBDataset
    UserName = 'frxDBDataset2'
    CloseDataSource = False
    DataSet = qryPriceAdjustHistory
    BCDToCurrency = False
    Left = 136
    Top = 80
  end
  object frxReport1: TfrxReport
    Version = '5.6.2'
    DotMatrixReport = False
    IniFile = '\Software\Fast Reports'
    PreviewOptions.Buttons = [pbPrint, pbLoad, pbSave, pbExport, pbZoom, pbFind, pbOutline, pbPageSetup, pbTools, pbEdit, pbNavigator, pbExportQuick]
    PreviewOptions.Zoom = 1.000000000000000000
    PrintOptions.Printer = 'Default'
    PrintOptions.PrintOnSheet = 0
    ReportOptions.CreateDate = 42976.576022662040000000
    ReportOptions.LastChange = 42976.576022662040000000
    ScriptLanguage = 'PascalScript'
    ScriptText.Strings = (
      'begin'
      ''
      'end.')
    Left = 320
    Top = 232
    Datasets = <>
    Variables = <>
    Style = <>
  end
end
