/*
 * A252970  Number of nX2 nonnegative integer arrays with upper left 0 and
 *          lower right n+2-5 and value increasing by 0 or 1 with every step
 *          right or down.
 * https://oeis.org/A252970
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n*(n-1)*(n-2)*(n^3+24*n^2+65*n-318)/720
 * Range:     u8 n <= 6, u16 n <= 16, u32 n <= 117, u64 n <= 210,
 *            u128 n <= 210, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n < 3)
        return 0;
    /* use 128‑bit arithmetic to avoid overflow for all values that fit
       into seq_t when seq_t is a 64‑bit unsigned integer. */
    __int128 sn = (__int128)n;
    __int128 t1 = sn * (sn - 1) * (sn - 2);
    __int128 t2 = sn * sn * sn + 24 * sn * sn + 65 * sn - 318;
    __int128 num = t1 * t2;
    return (seq_t)(num / 720);
}
