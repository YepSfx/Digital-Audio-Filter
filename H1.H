m = 21;
n = 31;

for (j = 1; j <= 31; j++) nonbasis[j] = j;

for (i = 1; i <= 21; i++) {
  cb[i] = -1.0;
  basis[i] = 31 + i;
  x = (double) (i - 1) / 20.0;
  a[i][0] = Math.exp (-3 * x * x);

  for (j = 11; j <= 31; j++) a[i][j] = 0;
  a[i][i + 10] = 1;
  a[i][1] = -1.0;
  a[i][6] = +1.0;
  for (j = 2; j <= 5; j++) a[i][j] = x * a[i][j - 1];
  for (j = 7; j <= 10; j++) a[i][j] = x * a[i][j - 1];
}
basis[22] = 0;
a[22][0] = 0;
for (j = 1; j <= 10; j++) a[22][j] = 0;
for (j = 11; j <= 31; j++) a[22][j] = -1;
