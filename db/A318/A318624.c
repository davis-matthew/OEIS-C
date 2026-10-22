/*
 * A318624  Number of 3-member subsets of [3*n] whose elements sum to a
 *          multiple of n.
 * https://oeis.org/A318624
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 9*n - 8*d + 27*((n*n - 3*n + 2*d)/6) * where d = gcd(n,3)
 *            = 3 if n%3==0 else 1. * a(0) = 0. * a(n) counts 3‑member subsets
 *            of [3*n] with sum multiple of n
 * Range:     u8 n <= 8, u16 n <= 121, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;

    idx_t d = (n % 3 == 0) ? 3 : 1;

    seq_t ns = (seq_t)n;
    seq_t ds = (seq_t)d;

    seq_t numerator = ns * ns - (seq_t)3 * ns + (seq_t)2 * ds;
    seq_t C = numerator / (seq_t)6;

    seq_t result = (seq_t)9 * ns - (seq_t)8 * ds + (seq_t)27 * C;
    return result;
}
