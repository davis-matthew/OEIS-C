/*
 * A304902  Let (P,<) be the strict partial order on the subsets of
 *          {1,2,...,n} ordered by their cardinality. Then a(n) is the number
 *          of paths of any length from {} to {1,2,...,n}.
 * https://oeis.org/A304902
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = ∑_{k=0}^{n-1} C(n,k) * a(k), a(0) = 1 Where C(n,k) is the
 *            binomial coefficient and a(n) counts the number of paths from
 *            the empty set to the full set {1..n} in the poset of subsets
 *            ordered by cardinality
 * Range:     u8 n <= 4, u16 n <= 5, u32 n <= 7, u64 n <= 10, u128 n <= 14,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)1;   /* a(0) = 1 */

    seq_t prefix = (seq_t)1;  /* prefix = Σ_{i=0}^{j-1} a(i) */
    seq_t binom   = (seq_t)1; /* current C(n, j), starting with j=0 */

    for (idx_t j = (idx_t)1; j <= n; ++j)
    {
        /* Update binomial coefficient: C(n,j) = C(n,j-1) * (n-j+1) / j */
        idx_t mult = n - j + (idx_t)1;
        binom = binom * (seq_t)mult / j;

        /* a(j) = binom * prefix (prefix holds Σ_{i=0}^{j-1} a(i)) */
        seq_t f_j = binom * prefix;

        if (j == n) /* a(n) is the last value needed */
            return f_j;

        /* Update prefix for next iteration: prefix = prefix + a(j)
           = prefix * (1 + binom) */
        prefix = prefix * ((seq_t)1 + binom);
    }

    /* Should never reach here */
    return (seq_t)0;
}
