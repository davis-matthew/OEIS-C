/*
 * A277051  a(n) = floor(n/(1-3/Pi)).
 * https://oeis.org/A277051
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = floor( n / (1 - 3/π) )
 * Range:     u8 n <= 11, u16 n <= 2953, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* High‑precision constant for π (enough to stay below 1 ulp for all
       0 ≤ n ≤ 10 000). */
    static const double pi =
        3.14159265358979323846264338327950288419716939937510;

    /* 1 - 3/π is positive (~0.04719755...), so division preserves
       ordering.  For n > 0 the result is > 0;  n = 0 is outside the
       offset but we return 0 safely. */
    if (n == 0)
        return (seq_t)0;

    double denom = 1.0 - 3.0 / pi;
    double v = (double)n / denom;      /* real value of the expression */

    /* Truncating a positive double with cast to an unsigned integer
       is equivalent to floor.  The cast to idx_t keeps the code
       type‑neutral; the final cast converts to seq_t. */
    return (seq_t)(idx_t)v;
}
