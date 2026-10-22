/*
 * A383956  Consecutive states of the linear congruential pseudo-random number
 *          generator used by BASIC on the Poly-1 computer when started at 1.
 * https://oeis.org/A383956
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (4253261 * a(n-1) + 3518565) mod 2^24, a(1) = 1
 * Range:     u8 n <= 1, u16 n <= 1, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Handle undefined indices */
    if (n == 0)
        return 0;

    /* constants as seq_t */
    const seq_t mult = (seq_t)4253261;
    const seq_t add  = (seq_t)3518565;
    const seq_t mask = (seq_t)0xFFFFFFULL;   /* 2^24 - 1 */

    seq_t val = (seq_t)1;   /* a(1) */

    /* For n == 1 the result is already in val */
    for (idx_t i = 2; i <= n; ++i)
        val = (mult * val + add) & mask;

    return val;
}
