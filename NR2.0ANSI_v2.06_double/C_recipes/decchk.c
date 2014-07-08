int decchk(char string[], int n, char *ch)
{
	char c;
	int j,k=0,m=0;
	static int ip[10][8]={0,1,5,8,9,4,2,7,1,5, 8,9,4,2,7,0,2,7,0,1,
		5,8,9,4,3,6,3,6,3,6, 3,6,4,2,7,0,1,5,8,9, 5,8,9,4,2,7,0,1,6,3,
		6,3,6,3,6,3,7,0,1,5, 8,9,4,2,8,9,4,2,7,0, 1,5,9,4,2,7,0,1,5,8};
	static int ij[10][10]={0,1,2,3,4,5,6,7,8,9, 1,2,3,4,0,6,7,8,9,5,
		2,3,4,0,1,7,8,9,5,6, 3,4,0,1,2,8,9,5,6,7, 4,0,1,2,3,9,5,6,7,8,
		5,9,8,7,6,0,4,3,2,1, 6,5,9,8,7,1,0,4,3,2, 7,6,5,9,8,2,1,0,4,3,
		8,7,6,5,9,3,2,1,0,4, 9,8,7,6,5,4,3,2,1,0};

	for (j=0;j<n;j++) {
		c=string[j];
		if (c >= 48 && c <= 57)
			k=ij[k][ip[(c+2) % 10][7 & m++]];
	}
	for (j=0;j<=9;j++)
		if (!ij[k][ip[j][m & 7]]) break;
	*ch=j+48;
	return k==0;
}
/* (C) Copr. 1986-92 Numerical Recipes Software 9.1-5i. */
