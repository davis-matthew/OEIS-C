/*
 * A373935  Number of compositions of 7*n-3 into parts 6 and 7.
 * https://oeis.org/A373935
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Sum_{k=0..floor((n-3)/6)} binomial(n+k, n-3-6*k)
 * Range:     u8 n <= 11, u16 n <= 17, u32 n <= 29, u64 n <= 34, u128 n <= 34,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This computes a(n) for n >= 1 using a multiplicative binomial coefficient */

static seq_t binom(idx_t n, idx_t k)
{
    if (k > n)
        return 0;
    if (k > n - k)
        k = n - k;
    seq_t r = 1;
    for (idx_t i = 1; i <= k; ++i)
    {
        r = (r * (seq_t)(n - k + i)) / (seq_t)i;
    }
    return r;
}

seq_t a(idx_t n)
{
    if (n < 3)
        return 0;        /* a(1) = 0, a(2) = 0 */

    seq_t sum = 0;
    idx_t km = (n - 3) / 6;   /* floor((n-3)/6) */
    for (idx_t k = 0; k <= km; ++k)
    {
        idx_t r = n - 3 - 6 * k;
        sum += binom(n + k, r);
    }
    return sum;
}
