/*
 * A385341  Consecutive states of the linear congruential pseudo-random number
 *          generator (1021*s + 25673) mod 121500 when started at s=1.
 * https://oeis.org/A385341
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (1021 * a(n-1) + 25673) mod 121500, a(1) = 1
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Constants for the recurrence */
    const seq_t MUL   = 1021;
    const seq_t ADD   = 25673;
    const seq_t MOD   = 121500;

    if (n <= 1)               /* n == 1 in OEIS => initial value */
        return (seq_t)1;

    seq_t res = (seq_t)1;     /* a(1) */

    /* Iterate the recurrence up to n */
    for (idx_t i = 2; i <= n; ++i)
    {
        res = (res * MUL + ADD) % MOD;
    }

    return res;
}
