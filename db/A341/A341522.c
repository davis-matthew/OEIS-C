/*
 * A341522  a(n) = A156552(3*A005940(1+n)).
 * https://oeis.org/A341522
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 1 + 2*n + 2^v2(1+n) (v2 = exponent of 2)
 * Range:     u8 n <= 126, u16 n <= 8192, u32 n <= 8192, u64 n <= 8192,
 *            u128 n <= 8192, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* m = n+1, never 0 for n >= 0 */
    idx_t m = n + 1;

    /* number of trailing zero bits in m (exponent of 2) */
    unsigned int k = __builtin_ctzll(m);

    /* 2^k computed in a wider type to avoid unsigned overflow before cast */
    uint64_t twopower_big = ((uint64_t)1) << k;

    /* result: 1 + 2*n + 2^k */
    seq_t term = ((seq_t)1) + ((seq_t)2) * ((seq_t)n) + (seq_t)twopower_big;
    return term;
}
