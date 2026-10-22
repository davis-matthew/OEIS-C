/*
 * A357431  Triangle read by rows where each term in row n is the next greater
 *          multiple of n..1.
 * https://oeis.org/A357431
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 376, u16 n <= 9870, u32 n <= 9870, u64 n <= 9870,
 *            u128 n <= 9870, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence a(n): term at index n of the triangle read by rows.
   Triangle definition:
     T(n,1) = n;
     T(n,k) = T(n,k-1) + (n-k+1) - (T(n,k-1) mod (n-k+1))   for k >= 2
   The sequence a(n) is obtained by reading the triangle row‑by‑row. */

/* returns a(n) for n >= 1 */
seq_t a(idx_t n)
{
    /* locate the row r such that r*(r+1)/2 >= n, and
       (r-1)*r/2 < n  <-- r is the smallest row index containing n   */
    idx_t low = 1, high = n;      /* r never exceeds n */
    while (low < high) {
        idx_t mid = low + (high - low) / 2;
        /* compute triangular number mid*(mid+1)/2 safely */
        unsigned __int128 tmid = (unsigned __int128)mid * (mid + 1) / 2;
        if (tmid >= n)
            high = mid;
        else
            low = mid + 1;
    }
    idx_t r = low;   /* row number */

    /* compute column k within that row */
    unsigned __int128 prev_tri = (unsigned __int128)(r - 1) * r / 2;
    idx_t k = n - (idx_t)prev_tri;   /* 1-based column index */

    /* compute the value T(r,k) */
    seq_t val = (seq_t)r;            /* T(r,1) */
    for (idx_t j = 2; j <= k; ++j) {
        seq_t d = (seq_t)(r - j + 1);               /* divisor for this step */
        seq_t rem = val % d;                        /* remainder of previous term */
        val += d - rem;                             /* next multiple of d (> val) */
    }
    return val;
}
