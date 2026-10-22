/*
 * A382597  a(n) = Product_{i=1..n} 1 - i + n*(n - i + 1) - (n - 2*i + 1)*((n
 *          - i + 1) mod 2).
 * https://oeis.org/A382597
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = \prod_{i=1}^{n} (1 - i + n*(n - i + 1) - (n - 2*i +
 *            1)*((n - i + 1) mod 2))
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 7, u64 n <= 11, u128 n <= 18,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t prod = 1;                 /* product over i = 1..n, product of empty set is 1 (n==0) */
    for (idx_t i = 1; i <= n; ++i) {
        seq_t term = (seq_t)1
                    - (seq_t)i
                    + (seq_t)n * ((seq_t)n - (seq_t)i + (seq_t)1);
        if (((n - i + (idx_t)1) & (idx_t)1) != 0) {   /* (n - i + 1) mod 2 == 1 */
            term = term - ((seq_t)n - (seq_t)2 * (seq_t)i + (seq_t)1);
        }
        prod *= term;
    }
    return prod;
}
