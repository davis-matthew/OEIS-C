/*
 * A258199  a(n) = largest term of A001563 <= n.
 * https://oeis.org/A258199
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = largest k*k! <= n (A001563 terms: 0, 1, 4, 18, 96, ...),
 *            found by iterating k with overflow-safe comparisons
 * Range:     u8 n <= 520, u16 n <= 4319, u32 n <= 4319, u64 n <= 4319,
 *            u128 n <= 4319, big any n
 * Validated: standard, pass, 86 terms through n = 3602 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t lim = (seq_t)n;
    seq_t best = 0;
    seq_t fact = 1; /* (k-1)! */
    for (seq_t k = 1;; k++) {
        if (fact > lim / k) break;   /* k! > n, so k*k! > n */
        fact *= k;                   /* k! */
        if (fact > lim / k) break;   /* k*k! > n */
        best = fact * k;
    }
    return best;
}
