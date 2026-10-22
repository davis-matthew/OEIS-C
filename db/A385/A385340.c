/*
 * A385340  Consecutive states of the linear congruential pseudo-random number
 *          generator (1093*s + 18257) mod 86436 when started at s=1.
 * https://oeis.org/A385340
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (1093 * a(n-1) + 18257) mod 86436, a(1) = 1
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 1)
        return 1;

    seq_t s = 1;
    const seq_t M = 86436;
    for (idx_t i = 2; i <= n; ++i)
        s = ((seq_t)1093 * s + 18257) % M;

    return s;
}
