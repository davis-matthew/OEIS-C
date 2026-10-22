/*
 * A370569  Number of permutations of [n] having no adjacent 2-cycles and no
 *          adjacent 4-cycles.
 * https://oeis.org/A370569
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Σ_{i,j≥0, 2i+4j≤n} (-1)^(i+j) * (n-i-3j)! / (i! j!)
 * Range:     u8 n <= 5, u16 n <= 8, u32 n <= 12, u64 n <= 20, u128 n <= 22,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Precompute n! as a large integer. */
    seq_t fact = 1;
    for (idx_t k = 1; k <= n; ++k)
        fact *= (seq_t)k;

    seq_t result = 0;

    /* val_i0 will hold (n-i)! / i! for the current i.  */
    seq_t val_i0 = fact;

    for (idx_t i = 0; i <= n / 2; ++i) {
        /* maximum j satisfying 2i + 4j ≤ n */
        idx_t max_j = (n - 2*i) / 4;
        seq_t val_j = val_i0;        /* term for j = 0 */

        for (idx_t j = 0; j <= max_j; ++j) {
            /* Add or subtract the term depending on parity of i+j. */
            if ( ((i + j) & 1) == 0 )
                result += val_j;
            else
                result -= val_j;

            /* Prepare next term for j+1, if applicable. */
            if (j < max_j) {
                seq_t m = (seq_t)(n - i - 3*j);                     /* m = n - i - 3j   */
                /* Denominator: m * (m-1) * (m-2) * (j+1)   */
                seq_t denom = m * (m-1) * (m-2) * (j+1);
                val_j /= denom;
            }
        }

        /* Update val_i0 for the next i, if any. */
        if (i < n / 2) {
            seq_t denom_i = (seq_t)(n - i) * (i + 1);
            val_i0 /= denom_i;
        }
    }

    return result;
}
