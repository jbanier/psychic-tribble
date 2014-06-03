
' This is a script to add local user accounts and add them to the local admin group
' Author: JBANIER Date: 28/10/2013

on error resume next

Const ADS_SCOPE_SUBTREE = 200
Const FOR_READING = 1
Const strUser = "localadmin"
Const strPassword = "localpa$$w0rd"
Dim objConnection, objCommand, objFile, strFile, strLDAP, strSelectAttr

Set objNet = WScript.CreateObject( "WScript.Network" )
Set objRootLDAP = GetObject("LDAP://rootDSE")
strPathToDomain = "LDAP://" & objRootLDAP.Get("defaultNamingContext")
strSelectAttr = "cn, operatingSystem"

Set objConnection = CreateObject("ADODB.Connection")
Set objCommand =   CreateObject("ADODB.Command")
objConnection.Provider = "ADsDSOObject"
objConnection.Open "Active Directory Provider"

Set objCommand.ActiveConnection = objConnection
objCommand.CommandText = _
    "Select " & strSelectAttr & " from '" & strPathToDomain _
        & "' Where objectClass='computer'"
Wscript.Echo objCommand.CommandText
objCommand.Properties("Page Size") = 1000
objCommand.Properties("Searchscope") = ADS_SCOPE_SUBTREE
Set objRecordSet = objCommand.Execute

objRecordSet.MoveFirst

Do Until objRecordSet.EOF
    ' Create user
    strComputer = objRecordSet.Fields("cn").Value
    Set colAccounts = GetObject("WinNT://" & strComputer & "")
    Set objUser = colAccounts.Create("user", strUser)
    objUser.SetPassword strPassword
    objUser.SetInfo
    Wscript.Echo "Added user " & strUser & " to " & strComputer
    ' add user to admin group
    Set objLocalUser = GetObject("WinNT://" & strComputer & "/" & strUser)
    Set objLocalAdmGroup = GetObject("WinNT://" & strComputer & "/Administrators,group")
    objLocalAdmGroup.Add(objLocalUser.AdsPath)
    Wscript.Echo "Added user " & strUser & " to " & strComputer & "'s local admin group"
    objRecordSet.MoveNext
Loop

Set objConnection = Nothing
Set objCommand = Nothing
Set objFile = Nothing
Set strFile = Nothing
Set strLDAP = Nothing
Set strSelectAttr = Nothing
