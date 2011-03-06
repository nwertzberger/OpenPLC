# author: Levente Kovacs
# author: Cory Cross made drill size 35mils instead
# email: lekovacs@interware.hu
# dist-license: GPL 2
# use-license: unlimited

Element["" "RJ-45 Jack, no shielding" "CONN" "" 200000 142500 -5000 0 0 100 ""]
(
	Pin[10000 35000 6000 2000 6000 3500 "" "8" "edge2"]
	Pin[0 30000 6000 2000 6000 3500 "" "7" "edge2"]
	Pin[10000 15000 6000 2000 6000 3500 "" "4" "edge2"]
	Pin[0 10000 6000 2000 6000 3500 "" "3" "edge2"]
	Pin[0 20000 6000 2000 6000 3500 "" "5" "edge2"]
	Pin[0 0 6000 2000 6000 3500 "" "1" "square,edge2"]
	Pin[10000 5000 6000 2000 6000 3500 "" "2" "edge2"]
	Pin[10000 25000 6000 2000 6000 3500 "" "6" "edge2"]
	Pin[-25000 -5000 12800 2000 12800 12800 "" "" "hole,edge2"]
	Pin[-25000 40000 12800 2000 12800 12800 "" "" "hole,edge2"]
	ElementLine [-47000 -14500 -47000 49300 1000]
	ElementLine [-56000 49500 15000 49500 1000]
	ElementLine [15000 49500 15000 -14500 1000]
	ElementLine [-56000 -14500 15000 -14500 1000]
	ElementLine [-56000 -14500 -56000 49500 1000]

	)
