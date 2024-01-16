#
# flos/libutf8
#
# © 2012-2016 Connor Lane Smith <cls@lubutu.com>
# © 2015 Laslo Hunhold <dev@frign.de>
# © 2022-2024 Armands Arseniuss Skolmeisters <arseniuss@arseniuss.id.lv>
#
# Permission is hereby granted, free of charge, to any person obtaining a
# copy of this software and associated documentation files (the "Software"),
# to deal in the Software without restriction, including without limitation
# the rights to use, copy, modify, merge, publish, distribute, sublicense,
# and/or sell copies of the Software, and to permit persons to whom the
# Software is furnished to do so, subject to the following conditions:
#
# The above copyright notice and this permission notice shall be included in
# all copies or substantial portions of the Software.
#
# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
# IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
# FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.  IN NO EVENT SHALL
# THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
# LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
# FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
# DEALINGS IN THE SOFTWARE.

BEGIN {
	FS = ";"
	# set up hexadecimal lookup table
	for(i = 0; i < 16; i++)
		hex[sprintf("%X",i)] = i;
}

$3  ~ /^L/ { alphas[nalpha++] = $1; }
$3  ~ /^Z/ || $5 == "WS" || $5 == "S" || $5 == "B" { spaces[nspace++] = $1; }
$3 == "Cc" { cntrls[ncntrl++] = $1; }
$3 == "Lu" { uppers[nupper++] = $1; tolowers[nuppercase++] = ($14 == "") ? $1 : $14; }
$3 == "Ll" { lowers[nlower++] = $1; touppers[nlowercase++] = ($13 == "") ? $1 : $13; }
$3 == "Lt" { titles[ntitle++] = $1; }
$3 == "Nd" { digits[ndigit++] = $1; }

END {
	mkis("alpha", alphas, nalpha, "source/isalpha.c");
	mkis("cntrl", cntrls, ncntrl, "source/iscntrl.c");
	mkis("digit", digits, ndigit, "source/isdigit.c");
	mkis("lower", lowers, nlower, "source/islower.c", touppers, nlowercase, "upper");
	mkis("space", spaces, nspace, "source/isspace.c");
	mkis("title", titles, ntitle, "source/istitle.c");
	mkis("upper", uppers, nupper, "source/isupper.c", tolowers, nuppercase, "lower");
}

# parse hexadecimal rune index to int
function code(s) {
	x = 0;
	for(i = 1; i <= length(s); i++) {
		c = substr(s, i, 1);
		x = (x*16) + hex[c];
	}
	return x;
}

function mkis(name, runes, nrune, file, cases, ncase, casename) {
	nsingle = 0;
	nrange = 0;
	nlace1 = 0;
	nlace2 = 0;
	mode = 1;

	# sort rune groups into singletons, ranges and laces
	for(j = 0; j < nrune; j++) {
		# range
		if(code(runes[j+1]) == code(runes[j])+1 && ((ncase == 0) ||
		   code(cases[j+1]) == code(cases[j])+1) && j+1 < nrune) {
			if(mode == 2) {
				continue;
			} else if(mode == 3) {
				laces11[nlace1] = runes[j];
				nlace1++;
			} else if(mode == 4) {
				laces21[nlace2] = runes[j];
				nlace2++;
			}
			mode = 2;
			ranges0[nrange] = runes[j];
			if(ncase > 0) {
				rangecases[nrange] = cases[j];
			}
			continue;
		}
		# lace 1
		if(code(runes[j+1]) == code(runes[j])+2 && ((ncase == 0) ||
		   (code(cases[j+1]) == code(runes[j+1])+1 && code(cases[j]) == code(runes[j])+1)) &&
		   j+1 < nrune) {
			if(mode == 3) {
				continue;
			} else if(mode == 2) {
				ranges1[nrange] = runes[j];
				nrange++;
			} else if(mode == 4) {
				laces21[nlace2] = runes[j];
				nlace2++;
			}
			mode = 3;
			laces10[nlace1] = runes[j];
			continue;
		}
		# lace 2
		if(code(runes[j+1]) == code(runes[j])+2 && ((ncase == 0) ||
		   (code(cases[j+1]) == code(runes[j+1])-1 && code(cases[j]) == code(runes[j])-1)) &&
		   j+1 < nrune) {
			if(mode == 4) {
				continue;
			} else if(mode == 2) {
				ranges1[nrange] = runes[j];
				nrange++;
			} else if(mode == 3) {
				laces11[nrange] = runes[j];
				nlace1++;
			}
			mode = 4;
			laces20[nlace2] = runes[j];
			continue;
		}
		# terminating case
		if(mode == 1) {
			singles[nsingle] = runes[j];
			if(ncase > 0) {
				singlecases[nsingle] = cases[j];
			}
			nsingle++;
		} else if(mode == 2) {
			ranges1[nrange] = runes[j];
			nrange++;
		} else if(mode == 3) {
			laces11[nlace1] = runes[j];
			nlace1++;
		} else {
			laces21[nlace2] = runes[j];
			nlace2++;
		}
		mode = 1;
	}

	print "/* Automatically generated by build_c.awk */" > file;
	print "#include \"type.h\"\n" > file;
	print "#define ISCHAR utf8_is"name > file;

	if(ncase > 0) {
		print "#define TOCHAR utf8_to"casename > file;
	}

        if(name == "digit") {
            print "#define TOINT utf8_toint" > file;
        }

	mkisarray(file, "singles", ncase > 0 ? 2 : 1, nsingle, singles, singlecases);
	mkisarray(file, "ranges",  ncase > 0 ? 3 : 2, nrange, ranges0, ranges1, rangecases);
	mkisarray(file, "laces1",  2, nlace1, laces10, laces11);
	mkisarray(file, "laces2",  2, nlace2, laces20, laces21);

	print "#include \"typebody.h\"" > file;
}

function mkisarray(file, name, ncols, nrows, column1, column2, column3) {
	if(nrows == 0) {
		return;
	}

	print "#define "toupper(name)" "name > file;

	if(ncols == 1) {
		print "static utf8_char "name"[][1] = {" > file;
		for(j = 0; j < nrows; j++) {
			print "\t{ 0x"column1[j]" }," > file;
		}
	} else if(ncols == 2) {
		print "static utf8_char "name"[][2] = {" > file;
		for(j = 0; j < nrows; j++) {
			print "\t{ 0x"column1[j]", 0x"column2[j]" }," > file;
		}
	} else if(ncols == 3) {
		print "static utf8_char "name"[][3] = {" > file;
		for(j = 0; j < nrows; j++) {
			print "\t{ 0x"column1[j]", 0x"column2[j]", 0x"column3[j]" }," > file;
		}
	}
	print "};" > file;
}

