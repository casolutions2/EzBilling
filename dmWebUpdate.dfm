object modWebUpdate: TmodWebUpdate
  OldCreateOrder = False
  Height = 251
  Width = 364
  object WebUpdate1: TWebUpdate
    Agent = 'TWebUpdate'
    Authenticate = waAuto
    DateFormat = 'dd/mm/yyyy'
    DateSeparator = '/'
    LogFileName = 'WUPDATE.LOG'
    PostUpdateInfo.Enabled = False
    ShowDownloadProgress = True
    Signature = 'WebUpdate'
    TempDirectory = '.'
    TimeFormat = 'hh:nn'
    TimeSeparator = ':'
    URL = 
      'http://www.ca-solutions.com/programs/updates/EzBilling/updateCon' +
      'trol.INF'
    Version = '2.4.2.0'
    Left = 56
    Top = 24
  end
  object WebUpdateWizardEnglish1: TWebUpdateWizardEnglish
    Welcome = 
      'Press start to start checking for available application updates ' +
      '...'
    StartButton = 'Start'
    NextButton = 'Next'
    ExitButton = 'Exit'
    RestartButton = 'Restart'
    CancelButton = 'Cancel'
    FailedDownload = 'Failed to download updates'
    GetUpdateButton = 'Get update'
    NewVersionFound = 'New version found'
    NewVersion = 'New version'
    NoNewVersionAvail = 'No new version available.'
    NewVersionAvail = 'New version available.'
    CurrentVersion = 'Current version'
    NoFilesFound = 'No files found for update'
    NoUpdateOnServer = 'No update found on server ...'
    CannotConnect = 'Could not connect to update server or'
    WhatsNew = 'What'#39's new'
    License = 'License agreement'
    AcceptLicense = 'I accept'
    NotAcceptLicense = 'I do not accept'
    ComponentsAvail = 'Available application components'
    DownloadingFiles = 'Downloading files'
    CurrentProgress = 'Current file progress'
    TotalProgress = 'Total file progress'
    UpdateComplete = 'Update completed ...'
    RestartInfo = 'Press restart to start the updated application.'
    WhatsNewPopup = 'View in Notepad'
    LicensePopup = 'View in Notepad'
    Left = 168
    Top = 112
  end
  object WebUpdateWizard1: TWebUpdateWizard
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -12
    Font.Name = 'Verdana'
    Font.Style = [fsBold]
    Language = WebUpdateWizardEnglish1
    WebUpdate = WebUpdate1
    Left = 176
    Top = 40
  end
end
