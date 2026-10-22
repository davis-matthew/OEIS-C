/*
 * A340881  Row sums of A340880.
 * https://oeis.org/A340881
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Σ_{k=0}^{n-1} 2^(k(k+1)/2) * Π_{j=k+1}^{n-1} (2^j - 1)
 * Range:     u8 n <= 4, u16 n <= 5, u32 n <= 8, u64 n <= 11, u128 n <= 15,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;          /* offset is 1; not used for n=0 */
    seq_t result = (seq_t)1;              /* a(1) = 1 */
    if (n == 1) return result;

    seq_t pow2_prev = (seq_t)2;           /* 2^(i-1) for current i */
    seq_t term_add = (seq_t)2;            /* 2^((i-1)*i/2) for current i */

    for (idx_t i = 2; i <= n; ++i) {
        seq_t mult  = pow2_prev - (seq_t)1;
        result = result * mult + term_add;

        if (i == n) break;                /* last iteration: no updates needed */

        pow2_prev <<= 1;                  /* 2^i  for next iteration */
        term_add <<= (unsigned int)i;    /* multiply by 2^i to get next 2^((i-1)*i/2) */
    }
    return result;
}
