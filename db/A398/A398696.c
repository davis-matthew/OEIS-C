/*
 * A398696  a(n) = floor(3*11^n/4).
 * https://oeis.org/A398696
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, base
 * Formula:   a(n) = floor(3*11^n/4)
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 9, u64 n <= 18, u128 n <= 37,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence used: a(0)=0, a(n)=11*a(n-1)+2 if n even, +8 if n odd (n>=1) */

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;

    seq_t res = 0;
    for (idx_t i = 1; i <= n; ++i) {
        res = res * (seq_t)11;
        if (i & 1)      /* odd n */
            res += (seq_t)8;
        else            /* even n */
            res += (seq_t)2;
    }
    return res;
}
