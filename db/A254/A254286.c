/*
 * A254286  Expansion of (1 - (1-256*x)^(1/4)) / (64*x).
 * https://oeis.org/A254286
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   (n+1)*a(n) = 64*(4*n-1)*a(n-1), a(0) = 1
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 4, u64 n <= 8, u128 n <= 16,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the n-th term of OEIS sequence A254286. */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;

    seq_t val = (seq_t)1;           /* a(0) */
    for (idx_t i = 1; i <= n; ++i) {
        /* Compute a(i) = 64*(4*i-1)*a(i-1)/(i+1) */
        seq_t top = val * (seq_t)64;
        top = top * (((seq_t)4 * (seq_t)i) - (seq_t)1);
        val = top / ((seq_t)(i + 1));
    }
    return val;
}
