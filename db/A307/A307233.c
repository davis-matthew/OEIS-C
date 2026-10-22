/*
 * A307233  a(n) = Product_{k=1..n} (k^2 + k + 1) mod n.
 * https://oeis.org/A307233
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Product_{k=1..n} (k^2 + k + 1) mod n
 * Range:     u8 n <= 16, u16 n <= 77, u32 n <= 77, u64 n <= 77, u128 n <= 77,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Offset 1: a(1) is the first term.  For completeness, return 0 for n<1. */
    if (n < 1)
        return (seq_t)0;

    seq_t prod = (seq_t)1;
    seq_t n_seq = (seq_t)n;

    for (idx_t k = 1; k <= n; ++k)
    {
        seq_t k_seq = (seq_t)k;
        seq_t term   = k_seq * k_seq + k_seq + (seq_t)1;
        prod = (prod * term) % n_seq;
    }
    return prod;
}
