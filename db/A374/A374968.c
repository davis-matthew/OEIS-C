/*
 * A374968  a(n) = 6*Sum_{t=0..n-1} [ A000217(t)/n ], where [ x ] means the
 *          fractional part of x here.
 * https://oeis.org/A374968
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 6*∑_{t=0}^{n-1} ((t*(t+1)/2) mod n) / n (a(0)=0)
 * Range:     u8 n <= 23, u16 n <= 68, u32 n <= 68, u64 n <= 68, u128 n <= 68,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implements the OEIS A374968 sequence. */
seq_t a(idx_t n) {
    if (n == 0) return 0;
    seq_t sum_mod = 0;
    idx_t cur = 0;
    for (idx_t k = 1; k < n; ++k) {
        /* Compute (cur + k) mod n without overflow. */
        if (k >= n - cur) {
            cur = k - (n - cur);   /* equivalent to cur = cur + k - n */
        } else {
            cur += k;
        }
        sum_mod += (seq_t)cur;
    }
    return ((seq_t)6 * sum_mod) / (seq_t)n;
}
