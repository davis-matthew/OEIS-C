/*
 * A384551  Consecutive states of the linear congruential pseudo-random number
 *          generator 3203713013*s mod 2^32 when started at s=1.
 * https://oeis.org/A384551
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 3203713013 * a(n-1) mod 2^32, a(1) = 1
 * Range:     u8 n <= 1, u16 n <= 1, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* a(1) = 1 */
    seq_t val = 1;
    if (n <= 1)
        return val;

    /* Multiplier and modulus as 64‑bit unsigned constants */
    const unsigned long long MULT = 3203713013ULL;   /* 3203713013 */
    const unsigned long long MOD  = 4294967296ULL;   /* 2^32 */

    /* Iteratively compute a(n) = MULT * a(n-1) mod 2^32 */
    for (idx_t i = 1; i < n; ++i)
        val = (val * MULT) % MOD;

    return val;
}
