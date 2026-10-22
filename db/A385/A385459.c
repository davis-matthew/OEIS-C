/*
 * A385459  Consecutive internal states of the linear congruential
 *          pseudo-random number generator (3877*s + 29573) mod 139968 when
 *          started at 1.
 * https://oeis.org/A385459
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (3877*a(n‑1) + 29573) mod 139968, with a(1) = 1
 * Range:     u8 n <= 1, u16 n <= 1, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Handle the first term directly. */
    if (n == 1) return (seq_t)1;

    /* Constants of the linear congruential generator. */
    const seq_t A     = (seq_t)3877;
    const seq_t B     = (seq_t)29573;
    const seq_t MOD   = (seq_t)139968;

    seq_t val = (seq_t)1;           /* a(1) */

    /* Iterate n‑1 steps.  The loop bounds use idx_t to avoid signed/unsigned
       mismatches, and the comparison is safe because n ≥ 2. */
    for (idx_t i = 2; i <= n; ++i)
    {
        val = (A * val + B) % MOD;
    }

    return val;
}
