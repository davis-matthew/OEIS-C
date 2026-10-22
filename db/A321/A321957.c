/*
 * A321957  a(n) = binomial(3*n, n + 1)*hypergeom([1, 1 - 2*n], [2 + n], -1).
 * https://oeis.org/A321957
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = ∑_{k=0}^{2n-1} binomial(3*n, k) (a(0)=0)
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 10, u64 n <= 21, u128 n <= 21,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;

    /* Compute binomial coefficients of C(3n, k) iteratively. */
    idx_t N      = 3 * n;          /* top of the binomial */
    idx_t Kmax   = 2 * n - 1;      /* highest k to include */

    seq_t cur     = 1;            /* C(3n,0) */
    seq_t result  = 0;            /* running sum */

    /* k = 0 term */
    result += cur;

    for (idx_t k = 1; k <= Kmax; ++k) {
        /* C(3n,k) = C(3n,k-1) * (3n-(k-1)) / k   */
        cur = (seq_t)(((unsigned __int128)cur * (N - (k - 1))) / k);
        result += cur;
    }

    return result;
}
