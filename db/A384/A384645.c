/*
 * A384645  Consecutive states of the linear congruential pseudo-random number
 *          generator HSRPUN when started at 1.
 * https://oeis.org/A384645
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 1, u32 n <= 2, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(1)=1,
   a(n) = (5^15 * a(n-1) + 7261067085) mod 2^35  (n ≥ 2) */

seq_t a(idx_t n)
{
    /* 2^35 - 1 mask */
    seq_t mask = (seq_t)1;
    mask <<= 35;
    mask -= (seq_t)1;

    /* 5^15 and additive constant */
    const seq_t C = (seq_t)30517578125ULL;   /* 5^15    */
    const seq_t K = (seq_t)7261067085ULL;    /* additive constant */

    if (n < 1)
        return (seq_t)0;                /* undefined below offset */

    seq_t val = (seq_t)1;                /* a(1) */

    for (idx_t i = 2; i <= n; ++i)
        val = (val * C + K) & mask;      /* recurrence modulo 2^35 */

    return val;
}
