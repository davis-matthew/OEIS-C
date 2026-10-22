/*
 * A356858  a(n) is the product of the first n numbers not divisible by 5.
 * https://oeis.org/A356858
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Product_{k=1..n} A047201(k)
 * Range:     u8 n <= 5, u16 n <= 7, u32 n <= 12, u64 n <= 19, u128 n <= 32,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;

    /* A047201(n) = n + floor((n-1)/4)  (nth positive integer not divisible by 5) */
    seq_t m = (seq_t)n + ((seq_t)n - (seq_t)1) / (seq_t)4;
    idx_t m_idx = (idx_t)m;             /* safe because m <= 1.25*n for all n */

    seq_t prod = (seq_t)1;
    for (idx_t i = (idx_t)1; i <= m_idx; ++i) {
        if (i % (idx_t)5 != (idx_t)0) {
            prod *= (seq_t)i;
        }
    }
    return prod;
}
