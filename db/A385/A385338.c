/*
 * A385338  Consecutive states of the linear congruential pseudo-random number
 *          generator (421*s + 17117) mod 81000 when started at s=1.
 * https://oeis.org/A385338
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, look, easy
 * Formula:   a(1) = 1; a(n) = (421*a(n-1) + 17117) mod 81000 (n >= 2)
 * Range:     u8 n <= 1, u16 n <= 5, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;                /* value outside the required offset */
    if (n == 1)
        return 1;                /* first term of the sequence */

    seq_t cur = 1;
    for (idx_t i = 2; i <= n; ++i)
    {
        cur = (421 * cur + 17117) % 81000;
    }
    return cur;
}
