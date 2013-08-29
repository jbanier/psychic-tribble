#!/opt/perl/bin/perl 

while(<STDIN>){
	if (m/open(?:64)?\(\"(.+)\",\s([\w|\|]+)(?:,\s\d+)?\)\s+=\s(\d+)/xmi ||
		m/openat(?:64)?\(\d+,\s\"(.+)\",\s([\w|\|]+)(?:,\s\d+)?\)\s+=\s(\d+)/xmi ) {
		$fileName = $1;
		$openOptions = $2;
		$fileDesc = $3;

		print "open,$fileDesc,$fileName $openOptions\n";
	}
	elsif (m/close\((\d+)\)/xmi) {
		print "close,$1\n";
	}
	elsif (m/read\((\d+),.*,\s(\d+)\)/xmi) {
		print "read,$1,$2\n";
	}
	elsif (m/write\((\d+),.*,\s(\d+)\)/xmi) {
		print "write,$1,$2\n";
	}
}
