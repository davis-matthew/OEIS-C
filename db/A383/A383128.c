/*
 * A383128  Consecutive internal states of the linear congruential
 *          pseudo-random number generator (321*s + 123) mod 10^5 when started
 *          at 1.
 * https://oeis.org/A383128
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, look, easy
 * Formula:   a(1) = 1 a(n) = (321*a(n-1) + 123) mod 100000 for n > 1 (Linear
 *            congruential generator with multiplier 321 and increment 123,
 *            modulus 10^5)
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n < (idx_t)1) {
        /* Behavior below the offset is unspecified; return 0 */
        return (seq_t)0;
    }

    const seq_t MOD = (seq_t)100000;   /* 10^5 */
    seq_t value = (seq_t)1;           /* a(1) */

    if (n == (idx_t)1) {
        return value;
    }

    for (idx_t i = 2; i <= n; ++i) {
        value = ((seq_t)321 * value + (seq_t)123) % MOD;
    }

    return value;
}
