object dmUniDac: TdmUniDac
  OldCreateOrder = False
  Height = 611
  Width = 1045
  object SQLiteUniProvider1: TSQLiteUniProvider
    Left = 72
    Top = 16
  end
  object uCon: TUniConnection
    DataTypeMap = <
      item
        DBType = 603
        FieldType = ftWideString
        FieldLength = 1024
      end
      item
        DBType = 615
        FieldType = ftString
        FieldLength = 1024
      end
      item
        DBType = 616
        FieldType = ftString
        FieldLength = 1024
      end
      item
        DBType = 617
        FieldType = ftWideString
        FieldLength = 1024
      end
      item
        DBType = 605
        FieldType = ftWideString
        FieldLength = 1024
      end>
    ProviderName = 'SQLite'
    SpecificOptions.Strings = (
      'SQLite.ConnectMode=cmReadWrite'
      'SQLite.Direct=True')
    LoginPrompt = False
    AfterConnect = uConAfterConnect
    Left = 24
    Top = 16
  end
  object tblLocations: TUniTable
    TableName = 'Locations'
    Connection = uCon
    MasterSource = dsCustomers
    MasterFields = 'CustomerID'
    DetailFields = 'CustomerID'
    KeyFields = 'ID'
    IndexFieldNames = 'CustomerID'
    Left = 312
    Top = 80
  end
  object dsLocations: TUniDataSource
    DataSet = tblLocations
    Left = 320
    Top = 136
  end
  object dsCustomers: TUniDataSource
    DataSet = tblCustomers
    Left = 24
    Top = 136
  end
  object tblCustomers: TUniTable
    TableName = 'Customers'
    Connection = uCon
    RefreshOptions = [roAfterInsert]
    AfterOpen = tblCustomersAfterOpen
    AfterClose = tblCustomersAfterClose
    AfterScroll = tblCustomersAfterScroll
    OnCalcFields = tblCustomersCalcFields
    Left = 24
    Top = 80
    object tblCustomersCustomerID: TIntegerField
      AutoGenerateValue = arAutoInc
      FieldName = 'CustomerID'
    end
    object tblCustomersMYOBID: TStringField
      FieldName = 'MYOBID'
    end
    object tblCustomersAccountNumber: TStringField
      FieldName = 'AccountNumber'
      Size = 30
    end
    object tblCustomersCoName: TStringField
      FieldName = 'CoName'
      Size = 100
    end
    object tblCustomersLName: TStringField
      FieldName = 'LName'
      Size = 60
    end
    object tblCustomersFName: TStringField
      FieldName = 'FName'
      Size = 40
    end
    object tblCustomersABN: TStringField
      FieldName = 'ABN'
    end
    object tblCustomersActive: TBooleanField
      FieldName = 'Active'
    end
    object tblCustomersIsIndividual: TBooleanField
      FieldName = 'IsIndividual'
    end
    object tblCustomersExternal: TBooleanField
      FieldName = 'External'
    end
    object tblCustomersNotes: TStringField
      FieldName = 'Notes'
      Size = 1024
    end
    object tblCustomersVolPricingAdjust: TBooleanField
      FieldName = 'VolPricingAdjust'
    end
    object tblCustomersVolPricingInterest: TBooleanField
      FieldName = 'VolPricingInterest'
    end
    object tblCustomersFixedPricingDate: TDateField
      FieldName = 'FixedPricingDate'
    end
    object tblCustomersGroupID: TIntegerField
      FieldName = 'GroupID'
    end
    object tblCustomersDisplayName: TStringField
      FieldKind = fkCalculated
      FieldName = 'DisplayName'
      ReadOnly = True
      Size = 160
      Calculated = True
    end
  end
  object dsSales: TUniDataSource
    DataSet = tblSales
    Left = 80
    Top = 136
  end
  object tblSales: TUniTable
    TableName = 'Sales'
    OrderFields = 'FillDate Desc'
    Connection = uCon
    MasterSource = dsCustomers
    MasterFields = 'CustomerID'
    DetailFields = 'CustomerID'
    KeyFields = 'ID'
    IndexFieldNames = 'CustomerID'
    Left = 80
    Top = 80
  end
  object dsTanks: TUniDataSource
    DataSet = tblTanks
    Left = 136
    Top = 136
  end
  object tblTanks: TUniTable
    TableName = 'Tanks'
    Connection = uCon
    MasterSource = dsLocations
    MasterFields = 'ID'
    DetailFields = 'LocationID'
    KeyFields = 'ID'
    IndexFieldNames = 'LocationID'
    Left = 144
    Top = 80
  end
  object dsVehicles: TUniDataSource
    DataSet = tblVehicles
    Left = 200
    Top = 136
  end
  object tblVehicles: TUniTable
    TableName = 'Vehicles'
    Connection = uCon
    Left = 200
    Top = 80
  end
  object dsBillingRates: TUniDataSource
    DataSet = tblBillingRates
    Left = 256
    Top = 136
  end
  object tblBillingRates: TUniTable
    TableName = 'CustomerBillingRates'
    OrderFields = 'DateStamp desc'
    Connection = uCon
    MasterSource = dsCustomers
    MasterFields = 'CustomerID'
    DetailFields = 'CustomerID'
    ReadOnly = True
    KeyFields = 'ID'
    IndexFieldNames = 'CustomerID'
    Left = 256
    Top = 80
  end
  object tblXtrnlAccts: TUniTable
    TableName = 'ExternalAccounts'
    Connection = uCon
    MasterSource = dsCustomers
    MasterFields = 'CustomerID'
    DetailFields = 'CustomerID'
    KeyFields = 'CustomerID'
    Options.DefaultValues = True
    Left = 24
    Top = 216
  end
  object dsXtrnlAccts: TDataSource
    DataSet = qryXtrnlAccts
    Left = 24
    Top = 272
  end
  object qryXtrnlAccts: TUniQuery
    Connection = uCon
    SQL.Strings = (
      
        'SELECT X.CustomerID, CASE C.CoName WHEN '#39#39' THEN C.LName || '#39', '#39' ' +
        '|| C.FName ELSE C.CoName END AS DisplayName, X.ExternalParty, X.' +
        'ExtAccountNumber'
      
        'FROM Customers C INNER JOIN ExternalAccounts X ON C.CustomerID=X' +
        '.CustomerID'
      'ORDER BY DisplayName;')
    Filter = 'CustomerID=1'
    Left = 24
    Top = 328
  end
  object qry1: TUniQuery
    Connection = uCon
    Left = 192
    Top = 24
  end
  object td: TAdvTaskDialog
    CommonButtons = []
    DefaultButton = 0
    DefaultRadioButton = 200
    Footer = 
      'Solution provided by <A href="http://www.ca-solutions.com">CA So' +
      'lutions (QLD) Pty Ltd</A>'
    FooterIcon = tfiInformation
    FooterTextColor = clWindowText
    Icon = tiError
    Options = [doHyperlinks]
    Version = '2.0.2.3'
    Left = 128
    Top = 16
  end
  object tblGroupRates: TUniTable
    Connection = uCon
    Left = 392
    Top = 80
  end
  object dsGroupRates: TDataSource
    DataSet = tblGroupRates
    Left = 392
    Top = 136
  end
  object tblResource: TUniTable
    TableName = 'Resources'
    Connection = uCon
    Left = 864
    Top = 440
  end
  object tblPlanner: TUniTable
    TableName = 'Planner'
    Connection = uCon
    IndexFieldNames = 'Key'
    AfterOpen = tblPlannerAfterOpen
    AfterClose = tblPlannerAfterClose
    Left = 928
    Top = 440
  end
  object dsResource: TUniDataSource
    DataSet = tblResource
    Left = 864
    Top = 488
  end
  object dsPlanner: TUniDataSource
    DataSet = tblPlanner
    Left = 928
    Top = 488
  end
  object plannerDaySource: TDBDaySource
    Active = False
    OnSetFilter = plannerDaySourceSetFilter
    AutoIncKey = False
    DataSource = dsPlanner
    ResourceDataSource.DataSource = dsResource
    ResourceDataSource.ResourceIDField = 'ID'
    ResourceDataSource.ResourceNameField = 'Name'
    ResourceMap = <>
    StartTimeField = 'StartTime'
    EndTimeField = 'Endtime'
    KeyField = 'Key'
    ReadOnly = False
    ResourceField = 'Resource'
    SubjectField = 'Subject'
    NotesField = 'Notes'
    UpdateByQuery = False
    OnFieldsToItem = plannerDaySourceFieldsToItem
    OnItemToFields = plannerDaySourceItemToFields
    OnItemsRead = plannerDaySourceItemsRead
    AutoHeaderUpdate = True
    DateFormat = 'mm/dd/yyyy'
    Day = 42702.000000000000000000
    Mode = dmMultiResDay
    NumberOfDays = 1
    NumberOfResources = 3
    RecurrencyField = 'Recurrency'
    MinTimeField = 'MinTimeField'
    MaxTimeField = 'MaxTimeField'
    Left = 928
    Top = 552
  end
  object tblParams: TUniTable
    Connection = uCon
    Left = 256
    Top = 24
  end
  object qryluCustomers: TUniQuery
    KeyFields = 'CustomerID'
    DataTypeMap = <
      item
        FieldName = 'DisplayName'
        FieldType = ftString
      end>
    Connection = uCon
    SQL.Strings = (
      'SELECT * FROM vwActiveCustomers;')
    DetailFields = 'CustomerID'
    Filtered = True
    Left = 216
    Top = 280
  end
  object tblluLocations: TUniTable
    TableName = 'Locations'
    Connection = uCon
    MasterSource = dsluCustomers
    MasterFields = 'CustomerID'
    DetailFields = 'CustomerID'
    KeyFields = 'LocationID'
    Filtered = True
    Filter = 'AddressLines NOT LIKE '#39'PO BOX%'#39
    FilterOptions = [foCaseInsensitive]
    IndexFieldNames = 'CustomerID'
    ObjectView = True
    Left = 296
    Top = 280
  end
  object dsluCustomers: TDataSource
    DataSet = qryluCustomers
    Left = 216
    Top = 336
  end
  object dsluLocations: TDataSource
    DataSet = tblluLocations
    Left = 296
    Top = 344
  end
  object tblGroups: TUniTable
    TableName = 'Groups'
    Connection = uCon
    Left = 464
    Top = 80
  end
  object dsGroups: TDataSource
    Left = 464
    Top = 136
  end
  object qryGroupRates: TUniQuery
    Connection = uCon
    SQL.Strings = (
      
        'SELECT g.Name, gr.GroupID, gr.EffectiveDate, gr.EndDate, gr.Disc' +
        'ountType, '
      'p.paramName, gr.Rate '
      'FROM Groups g JOIN GroupRates gr ON g.ID=gr.GroupID'
      
        '   JOIN GlobalParams p ON gr.DiscountType=p.ParamValue AND p.Tag' +
        ' = '#39'DiscType'#39
      'ORDER BY g.Name, gr.EffectiveDate;')
    Left = 392
    Top = 24
  end
  object dsLUXtrnlAccts: TDataSource
    DataSet = tblXtrnlAccts
    Left = 96
    Top = 272
  end
  object UniSQLMonitor1: TUniSQLMonitor
    Left = 504
    Top = 288
  end
end
