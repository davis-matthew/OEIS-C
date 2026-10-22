/*
 * A372230  Triangular array read by rows. T(n,k) is the number of size k
 *          circuits in the linear matroid M[A] where A is the n X 2^n-1
 *          matrix whose columns are the nonzero vectors in GF(2)^n, n>=2,
 *          3<=k<=n+1.
 * https://oeis.org/A372230
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = T(r, pos+3) * where * r = smallest row index ≥ 2 such
 *            that (r-1)*r/2 > n-2 * pos = (n-2) - (r-2)*(r-1)/2 * k = pos + 3
 *            (k ranges from 3 to r+1) * and * T(r,k) = ∏_{i=0}^{k-2} (2^r –
 *            2^i) / k! (products over seq_t) * The triangular array is read
 *            row by row (rows start at n=2). * This implem...
 * Range:     u8 n <= 5, u16 n <= 9, u32 n <= 15, u64 n <= 29, u128 n <= 29,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Offset of the sequence is 2: a(2) is the first term. */
    if (n < 2) return 0;               /* undefined for n<2, return 0 */

    /* Convert the linear index (starting at 2) to 0‑based position L. */
    idx_t L = n - 2;                    /* L = 0 for the first element */

    /* Find the row `r` such that (r-1)*r/2 > L.  r starts at 2. */
    idx_t r = 2;
    idx_t cumulative = 1;               /* (r-1)*r/2 for r = 2 */
    while (cumulative <= L) {
        ++r;
        cumulative += r - 1;             /* add the length of the new row */
    }

    /* Number of elements before current row r. */
    idx_t prev_cumulative = cumulative - (r - 1);

    /* Position within the row (0‑based). */
    idx_t pos = L - prev_cumulative;    /* pos ∈ [0, r-2] */

    /* The column index k (cardinality of the circuit) */
    idx_t k = pos + 3;                   /* k ∈ [3, r+1] */

    /* Compute T(r,k) = ∏_{i=0}^{k-2} (2^r – 2^i) / k! */
    seq_t product = 1;
    seq_t two_r = (seq_t)1 << r;        /* 2^r */

    for (idx_t i = 0; i <= k - 2; ++i) {
        seq_t factor = two_r - ((seq_t)1 << i);
        product *= factor;
    }

    /* Compute k! */
    seq_t fact = 1;
    for (idx_t i = 2; i <= k; ++i) {
        fact *= (seq_t)i;
    }

    /* Exact division because the result is integer. */
    product /= fact;
    return product;
}
