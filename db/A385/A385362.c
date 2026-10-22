/*
 * A385362  Consecutive states of the linear congruential pseudo-random number
 *          generator (2041*s + 25673) mod 121500 when started at s=1.
 * https://oeis.org/A385362
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, look, easy
 * Formula:   a(n) = (2041 * a(n-1) + 25673) mod 121500, a(1)=1
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    const seq_t MOD = (seq_t)121500;
    const seq_t C   = (seq_t)2041;
    const seq_t D   = (seq_t)25673;

    seq_t val = (seq_t)1;             /* a(1) */
    for (idx_t i = 1; i < n; ++i) {
        val = (C * val + D) % MOD;
    }
    return val;
}
