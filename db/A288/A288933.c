/*
 * A288933  Positions of 0 in A288932; complement of A288934.
 * https://oeis.org/A288933
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 3*n-1 - A000120(n-1) (HammingWeight of n-1)
 * Range:     u8 n <= 86, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* count 1-bits in n-1 */
    idx_t k = n - 1;
    unsigned bits = 0;
    while (k) {
        bits += (unsigned)(k & 1u);
        k >>= 1;
    }

    seq_t nn = (seq_t)n;
    return (seq_t)(3 * nn - 1 - (seq_t)bits);
}
