/*
 * A370014  a(n) = Product_{k=0..n} (2^k + 4^(n-k)).
 * https://oeis.org/A370014
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Product_{k=0..n} (2^k + 4^(n-k))
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 4, u64 n <= 6, u128 n <= 9,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Pre‑compute 4^n = 2^(2n) */
    seq_t pow4n = 1;
    for (idx_t i = 0; i < n; ++i) {
        pow4n *= 4;
    }

    seq_t result = 1;
    seq_t pow2k  = 1;   /* 2^k, starts at k = 0 */

    for (idx_t k = 0; k <= n; ++k) {
        seq_t term = pow2k + pow4n;
        result *= term;

        /* Update for next k */
        if (k < n) {
            pow2k <<= 1;  /* multiply by 2 */
            pow4n /= 4;   /* divide by 4 – exact for integer powers */
        }
    }

    return result;
}
