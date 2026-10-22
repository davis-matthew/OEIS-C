/*
 * A342354  M(n,k) = 2*n^2 + 2*k + 1 for 0 <= k <= n and M(n,k) = 2*k^2 + 4*k
 *          - 2*n + 1 for 0 <= n <= k; square array M(n,k) read by ascending
 *          antidiagonals (n, k >= 0).
 * https://oeis.org/A342354
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   For the square array M(n,k) * M(n,k) = 2*n*n + 2*k + 1 if k ≤ n
 *            * = 2*k*k + 4*k – 2*n + 1 if k > n * * The sequence a(n) is the
 *            reading of M(n,k) by ascending antidiagonals: * d = n + k (d =
 *            0,1,2,…) * for each d, k runs from 0 to d, n = d – k. * a(n) is
 *            the n‑th element in this order (offset 0)....
 * Range:     u8 n <= 76, u16 n <= 77, u32 n <= 77, u64 n <= 77, u128 n <= 77,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

static inline idx_t integer_sqrt(idx_t x)
{
    idx_t lo = 0, hi = x;
    while (lo < hi) {
        idx_t mid = lo + ((hi - lo + 1) >> 1);
        if (mid != 0 && mid <= x / mid)
            lo = mid;
        else
            hi = mid - 1;
    }
    return lo;
}

seq_t a(idx_t n)
{
    /* Step 1: find diagonal d and position k on the diagonal. */
    idx_t s   = 8 * n + 1;                 /* 8*n + 1 */
    idx_t ts  = integer_sqrt(s);           /* floor(sqrt(8*n+1)) */
    idx_t t   = (ts + 1) >> 1;             /* floor((sqrt + 1)/2) */
    if (t == 0) t = 1;                    /* ensure t≥1 */
    idx_t d   = t - 1;                     /* diagonal index */

    /* Step 2: number of terms before diagonal d */
    idx_t prev = (idx_t)((((unsigned __int128)d * (d + 1)) >> 1));
    idx_t pos  = n - prev;                 /* k = 0 … d  */
    idx_t row  = d - pos;                  /* n index on diagonal   */
    idx_t col  = pos;                      /* k index on diagonal   */

    /* Step 3: compute M(row, col) */
    seq_t res;
    if (col <= row) {
        /* M(row, col) = 2*row^2 + 2*col + 1 */
        res = 2 * ( (seq_t)row * row ) + 2 * ( (seq_t)col ) + 1;
    } else {
        /* M(row, col) = 2*col^2 + 4*col – 2*row + 1 */
        res = 2 * ( (seq_t)col * col ) + 4 * ( (seq_t)col )
              - 2 * ( (seq_t)row ) + 1;
    }
    return res;
}
