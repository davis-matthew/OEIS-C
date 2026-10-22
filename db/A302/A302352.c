/*
 * A302352  a(n) = Sum_{k=0..n} k^4*binomial(2*n-k,n).
 * https://oeis.org/A302352
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..n} k^4 * binomial(2*n - k, n)
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 14, u64 n <= 30, u128 n <= 62,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t sum = 0;

    /* Compute binomial(2*n, n) iteratively. */
    seq_t cur = 1;
    for (idx_t i = 1; i <= n; ++i) {
        cur = cur * (seq_t)(n + i);
        cur = cur / (seq_t)i;   /* exact division */
    }

    /* Sum terms for k = 1 .. n (k=0 contributes 0). */
    for (idx_t k = 1; k <= n; ++k) {
        /* Update cur to binomial(2*n - k, n). */
        cur = cur * (seq_t)(n - k + 1);
        cur = cur / (seq_t)(2 * n - k + 1);  /* exact */

        /* Compute k^4. */
        seq_t kk = (seq_t)k;
        seq_t k4 = kk * kk * kk * kk;

        sum += k4 * cur;
    }
    return sum;
}
