/*
 * A345108  a(n) = Sum_{k=1..n} 2^(n - floor(n/k)).
 * https://oeis.org/A345108
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=1..n} 2 ^ (n - floor(n/k))
 * Range:     u8 n <= 6, u16 n <= 13, u32 n <= 28, u64 n <= 59, u128 n <= 122,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Treat n < 1 as 0 (offset is 1, but no harm) */
    if (n == 0)
        return 0;

    seq_t sum = 0;
    seq_t pwr = 1;               /* will become 2^(n-1) */

    /* Compute the initial power 2^(n-1) */
    for (idx_t i = 0; i < n - 1; ++i)
        pwr <<= 1;

    /* Loop over all possible values of t = floor(n/k) */
    for (idx_t t = 1; t <= n; ++t) {
        idx_t cnt = n / t - n / (t + 1);   /* number of k producing this t */
        sum += pwr * (seq_t)cnt;           /* add the contribution         */
        if (t < n)
            pwr >>= 1;                     /* next lower power of two      */
    }

    return sum;
}
