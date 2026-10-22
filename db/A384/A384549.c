/*
 * A384549  Consecutive states of the linear congruential pseudo-random number
 *          generator 2396548189*s mod 2^32 when started at s=1.
 * https://oeis.org/A384549
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2396548189 * a(n-1) mod 2^32, a(1)=1
 * Range:     u8 n <= 1, u16 n <= 1, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Multiplier modulo 2^32 (32‑bit wraparound) */
    const seq_t mult = (seq_t)2396548189u;
    const seq_t mask = (seq_t)0xFFFFFFFFu;   /* 2^32 – 1 */

    /* n is 1‑based.  Use exponentiation by squaring. */
    seq_t result = (seq_t)1;           /* a(1) = 1 */
    seq_t base   = mult;
    idx_t exp    = n - (idx_t)1;      /* power to compute */

    while (exp > 0)
    {
        if (exp & (idx_t)1)
            result = (result * base) & mask;
        base = (base * base) & mask;
        exp = exp >> (idx_t)1;
    }

    return result;
}
