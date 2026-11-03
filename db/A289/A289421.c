/*
 * A289421  Number of Dyck paths of semilength n and height exactly 8.
 * https://oeis.org/A289421
 *
 * Offset:    8    (a(8) is the first term)
 * Keywords:  nonn, easy
 * Formula:   number of Dyck paths of semilength n and height exactly 8.
 *            Computed via dynamic programming: the number of paths of length
 *            2n that stay in [0,…,k] is obtained by two passes up and down.
 *            The desired value is f(8) – f(7), where f(k) is the number of
 *            length‑2n Dyck paths of height ≤ k. The algorithm...
 * Range:     u8 n <= 10, u16 n <= 13, u32 n <= 21, u64 n <= 37, u128 n <= 71,
 *            big any n
 * Validated: standard, pass, 40 terms through n = 991 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

static seq_t _compute_f(idx_t n, int k)
{
    /* `cur` holds the number of paths that end at each height
       after a certain number of steps.  `next` is the same for the
       following step.  Heights range 0…8, so the arrays have fixed
       size 9; we only index up to `k`. */
    seq_t cur[9], next[9];
    int h;

    for (h = 0; h <= k; ++h) {
        cur[h] = (seq_t)0;
        next[h] = (seq_t)0;
    }
    cur[0] = (seq_t)1;                 /* empty path   */

    for (idx_t step = 1; step <= 2 * n; ++step) {
        for (h = 0; h <= k; ++h) next[h] = (seq_t)0;

        for (h = 0; h <= k; ++h) {
            if (h > 0)     next[h] += cur[h - 1];   /* up   */
            if (h < k)     next[h] += cur[h + 1];   /* down */
        }

        for (h = 0; h <= k; ++h) cur[h] = next[h];
    }

    return cur[0];                    /* number of paths of length 2n ending at height 0 */
}

seq_t a(idx_t n)
{
    if (n < 8) return (seq_t)0;        /* offset is 8 */

    seq_t f8 = _compute_f(n, 8);
    seq_t f7 = _compute_f(n, 7);
    return f8 - f7;                    /* paths of height exactly 8 */
}
