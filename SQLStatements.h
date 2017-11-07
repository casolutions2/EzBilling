// text file for sql statements
#include <string>

// MYOB SQL Statements
#pragma region MYOB SQL

const std::wstring sqlMyobIdentifyBy = L"IDENTIFYBY RECORDID";
const std::wstring sqlMyobIdentifyByCardID = L"IDENTIFYBY CARDID";
const std::wstring sqlMyobDuplicates = L"UPDATE DUPLICATES";

const std::wstring sqlBillingAdjust = L"SELECT CardRecordID, HourlyBillingRate, TotalReceivableDays, TotalPaidInvoices " \
	"FROM Customers WHERE IsInactive = 'N' ORDER BY CardIdentification, Name;";

const std::wstring sqlSalesAnalysis = L"SELECT C.CardRecordID, I.ItemName, Sum(IS.TaxExclusiveTotal) as Total_Invoices, Sum(IS.Quantity) as Volume, COUNT(IS.SaleLineID) as Number_Sales, MAX(InvoiceDate) as Last_Invoice_Date " \
	"FROM Customers AS C JOIN Sales ON C.CardRecordID=Sales.CardRecordID " \
	"	JOIN ItemSaleLines AS IS ON IS.SaleID=Sales.SaleID " \
	"	JOIN Items AS I ON IS.ItemID=I.ItemID " \
	"WHERE Sales.InvoiceDate >= '<<<startdate>>>' AND Sales.InvoiceDate <= '<<<enddate>>>' AND I.ItemNumber IN (<<<itemnumbers>>>) " \
	"GROUP BY C.CardRecordID, I.ItemName " \
	"ORDER BY C.CardRecordID;";

const std::wstring sqlUpdateBillingRate = L"INSERT INTO Import_Customer_Cards (RecordID, BillingRate) VALUES ( :p1, :p2 );";

const std::wstring sqlInsertVolumes = L"INSERT INTO Import_Customer_Cards (RecordID, CustomField1) VALUES ( :pRecID, :pVolume );";

const std::wstring sqlInsertMyobCustomer 	= L"INSERT INTO Import_Customer_Cards ( CoLastName, FirstName, CardStatus, ABN, Notes, " \
	"Address1AddressLine1, Address1City, Address1State, Address1PostCode, Address1Country, Address1Phone1, Address1Phone2, Address1Email, Address1ContactName ) " \
	"VALUES ('<<<colastname>>>', '<<<fname>>>', 'Y', '<<<abn>>>', '<<<notes>>>', '<<<addresslines>>>', '<<<city>>>', '<<<state>>>', '<<<postcode>>>', '<<<country>>>', '<<<phone1>>>', '<<<phone2>>>', '<<<email>>>', '<<<contactname>>>' );";

const std::wstring sqlMyobInsertCustomer = L"INSERT INTO Import_Customer_Cards ( CardID, CoLastName, FirstName, CardStatus, ABN, Notes, BillingRate, " \
	"Address1AddressLine1, Address1City, Address1State, Address1PostCode, Address1Country, Address1Phone1, Address1Phone2, Address1Email, Address1ContactName ) " \
	"VALUES (:p1, :p2, :p3, :p4, :p5, :p6, :p7, :p8, :p9, :p10, :p11, :p12, :p13, :p14, :p15, :p16 );";

const std::wstring sqlMyobGetCustomerID = L"SELECT CardRecordID FROM Customers WHERE CardIdentification = '<<<:p1>>>' ";

const std::wstring sqlSyncCustomers 	= L"SELECT CardRecordID, Name, LastName, FirstName, IsIndividual, " \
	"IsInactive, Notes, TermsID, ABN, PriceLevelID, HourlyBillingRate, CustomField1, CustomField2, Identifiers " \
	"FROM Customers WHERE IsInactive = 'N' ORDER BY Name;";

const std::wstring sqlSyncAddress		= L"SELECT CardRecordID, Location, StreetLine1, StreetLine2, StreetLine3, StreetLine4, City, State, Postcode, Country, Phone1, Phone2, Email, ContactName " \
	"FROM Address JOIN Customers ON Address.CardRecordID=Customers.CardRecordID " \
	"WHERE Customers.IsInactive = 'N' " \
	"ORDER BY Customers.CardRecordID, Location;";

const std::wstring sqlSyncCustomerAddress	= L"SELECT Location, StreetLine1, StreetLine2, StreetLine3, StreetLine4, City, State, Postcode, Country, Phone1, Phone2, Email, ContactName " \
	"FROM Address " \
	"WHERE CardRecordID=<<<customerid>>> " \
	"ORDER BY Location;";

const std::wstring sqlSyncSales				= L"SELECT S.CardRecordID, S.InvoiceDate, ISL.Quantity, ISL.TaxExclusiveTotal, Items.ItemNumber, ISL.Description, S.InvoiceNumber " \
	"FROM Sales AS S INNER JOIN Customers AS C ON S.CardRecordID=C.CardRecordID INNER JOIN ItemSaleLines AS ISL ON ISL.SaleID=S.SaleID INNER JOIN Items ON Items.ItemID=ISL.ItemID " \
	"WHERE C.IsInactive = 'N' AND S.InvoiceDate >= '<<<startdate>>>' AND S.InvoiceDate <= '<<<enddate>>>' AND Items.ItemNumber LIKE '<<<itemnumber>>>' " \
	"ORDER BY S.InvoiceDate ASC, S.CardRecordID;";

const std::wstring sqlSyncBillingRates		= L"SELECT CardRecordID, HourlyBillingRate FROM Customers WHERE IsInactive = 'N' ORDER BY Name;";

#pragma region InsertInvoiceStatements
const String sqlLastInvoice 					= L"SELECT Max(InvoiceNumber) FROM Sales WHERE LEFT(InvoiceNumber, 1)='0' AND InvoiceTypeID='I';";

const std::wstring sqlInsertInvoice 			= L"INSERT INTO Import_Item_Sales (RecordID,Inclusive,InvoiceNumber,SaleDate,CustomersNumber," \
	"ItemNumber,Quantity,Description,IncTaxPrice,ExTaxPrice,TaxCode,IncTaxTotal,ExTaxTotal,GSTAmount," \
	"DeliveryStatus,SaleStatus,PaymentIsDue,BalanceDueDays,Memo) " \
	"VALUES ((<<<customer.RecordID>>>,'Y','<<<invoice.InvoiceNumber>>>','<<<invoice.InvoiceDate>>>','<<<invoice.CustomerPO>>>'," ;

const std::wstring sqlInsertInvoiceLine2On		= L",('','','','','', ";
const std::wstring sqlInsertInvoiceLineAll 		= L"'<<<item.ItemNumber>>>',<<<item.Qty>>>,'<<<item.Description>>>',<<<item.IncTaxPrice>>>,<<<item.ExTaxPrice>>>," \
	"'GST',<<<item.IncTaxAmount>>>,<<<item.ExTaxAmount>>>,<<<item.GSTAmount>>>,";
	/*If il = 1 And invoice.InvoiceLines.Count > 1 Then */
const std::wstring sqlInsertInvoiceLineEndMulti	= L"'','','','','')";
	/*Else*/
const std::wstring sqlInsertInvoiceLineEnd1 	= L"'P','I',2,14,'In-situ Gas Delivery')";

#pragma end_region

#pragma end_region

#pragma region SQLite SQL
	#pragma region Retrieve Records
const std::wstring sqlGetCustomers 				= L"SELECT *, CASE CoName WHEN '' THEN LName || FName ELSE CoName END AS DisplayName FROM Customers ORDER BY DisplayName;";

const std::wstring sqlGetCustomerBillingRate 	= L"SELECT * FROM vwLatestBillingRate;";

const std::wstring sqlGetCustomerVolumes		= L"SELECT * FROM vwLatestCustomerVolumes;";

const std::wstring sqlGetLocations 				= L"SELECT * FROM Locations ORDER BY CustomerID, ID;";

const std::wstring sqlGetContainers 			= L"SELECT * FROM Tanks ORDER BY LocationID, ID;";

const std::wstring sqlGetGSTRate				= L"SELECT ParamValue FROM GlobalParams WHERE ParamName='GSTRate';";

const std::wstring sqlGetEAs					= L"SELECT * FROM ExternalAccounts WHERE ExternalParty='<<<extparty>>>' ORDER BY ExtAccountNumber;";

	#pragma end_region
	#pragma region InsertRecords
const std::wstring sqlInsertCustomer			= L"INSERT INTO Customers (MYOBID, AccountNumber, CoName, LName, FName, ABN, Active, IsIndividual, External, Notes, VolPricingAdjust, VolPricingInterest, FixedPricingDate ) " \
	"VALUES ( :p1, :p2, :p3, :p4, :p5, :p6, :p7, :p8, :p9, :p10, :p11, :p12, :p13 );";

const std::wstring sqlInsertExternalAccount		= L"INSERT INTO ExternalAccounts VALUES (:p1, :p2, :p3);";

// this statement is used during synchronisation of myob historical sales
const std::wstring sqlInsertMyobSale 			= L"INSERT INTO Sales (CustomerID, FillDate, Quantity, ExclTotal, MyobItemID, Notes, MyobInvNumber ) " \
	"VALUES (:p1, :p2, :p3, :p4, :p5, :p6, :p7 );";

// this statement is used during importing of external deliveries
const std::wstring sqlInsertSale 			= L"INSERT INTO Sales (CustomerID, LocationID, TankID, FillDate, Quantity, ExclTotal, MyobItemID, Notes, MyobInvNumber ) " \
	"VALUES (:p1, :p2, :p3, :p4, :p5, :p6, :p7, :p8, :p9 );";

const std::wstring sqlInsertCustomerLocation 	= L"INSERT INTO Locations ( CustomerID, AddressLines, City, State, Country, PostCode, Email, Phone, MobilePhone, ContactName, IsPrimary, Notes ) " \
	"VALUES ( :p1, :p2, :p3, :p4, :p5, :p6, :p7, :p8, :p9, :p10, :p11, :p12 );";

const std::wstring sqlInsertBillingRate 		= L"INSERT INTO CustomerBillingRates (CustomerID, DateStamp, Rate) VALUES ( :p1, :p2, :p3 );";

	#pragma end_region

const std::wstring sqlBulkDeactivateCustomers 	= L"UPDATE Customers SET Active=0 WHERE CustomerID NOT IN ( SELECT DISTINCT CustomerID FROM Sales WHERE FillDate >= '<<<startdate>>>' );";

const std::wstring sqlSQLUpdateSalesVolumes		= L"INSERT INTO CustomerVolumes (CustomerID, DateStamp, Volume) " \
	"SELECT CustomerID, CURRENT_TIMESTAMP, SUM(Quantity) " \
	"FROM Sales WHERE FillDate >= '<<<startdate>>>' AND FillDate <= '<<<enddate>>>' AND MyobItemID LIKE '04%' " \
	"GROUP By CustomerID;" ;

const std::wstring sqlRunSheet					= L"SELECT r.ID, r.Name, p.StartTime, p.Subject, l.AddressLines, p.Notes, p.EstQty " \
	"FROM Planner p JOIN Customers c ON p.CustomerID=c.CustomerID JOIN Locations l ON p.LocationID=l.ID JOIN Resources r ON p.Resource=r.ID " \
	"WHERE Resource > 1 AND Date(StartTime) = '<<<pdate>>>' " \
	"ORDER BY Resource, StartTime;";

#pragma end_region

#pragma region Customer2MYOB
const std::wstring sqlGetNonSyncdCustomers     = L"SELECT CustomerID, " \
	"CAST( (CASE Length(CoName) WHEN 0 " \
	"	THEN CASE WHEN LName IS NULL " \
	"			THEN '' ELSE LName END || ', ' || " \
	"		CASE WHEN FName IS NULL " \
	"			THEN '' ELSE FName END " \
	"	ELSE CoName END ) AS varchar(120) ) as DisplayName, Active " \
	"FROM Customers " \
	"WHERE Active = 1 AND MYOBID IS NULL " \
	"ORDER BY DisplayName;";

const std::wstring sqlGetCustomerID     = L"SELECT CustomerID, " \
	"CAST( (CASE Length(CoName) WHEN 0 " \
	"	THEN CASE WHEN LName IS NULL " \
	"			THEN '' ELSE LName END || ', ' || " \
	"		CASE WHEN FName IS NULL " \
	"			THEN '' ELSE FName END " \
	"	ELSE CoName END ) AS varchar(120) ) as DisplayName, CardRecordID " \
	"FROM Customers " \
	"WHERE CardRecordID = '<<<cardid>>>'; ";

#pragma end_region

#pragma region MYOB Errors
const std::wstring sqlGetImportError = L"SELECT Description from ImportErrors WHERE ImportErrorID = <<<nNativeError>>>;";
const std::wstring sqlGetImportWarning	= L"SELECT Description from ImportWarnings WHERE ImportWarningID = <<<nNativeError>>>;";
const std::wstring sqlGetMiscError      = L"SELECT Description from INternalODBCErrors where NativeErrorNumber = <<<nNativeError>>>;";

