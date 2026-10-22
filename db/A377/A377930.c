/*
 * A377930  Square array A(n, k), n, k > 0, read by antidiagonals; A(n, k) =
 *          max(A007814(n), A007814(k)).
 * https://oeis.org/A377930
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, tabl
 * Formula:   a(n) = max(v₂(i), v₂(j)) where (i,j) are the indices on the
 *            antidiagonal that contains the n-th term. The pair is obtained
 *            by mapping n to the unique (i,j) with i + j = d + 1, where d is
 *            the minimal integer satisfying d(d+1)/2 ≥ n
 * Range:     u8 n <= 91, u16 n <= 91, u32 n <= 91, u64 n <= 91, u128 n <= 91,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n < 1)
        return (seq_t)0;

    /* Find the antidiagonal d that contains a(n). */
    idx_t d = 1, s = 1;        /* s = 1 + 2 + … + d */
    while (s < n) {
        ++d;
        s += d;
    }

    /* Position of a(n) within that antidiagonal (0‑based). */
    idx_t prev = s - d;        /* sum up to d‑1 terms */
    idx_t t    = n - prev - 1; /* offset within diagonal */

    /* Indices i and j. */
    idx_t i = t + 1;
    idx_t j = d - t;

    /* Compute 2‑adic valuation of i and j. */
    unsigned int vi = 0, vj = 0;
    idx_t tmp;

    tmp = i;
    while ((tmp & 1) == 0) {
        ++vi;
        tmp >>= 1;
    }

    tmp = j;
    while ((tmp & 1) == 0) {
        ++vj;
        tmp >>= 1;
    }

    return (seq_t)(vi > vj ? vi : vj);
}
