/*
 * A395764  First column of the triangular array with T(0, m) = m! and T(n, m)
 *          = T(n - 1, m + 3) - 2*T(n - 1, m + 2) + T(n - 1, m + 1).
 * https://oeis.org/A395764
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{j=0..2*n} (-1)^j * binomial(2*n,j) * (3*n - j)!
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 4, u64 n <= 6, u128 n <= 11,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Compute factorial(3*n) */
    seq_t fact = 1;
    for (idx_t k = 1; k <= 3 * n; ++k)
        fact *= (seq_t)k;

    seq_t g     = fact;        /* current term g_j = C(2n,j)*(3n-j)!   */
    seq_t sum   = 0;
    idx_t maxj  = 2 * n;

    for (idx_t j = 0; j <= maxj; ++j) {
        seq_t term = g;
        if (j & 1)
            sum -= term;
        else
            sum += term;

        if (j == maxj)
            break;

        /* Update g to next j+1:
           g_{j+1} = g_j * (2n - j) / (j + 1) / (3n - j) */
        seq_t num1 = (seq_t)(2 * n - j);   /* numerator for binomial ratio   */
        seq_t den1 = (seq_t)(j + 1);       /* denominator for binomial ratio */
        seq_t den2 = (seq_t)(3 * n - j);   /* denominator for factorial ratio */

        g = (g * num1) / den1;   /* g * (2n - j) / (j + 1) */
        g = g / den2;            /* divide by (3n - j)        */
    }

    return sum;
}
