/*
 * A385335  Consecutive states of the linear congruential pseudo-random number
 *          generator (1741*s + 2731) mod 12960 when started at s=1.
 * https://oeis.org/A385335
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, look, easy
 * Formula:   a(n) = (1741 * a(n-1) + 2731) mod 12960, a(1) = 1
 * Range:     u8 n <= 1, u16 n <= 9712, u32 n <= 9712, u64 n <= 9712,
 *            u128 n <= 9712, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* a(1) is the first term */
    if (n <= 1)
        return (seq_t)1;

    seq_t val = (seq_t)1;
    /* iterate n-1 steps */
    for (idx_t i = 2; i <= n; ++i)
    {
        unsigned long long tmp = 1741ULL * (unsigned long long)val + 2731ULL;
        val = (seq_t)(tmp % 12960ULL);
    }
    return val;
}
