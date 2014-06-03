# sync script between ldap and AD (samba4) used to bootstrap the config
# doesn't sync the password hashes
import sys
from ldif import LDIFParser,LDIFWriter

SKIP_DN = ["uid=foo,ou=People,dc=example,dc=com",
           "uid=bar,ou=People,dc=example,dc=com"]

class MyLDIF(LDIFParser):
    def __init__(self,input,output):
        LDIFParser.__init__(self,input)
        self.writer = LDIFWriter(output)
        self.user = 0
        self.group = 0

    def handle(self,dn,entry):
        if dn in SKIP_DN:
            return
        if "groupOfNames" in entry['objectClass']:
            print ("samba-tool add group {0}").format( entry['cn'] )
            self.group += 1
        if "inetOrgPerson" in entry['objectClass']:
            print ("samba-tool add user {0}").format( entry['cn'] )
            try:
                for group in entry['MEMBEROF']:
                    print ("samba-tool change? group {0} add {1}").format(group, entry['cn'])
            except KeyError:
                pass
            self.user += 1

parser = MyLDIF(open("input.ldif", 'rb'), sys.stdout)
parser.parse()

print ("Added {0} users.").format(parser.user)
print ("Added {0} groups.").format(parser.group)

