/*Convert binary integer to BCD
bcd values in bcd variables*/


#include "Binary_to_BCD.h"

//generates BCD
//bcd10000, bcd1000, bcd100,bcdtens, bcdunits
void binary_to_BCDconvert(int nbin)
	{
bcd10000 = 0;
bcd1000 =0;
bcd100 =0;
bcdtens =0;
bcdunits =0;

while (nbin>=10000) {
nbin = nbin-10000;
bcd10000 =bcd10000+1;
}
while (nbin>=1000) {
nbin = nbin-1000;
bcd1000 =bcd1000+1;
}
while (nbin>=100) {
nbin = nbin-100;
bcd100 =bcd100+1;
}
while (nbin>=10) {
nbin = nbin-10;
bcdtens =bcdtens+1;
}
bcdunits =nbin;
}