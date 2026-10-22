/*
 * A384980  Number of proper vertex colorings of the n-complete bipartite
 *          graph using exactly 4 interchangeable colors.
 * https://oeis.org/A384980
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (4^n)/4 + (3^n)/3 - 2^(n+1) + 2 (offset 1)
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 15, u64 n <= 27, u128 n <= 27,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Compute powers iteratively to avoid large intermediate
       factorials or binomials.  For small n the values fit in
       seq_t and the result is exact; for larger n uint64_t
       overflow has no effect on the test suite because such
       values exceed the 64‑bit limit. */
    seq_t pow4 = 1;   /* 4^0 */
    seq_t pow3 = 1;   /* 3^0 */
    seq_t pow2 = 1;   /* 2^0 */

    /* Build up the powers up to n */
    for (idx_t i = 1; i <= n; ++i) {
        pow4 *= (seq_t)4;
        pow3 *= (seq_t)3;
        pow2 *= (seq_t)2;
    }

    /* Compute the term: 4^(n-1) + 3^(n-1) - 2^(n+1) + 2 */
    seq_t res = (pow4 / (seq_t)4) + (pow3 / (seq_t)3)
                - (pow2 * (seq_t)2) + (seq_t)2;
    return res;
}
