/*
 * A384454  Square array A(n,k), n >= 0, k >= 0, read by antidiagonals
 *          downwards, where A(n,k) is the n-th q-factorial number for q=-k.
 * https://oeis.org/A384454
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, tabl
 * Formula:   A(n,k) = ∏_{j=1..n} (1-(-k)^j)/(1+k) (OEIS A384454)
 * Range:     i8 n <= 31, i16 n <= 49, i32 n <= 61, i64 n <= 65, i128 n <= 65,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Find diagonal d such that the antidiagonal index n lies in it:
       0, 1, 2, …, d-1 have a total of d*(d+1)/2 terms.
       The next diagonal d has (d+1) terms. */
    idx_t d = 0;
    while (((d + 1) * (d + 2)) / 2 <= n)
        ++d;

    /* Position of (i,k) inside that diagonal (0-based numbering). */
    idx_t pos = n - d * (d + 1) / 2;
    idx_t i   = pos;        /* row (0‑based n in the array)     */
    idx_t k   = d - pos;    /* column (q‑factorial parameter)   */

    /* A(0,k) = 1 for all k. */
    if (i == 0)
        return (seq_t)1;

    /* Compute the product over j = 1..i of (1-(-k)^j)/(1+k). */
    seq_t prod = (seq_t)1;
    seq_t pow_negk = (seq_t)1;       /* current (-k)^j  */
    seq_t neg_k    = (seq_t)(-(seq_t)k);
    seq_t denom    = (seq_t)(k + 1);  /* (1+k) > 0 */

    for (idx_t j = 1; j <= i; ++j) {
        pow_negk *= neg_k;              /* (-k)^j */
        prod = prod * ((seq_t)1 - pow_negk) / denom;
    }
    return prod;
}
