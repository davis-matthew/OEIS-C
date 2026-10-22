/*
 * A383419  a(n) = A378762(A381968(n)).
 * https://oeis.org/A383419
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabf
 * Formula:   a(n) = (r-1)*(2r-3) + P(r,k) where r is the unique integer with
 *            r*(2r-1) >= n, k = n - (r-1)*(2r-3), P(r,k) is defined piecewise
 *            in the OEIS entry: if k < m (m = 2r-1) : if (k odd) P = m + k;
 *            else P = m + 1 - k; else (k >= m) : if (k odd) P = 3m - 1 - k;
 *            else P = 2m - 1 - k; This implements a(n)...
 * Range:     u8 n <= 22, u16 n <= 7563, u32 n <= 9730, u64 n <= 9730,
 *            u128 n <= 9730, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Find the row r such that n lies in its block.
       The cumulative length of the first r rows is r*(2r-1). */
    idx_t low = 1;
    idx_t high = n;               /* upper bound – safe for all n >= 1 */
    while (low < high) {
        idx_t mid = low + (high - low) / 2;
        seq_t val = (seq_t)mid * ((seq_t)2 * mid - 1);
        if (val >= n)
            high = mid;
        else
            low = mid + 1;
    }
    idx_t r = low;                       /* row index (>=1) */

    /* Base of the current row: cumulative length of previous rows. */
    idx_t prev = r - 1;
    seq_t base = (seq_t)prev * ((seq_t)2 * r - 3);   /* (r-1)*(2r-3) */

    /* Position k (1‑based) of the element within its row. */
    seq_t k = (seq_t)n - base;            /* 1 ≤ k ≤ 4r-3 */

    /* Constants for the permutation formula. */
    seq_t m = (seq_t)2 * r - 1;           /* 2r-1, odd */

    /* Compute P(r,k) according to the piece‑wise definition. */
    seq_t P;
    if (k < m) {                          /* k < m */
        if (k & 1)                        /* odd k */
            P = m + k;
        else                              /* even k */
            P = m + 1 - k;
    } else {                              /* k >= m */
        if (k & 1)                        /* odd k */
            P = 3 * m - 1 - k;
        else                              /* even k */
            P = 2 * m - 1 - k;
    }

    /* Result is the base (the first number of this block) plus P. */
    return base + P;
}
