/*
 * A329830  Beatty sequence for (4+sqrt(10))/3.
 * https://oeis.org/A329830
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = floor(n*(4+sqrt(10))/3)
 * Range:     u8 n <= 61, u16 n <= 61, u32 n <= 61, u64 n <= 61, u128 n <= 61,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;                /* offset is 1; n=0 is out of range */

    /* Work in unsigned 128‑bit to avoid overflow in all intermediate
       calculations.  n fits in idx_t (uint64_t). */
    unsigned __int128 nn = (unsigned __int128)n;

    /* Compute floor(n*sqrt(10)) as floor(sqrt(10*n^2)). */
    unsigned __int128 prod = 10 * nn * nn;          /* 10*n^2 */

    /* Binary search for floor(sqrt(prod)) where prod <= 2^128-1. */
    unsigned __int128 lo = 0;
    unsigned __int128 hi = ((unsigned __int128)1 << 64) - 1; /* sqrt(2^128-1) */
    while (lo < hi) {
        unsigned __int128 mid = (lo + hi + 1) >> 1;
        if (mid * mid <= prod)
            lo = mid;
        else
            hi = mid - 1;
    }
    unsigned __int128 t = lo;            /* floor(n*sqrt(10)) */

    /* Now a(n) = floor((4*n + t)/3). */
    unsigned __int128 a128 = (4 * nn + t) / 3;

    return (seq_t)a128;
}
