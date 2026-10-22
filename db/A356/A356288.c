/*
 * A356288  Sum of numbers in n-th upward diagonal of triangle the sum of {1;
 *          2,3; 4,5,6; 7,8,9,10; ...} and {1; 2,3; 3,4,5; 4,5,6,7; ...}.
 * https://oeis.org/A356288
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (n * ceil(n/2)) + ((15 + 25*n + 15*n^2 + 14*n^3 -
 *            3*(-1)^n * (5 + n*(3 + n))) / 96)
 * Range:     u8 n <= 2, u16 n <= 16, u32 n <= 47, u64 n <= 47, u128 n <= 47,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)   /* offset is 1; for n==0 return 0 (unused) */
        return 0;

    seq_t sn = n;
    /* ceil(n/2) = (n + 1)/2 for integer division */
    seq_t half = (sn + 1) / 2;
    seq_t term1 = sn * half;   /* n * ceil(n/2) */

    /* intermediate S = 5 + n*(n+3) */
    seq_t S = 5 + sn * (sn + 3);

    /* N = 15 + 25*n + 15*n^2 + 14*n^3  +/- 3*S depending on parity */
    seq_t N = 15 + 25 * sn + 15 * sn * sn + 14 * sn * sn * sn;

    /* parity: 0 for even, 1 for odd */
    if ((n & 1ULL) == 0) {      /* even n: subtract 3*S */
        N -= 3 * S;
    } else {                    /* odd n: add 3*S */
        N += 3 * S;
    }

    seq_t term2 = N / 96;       /* division is exact */
    return term1 + term2;
}
