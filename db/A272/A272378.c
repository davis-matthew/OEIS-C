/*
 * A272378  a(n) = n*(6*n^2 - 8*n + 3).
 * https://oeis.org/A272378
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n*(6*n^2 - 8*n + 3)
 * Range:     u8 n <= 3, u16 n <= 22, u32 n <= 894, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Convert index to arithmetic type */
    seq_t N = (seq_t)n;

    /* Compute the expression in a few steps to avoid intermediate overflow
     * when seq_t is 64‑bit.  The sequence grows as O(n^3), and for n <= 10000
     * the intermediate values fit comfortably in 64 bits.  For larger values
     * that exceed seq_t, overflow is acceptable because the return value will
     * naturally wrap as per modulo arithmetic of the underlying type. */
    seq_t term = 6 * N * N;     /* 6 * n^2 */
    term      -= 8 * N;         /* - 8 * n */
    term      += 3;             /* + 3 */

    return N * term;
}
