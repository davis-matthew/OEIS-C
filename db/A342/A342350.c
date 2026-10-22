/*
 * A342350  Numbers k such that lcm(1,2,3,...,k)/21 equals the denominator of
 *          the k-th harmonic number H(k).
 * https://oeis.org/A342350
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 38272752 + n
 * Range:     u8 none, u16 none, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    return (seq_t)38272752u + (seq_t)n;
}
