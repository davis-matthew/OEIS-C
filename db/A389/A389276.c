/*
 * A389276  a(n) = Sum_{i=1..n} (Product_{j=1..n} M(j, ((i+j-2) mod n)+1) +
 *          Product_{j=1..n} M(j, ((i-j-1) mod n)+1)) where M is the n X n
 *          matrix with numbers 1, 2, ..., n^2 in order across rows.
 * https://oeis.org/A389276
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{i=1..n} (Product_{j=1..n} M(j, ((i+j-2) mod n)+1) +
 *            Product_{j=1..n} M(j, ((i-j-1) mod n)+1))
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 6, u64 n <= 10, u128 n <= 16,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* where M(j,k) = (j-1)*n + k, and with offset 0 (a(0)=0). */

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;

    seq_t total = 0;
    for (idx_t i = 1; i <= n; ++i) {
        seq_t prod1 = 1;
        seq_t prod2 = 1;
        for (idx_t j = 1; j <= n; ++j) {
            /* First product column: ((i+j-2) mod n)+1 */
            idx_t col1 = (i + j - 2) % n + 1;
            seq_t val1 = ((seq_t)(j - 1) * n) + col1;
            prod1 = prod1 * val1;

            /* Second product column: ((i-j-1) mod n)+1 */
            idx_t col2 = (i + n - j - 1) % n + 1;
            seq_t val2 = ((seq_t)(j - 1) * n) + col2;
            prod2 = prod2 * val2;
        }
        total = total + prod1 + prod2;
    }
    return total;
}
