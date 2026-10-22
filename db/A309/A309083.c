/*
 * A309083  a(n) = n - floor(n/2^4) + floor(n/3^4) - floor(n/4^4) + ...
 * https://oeis.org/A309083
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=1..⌊n^{1/4}⌋} (-1)^{k+1} ⌊n / k^4⌋
 * Range:     u8 n <= 268, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t sn   = (seq_t)n;   /* convert index to value type */
    seq_t sum  = 0;          /* running total */

    for (idx_t k = 1; ; ++k)
    {
        seq_t kk  = (seq_t)k;                 /* k as value type   */
        seq_t k4  = kk * kk * kk * kk;        /* k^4                */
        if (k4 > sn) break;                  /* stop when k^4 > n  */

        seq_t term = sn / k4;                /* ⌊n / k^4⌋           */
        if ((k & 1) == 1)                    /* k odd → add       */
            sum += term;
        else                                 /* k even → subtract */
            sum -= term;
    }
    return sum;
}
