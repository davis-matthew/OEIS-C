/*
 * A382454  Number of solutions winning the Tchoukaillon game with n seeds and
 *          2n pits.
 * https://oeis.org/A382454
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A008302(2n+1 , n) where A008302(m , k) = number of
 *            permutations of [m] with k inversions. Recurrence: T(0,0)=1,
 *            T(m,k)=Σ_{i=0}^{min(k,m-1)} T(m-1,k-i) (m≥1)
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 12, u64 n <= 23, u128 n <= 23,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Special case */
    if (n == 0)
        return (seq_t)1;

    /* Arrays for consecutive rows of the triangle */
    seq_t *cur = (seq_t*)malloc((n + 1) * sizeof(seq_t));
    seq_t *nxt = (seq_t*)malloc((n + 1) * sizeof(seq_t));
    if (!cur || !nxt) return (seq_t)0;   /* Allocation failed – return 0 as a fallback */

    /* Initialise row m = 0: T(0,0)=1, others 0   */
    for (idx_t i = 0; i <= n; ++i)
        cur[i] = (seq_t)0;
    cur[0] = (seq_t)1;

    /* Build rows m = 1 .. 2n+1 */
    for (idx_t m = 1; m <= 2 * n + 1; ++m) {
        seq_t window = 0;
        for (idx_t k = 0; k <= n; ++k) {
            window += cur[k];
            if (k >= m)
                window -= cur[k - m];
            nxt[k] = window;
        }
        /* swap rows for next iteration */
        seq_t *tmp = cur;
        cur = nxt;
        nxt = tmp;
    }

    /* cur now holds row m = 2n+1; answer is T(2n+1 , n) = cur[n] */
    seq_t result = cur[n];

    free(cur);
    free(nxt);
    return result;
}
