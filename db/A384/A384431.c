/*
 * A384431  Consecutive states of the linear congruential pseudo-random number
 *          generator (430*s + 2531) mod 11979 when started at s=1.
 * https://oeis.org/A384431
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, look, easy
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 9998, u32 n <= 9998, u64 n <= 9998,
 *            u128 n <= 9998, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(1) = 1;
 * a(n) = (430*a(n-1) + 2531) mod 11979  for n ≥ 2
 * The sequence is periodic with period 11979, so a(n) depends only on
 * (n‑1) mod 11979.
 */

seq_t a(idx_t n)
{
    /* a(1) is the first term */
    if (n == 1) return (seq_t)1;

    /* Period of the LCG */
    const seq_t MOD = 11979;
    /* Reduce index to the equivalent position within one period */
    idx_t k = (n - 1) % 11979;   /* 0 ≤ k ≤ 11978 */

    seq_t prev = 1;              /* a(1) */

    for (idx_t i = 1; i <= k; ++i) {
        prev = ( ((seq_t)430 * prev + (seq_t)2531) % MOD );
    }
    return prev;
}
