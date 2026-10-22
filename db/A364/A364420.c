/*
 * A364420  Number of divisors of n of the form 5*k+4 that are at most
 *          sqrt(n).
 * https://oeis.org/A364420
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of divisors d of n such that d ≤ sqrt(n) and d ≡ 4
 *            (mod 5). Implementation uses an integer square‑root routine
 *            based on Newton's method, avoiding any floating‑point operations
 * Range:     u8 n <= 108, u16 n <= 108, u32 n <= 108, u64 n <= 108,
 *            u128 n <= 108, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

static inline idx_t isqrt_u64(idx_t n)
{
    if (n == 0) return 0;
    idx_t x = n;
    idx_t y = (x + 1) >> 1;
    while (y < x)
    {
        x = y;
        y = (x + n / x) >> 1;
    }
    return x;
}

seq_t a(idx_t n)
{
    /* the OEIS offset is 1, but we handle n==0 safely */
    if (n == 0) return 0;

    idx_t s = isqrt_u64(n);
    seq_t cnt = 0;
    for (idx_t d = 1; d <= s; ++d)
    {
        if ((n % d == 0) && (d % 5 == 4))
            ++cnt;
    }
    return cnt;
}
