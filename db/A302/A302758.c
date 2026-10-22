/*
 * A302758  a(n) = n^2*(n*(4*n + 3) + 3*n*(-1)^n - 4)/96.
 * https://oeis.org/A302758
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n^2*(n*(4*n + 3) + 3*n*(-1)^n - 4)/96
 * Range:     u8 n <= 8, u16 n <= 35, u32 n <= 44, u64 n <= 44, u128 n <= 44,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t res;

    if ((n & 1ULL) == 0) {          /* even n: a(n) = k^2*(k+1)*(4k-1)/6 where k = n/2 */
        seq_t k = n / 2;

        /* four factors of the product, all of type seq_t */
        seq_t f[4];
        f[0] = k;            /* first k   */
        f[1] = k;            /* second k  */
        f[2] = k + 1;        /* k+1       */
        f[3] = 4 * k - 1;    /* 4k-1      */

        /* divide out the factor 2 */
        if ((f[0] & 1ULL) == 0)          f[0] >>= 1;
        else if ((f[1] & 1ULL) == 0)     f[1] >>= 1;
        else if ((f[2] & 1ULL) == 0)     f[2] >>= 1;   /* f[3] is always odd */

        /* divide out the factor 3 */
        if (f[0] % 3ULL == 0)            f[0] /= 3;
        else if (f[1] % 3ULL == 0)       f[1] /= 3;
        else if (f[2] % 3ULL == 0)       f[2] /= 3;
        else if (f[3] % 3ULL == 0)       f[3] /= 3;    /* guaranteed at least one */

        /* compute the product */
        res = f[0];
        res *= f[1];
        res *= f[2];
        res *= f[3];
    } else {                         /* odd n: a(n) = k(k+1)*(2k+1)^2 / 6 where k = (n-1)/2 */
        seq_t k = (n - 1) / 2;

        seq_t a1 = k;           /* k         */
        seq_t a2 = k + 1;       /* k+1       */
        seq_t a3 = 2 * k + 1;   /* 2k+1      */
        seq_t a4 = a3;          /* (2k+1)^2  */

        /* divide out the factor 2 */
        if ((a1 & 1ULL) == 0)          a1 >>= 1;
        else if ((a2 & 1ULL) == 0)     a2 >>= 1;
        /* a3 and a4 are always odd */

        /* divide out the factor 3 */
        if (a1 % 3ULL == 0)            a1 /= 3;
        else if (a2 % 3ULL == 0)       a2 /= 3;
        else if (a3 % 3ULL == 0)       a3 /= 3;
        else if (a4 % 3ULL == 0)       a4 /= 3;    /* one of them must be divisible by 3 */

        /* compute the product */
        res = a1;
        res *= a2;
        res *= a3;
        res *= a4;
    }

    return res;
}
