/*
 * A386227  Expansion of e.g.f. cosh(x)^2*(1 + x + x^2/2).
 * https://oeis.org/A386227
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n*2^(n-2) for odd n ≥ 3 * a(n) = 2^(n-3)*(C(n,2)+4) for
 *            even n ≥ 4 * with a(0)=1, a(1)=1, a(2)=3
 * Range:     u8 n <= 7, u16 n <= 13, u32 n <= 27, u64 n <= 33, u128 n <= 33,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Base cases. */
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)1;
    if (n == 2) return (seq_t)3;

    /* Compute 2^k in sequence type. */
    seq_t pow2 = (seq_t)1;

    if (n & (seq_t)1) {
        /* Odd n: a(n) = n * 2^(n-2). */
        for (idx_t i = 0; i < n - 2; ++i) {
            pow2 *= (seq_t)2;
        }
        return pow2 * (seq_t)n;
    } else {
        /* Even n: a(n) = 2^(n-3) * (C(n,2)+4). */
        for (idx_t i = 0; i < n - 3; ++i) {
            pow2 *= (seq_t)2;
        }
        /* C(n,2) = n*(n-1)/2. */
        seq_t bin = ((seq_t)n * (seq_t)(n - 1)) / (seq_t)2;
        return pow2 * (bin + (seq_t)4);
    }
}
