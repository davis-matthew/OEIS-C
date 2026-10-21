/*
 * A270609  Number of 4X4X4 triangular 0..n arrays with some element plus some
 *          adjacent element totalling n+1 or n-1 exactly once.
 * https://oeis.org/A270609
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   The first 19 terms are given explicitly. For n > 15 the sequence
 *            follows the empirical 9th‑degree polynomials (even and odd n
 *            separately): n even: 36*n^9 - 936*n^8 + 12600*n^7 - 110928*n^6 +
 *            693084*n^5 - 3156192*n^4 + 10401756*n^3 - 23778456*n^2 +
 *            34012980*n - 23053764 n odd : 36*n^9 - 936*n^8 +...
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 10, u64 n <= 58, u128 n <= 58,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* table for n = 1 .. 19  (index 0 unused) */
    static const seq_t base[20] = {
        0,                 /* a(0) unused */
        0,                 /* a(1) */
        0,                 /* a(2) */
        8808ULL,           /* a(3) */
        171576ULL,         /* a(4) */
        1728288ULL,        /* a(5) */
        13177740ULL,       /* a(6) */
        70129212ULL,       /* a(7) */
        309511644ULL,      /* a(8) */
        1134068490ULL,     /* a(9) */
        3559466436ULL,     /* a(10) */
        10133969880ULL,    /* a(11) */
        25490692140ULL,    /* a(12) */
        60750723804ULL,    /* a(13) */
        131276815188ULL,   /* a(14) */
        274807319832ULL,   /* a(15) */
        531982013820ULL,   /* a(16) */
        1009843336080ULL,  /* a(17) */
        1799168710884ULL,  /* a(18) */
        3165521551848ULL   /* a(19) */
    };

    if (n <= 19)
        return base[n];

    /* compute n^k for k = 0 .. 9 in 128‑bit integer */
    __int128 npow[10];
    npow[0] = (__int128)1;
    for (unsigned i = 1; i <= 9; ++i)
        npow[i] = npow[i - 1] * (__int128)n;

    /* coefficients for even and odd n */
    static const int coeff_even[10] = {
        36,    -936,   12600,  -110928, 693084,
        -3156192, 10401756, -23778456, 34012980, -23053764
    };
    static const int coeff_odd[10] = {
        36,    -936,   12816,  -117516, 783468,
        -3895902, 14328570, -37318686, 62032434, -49736268
    };

    const int *coeff = (n & 1) ? coeff_odd : coeff_even;

    __int128 result = 0;
    for (int i = 0; i < 10; ++i)
        result += (__int128)coeff[i] * npow[9 - i];

    return (seq_t)result;
}
