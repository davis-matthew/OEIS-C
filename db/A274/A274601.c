/*
 * A274601  a(n) = 2*3^(s-1) - n, where s is the number of trits of n in
 *          balanced ternary form.
 * https://oeis.org/A274601
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = 2*3^(floor(log_3(2*n-1))) - n
 * Range:     u8 n <= 121, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)   /* behaviour for n < 1 is irrelevant */
        return (seq_t)0;

    /* m = 2*n - 1  (fits in idx_t for the tested range) */
    idx_t m = n * 2 - 1;

    /* Find the largest power of 3 not exceeding m */
    idx_t pow3 = 1;                     /* 3^e */
    while (pow3 <= m / 3)               /* avoid overflow when multiplying */
        pow3 *= 3;                      /* now pow3 = 3^floor(log_3(m)) */

    /* a(n) = 2*pow3 - n  (computed in a wider type to avoid overflow) */
    __int128 tmp = (__int128)pow3 * 2 - (__int128)n;

    return (seq_t)tmp;
}
