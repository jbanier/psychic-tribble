<#
	inspired by http://www.sivarajan.com/scripts.html
#>
#######################################################################################
Function Search_AD
{
$ServerInfoFile = New-Item -type file -force "C:\Scripts\Servers.csv"
"ServerName" | Out-File $ServerInfoFile -encoding ASCII
 $n = 0
 $ObjFilter = "(objectClass=Computer)"
 $objSearch = New-Object System.DirectoryServices.DirectorySearcher
 $objSearch.PageSize = 15000
 $objSearch.Filter = $ObjFilter
 $objSearch.SearchRoot = "LDAP://dc=voxbone, dc=com"
 $AllObj = $objSearch.FindAll()
 Write-host -nonewline "Searching AD...."
    foreach ($Obj in $AllObj)
    { 
    $objItemT = $Obj.Properties
    $CName = $objItemT.name
    "$CName" | Out-File $ServerInfoFile -encoding ASCII -append
    $n++
    }
 Write-host "....$n Computer Objects Found"
 }
#######################################################################################
 Function Search_LAdmin
    {
    $GFile = New-Item -type file -force "C:\Scripts\SGroupMemberDetails.csv"
    Import-CSV "C:\Scripts\Servers.csv" | ForEach-Object {
    $SName = $_.ServerName
    $AdminPath = Test-Path "\\$SName\admin$"
        If ($AdminPath -eq "TRUE")
        {
            Write-host -nonewline "Collecting Admin info from $SName...."
            "Server Name - $SName" | Out-File $GFile -encoding ASCII -append
            $group = [ADSI]("WinNT://$SName/Administrators,group") 
            $GMembers = $group.psbase.invoke("Members")
            $GMembers | ForEach-Object {$_.GetType().InvokeMember("Name",'GetProperty', $null, $_, $null) | Out-File $GFile -encoding ASCII -append}
            Write-host "......Done!"
        }

        else
        {
        write-host -fore Red "Cannot Acces $SName" 
        out-file -inputObject "Server Name - $SName -> ########Cannot Access" -filepath $GFile -encoding ASCII -append

        }

    $GMembers = ""
    }
    }
#######################################################################################
function Send_Email
    {
    $EFrom = "amdin@santhosh.lab"
    $ETo = "managers@santhosh.lab"
    $ESubject = "Local Administrator Group Memebership Details"  
    $EBody = “Attached fle ($GFile) contians the local Admin group details”
    $SMTPServer = "mail.santhosh.lab"
    #Provide user name and password for SMTP server if required.  
    #$SMTPUsername = "SMTP User Name"
    #$SMTPPassword = "SMTP Password"
    $Emailmessage = New-Object system.net.mail.mailmessage 
    $Emailmessage.from = ($EFrom) 
    $Emailmessage.To.add($ETo)
    $Emailmessage.Subject = $ESubject
    $Emailmessage.Body = $EBody
    $Attachment = New-Object System.Net.Mail.Attachment($GFile)
    $Emailmessage.Attachments.Add($Attachment)
    $SMTP = New-Object Net.Mail.SmtpClient($SMTPServer)  
    #$SMTPClient.Credentials = New-Object System.Net.NetworkCredential("$SMTPUsername", "$SMTPPassword") 
    $SMTP.Send($Emailmessage)
    }
#######################################################################################
clear
Search_AD
Search_LAdmin
#Send_Email
#######################################################################################
