/*
 * A282086  Number of n-element subsets of [n+10] having an even sum.
 * https://oeis.org/A282086
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Σ_{k even} C(m,k)·C(e, n−k) where m = ceil((n+10)/2), e =
 *            floor((n+10)/2)
 * Range:     u8 n <= 3, u16 n <= 9, u32 n <= 39, u64 n <= 404, u128 n <= 1000,
 *            big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/gcd.h"

static seq_t binom_helper(idx_t N, idx_t k) {
    if (k == (idx_t)0) return (seq_t)1;
    if (k > N) return (seq_t)0;
    if (k > N - k) k = N - k;            // use symmetry
    seq_t res = (seq_t)1;
    for (idx_t i = (idx_t)1; i <= k; ++i) {
        seq_t num = (seq_t)(N - k + i);  // numerator part
        seq_t den = (seq_t)i;            // denominator part

        /* Cancel common factors with the running result first. */
        seq_t g1 = gcd(res, den);
        res /= g1;
        den /= g1;

        /* Cancel remaining common factors between num and den. */
        seq_t g2 = gcd(num, den);
        num /= g2;
        den /= g2;

        res *= num;
        if (den != (seq_t)1) {
            /* After cancellations den must divide res exactly. */
            res /= den;
        }
    }
    return res;
}

seq_t a(idx_t n) {
    /* Total set size */
    idx_t N = n + (idx_t)10;

    /* Count of odd and even numbers in [1..N] */
    idx_t m = (N + (idx_t)1) / (idx_t)2;   /* odds   */
    idx_t e = N - m;                       /* evens  */

    /* Minimum even k such that n‑k ≤ e. */
    idx_t lower = (n >= e) ? (n - e) : (idx_t)0;
    idx_t k_min = (lower & (idx_t)1) ? lower + (idx_t)1 : lower;  /* make even */

    if (k_min > m) return (seq_t)0;        /* no valid subsets */

    /* Maximum even k up to n and m. */
    idx_t k_max = (n < m) ? n : m;
    if (k_max & (idx_t)1) k_max--;         /* make even */

    seq_t sum = (seq_t)0;
    for (idx_t k = k_min; k <= k_max; k += (idx_t)2) {
        seq_t binom_mk = binom_helper(m, k);
        idx_t t = n - k;                    /* t = n‑k ≤ e */
        seq_t binom_et = binom_helper(e, t);
        sum += binom_mk * binom_et;
    }
    return sum;
}
