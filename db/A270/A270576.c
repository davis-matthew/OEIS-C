/*
 * A270576  Expansion of g.f. (1+2*x)/(1-6*x).
 * https://oeis.org/A270576
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0) = 1, a(n) = 8*6^(n-1) for n > 0 (so a(1)=8), a(n) =
 *            6*a(n-1) for n > 1
 * Range:     u8 n <= 2, u16 n <= 6, u32 n <= 12, u64 n <= 24, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the n-th term of OEIS A270576 */
seq_t a(idx_t n)
{
    /* Handle the offset (a(0) = 1) */
    if (n == 0)
        return (seq_t)1;

    /* a(1) = 8, then each subsequent term is 6 times the previous one */
    seq_t res = (seq_t)8;
    for (idx_t i = 1; i < n; ++i)
        res *= (seq_t)6;

    return res;
}
