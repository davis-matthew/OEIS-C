/*
 * A307310  Expansion of Product_{k>=1} (1 - x^k/(1 - x)^k).
 * https://oeis.org/A307310
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = 1 (for n=0) a(n) = Σ_{m≥1} (-1)^m [ C(n-1, g1(m)-1) +
 *            C(n-1, g2(m)-1) ] for n>0, where g1(m)=m(3m-1)/2,
 *            g2(m)=m(3m+1)/2, and C(N,K)=0 if K<0 or K>N
 * Range:     i8 n <= 9, i16 n <= 19, i32 n <= 31, i64 n <= 35, i128 n <= 35,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    if (n == 0)
        return (seq_t)1;        /* a(0)=1 */

    seq_t sum = 0;
    idx_t N = n - 1;            /* we need binom(N, k-1) */

    for (idx_t m = 1;; ++m) {
        /* Pentagonal numbers (may be computed in 64‑bit safely for n≤10000) */
        idx_t g1 = m * (3 * m - 1) / 2;
        idx_t g2 = m * (3 * m + 1) / 2;

        /* Stop once both exceed n */
        if (g1 > n && g2 > n)
            break;

        seq_t sign = (m % 2 == 0) ? (seq_t)1 : (seq_t)-1;

        if (g1 <= n) {
            idx_t k = g1 - 1;                 /* choose index in binom(N, k) */
            seq_t bin = 1;
            for (idx_t i = 1; i <= k; ++i) {
                bin = (bin * (seq_t)(N - k + i)) / (seq_t)i;
            }
            sum += sign * bin;
        }

        if (g2 <= n) {
            idx_t k = g2 - 1;
            seq_t bin = 1;
            for (idx_t i = 1; i <= k; ++i) {
                bin = (bin * (seq_t)(N - k + i)) / (seq_t)i;
            }
            sum += sign * bin;
        }
    }

    return sum;
}
