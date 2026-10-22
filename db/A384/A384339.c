/*
 * A384339  Consecutive states of the linear congruential pseudo-random number
 *          generator for Berkeley Pascal 3.1 when started at 1.
 * https://oeis.org/A384339
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 1, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(1) = 1
   a(n) = (62605 * a(n‑1) + 113218009) mod 2^29   for n ≥ 2 */

seq_t a(idx_t n)
{
    if (n == 0) return 0;            /* n is undefined, but guard */

    seq_t val = (seq_t)1;            /* a(1) */
    if (n == 1) return val;

    const seq_t mod = (seq_t)((uint64_t)1 << 29);  /* 2^29 */

    for (idx_t i = 2; i <= n; ++i)
    {
        val = ((val * (seq_t)62605 + (seq_t)113218009) % mod);
    }

    return val;
}
