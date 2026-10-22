/*
 * A357847  Number of integer compositions of n whose length is twice their
 *          alternating sum.
 * https://oeis.org/A357847
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = sum_{m ≥ 1, m ≤ n/3, n≡m (mod 2)} C((n+m)/2 – 1, m – 1) *
 *            C((n−m)/2 – 1, m – 1) with a(0) = 1
 * Range:     u8 n <= 13, u16 n <= 22, u32 n <= 39, u64 n <= 73, u128 n <= 140,
 *            big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/gcd.h"   /* include to get type definitions */

static /* compute gcd of two seq_t values */ seq_t seq_gcd(seq_t a, seq_t b) {
    while (b) {
        seq_t t = a % b;
        a = b;
        b = t;
    }
    return a;
}

/* Compute the binomial coefficient C(n, k) (0 ≤ k ≤ n) using a multiplicative
   algorithm that reduces at each step to keep intermediate values small.
   All arguments are idx_t, but calculations are performed with seq_t. */
static seq_t binom_idx(idx_t n, idx_t k) {
    if (k > n) return 0;
    if (k > n - k) k = n - k;          /* use symmetry */
    seq_t result = 1;
    for (idx_t i = 1; i <= k; ++i) {
        /* numerator term (n - k + i) */
        seq_t num = (seq_t)(n - k + i);
        /* denominator term i */
        seq_t den = (seq_t)i;

        /* cancel common factors with result first */
        seq_t g = seq_gcd(result, den);
        if (g > 1) {
            result /= g;
            den /= g;
        }

        /* cancel remaining denominator with numerator */
        g = seq_gcd(num, den);
        if (g > 1) {
            num /= g;
            den /= g;
        }

        /* Now den should be 1 */
        result *= num;   /* safe: den == 1 */
    }
    return result;
}

seq_t a(idx_t n) {
    if (n == 0) return 1;   /* the empty composition */

    seq_t total = 0;
    idx_t max_m = n / 3;    /* maximum m satisfying n ≥ 3m */

    for (idx_t m = 1; m <= max_m; ++m) {
        /* require n and m to have the same parity (k = 2m must be even) */
        if ((n ^ m) & 1) continue;

        idx_t A = (n + m) >> 1;   /* sum of odd-position parts */
        idx_t B = (n - m) >> 1;   /* sum of even-position parts */

        /* Since n ≥ 3m, A−1 ≥ m−1 and B−1 ≥ m−1 are guaranteed. */
        seq_t c1 = binom_idx(A - 1, m - 1);
        seq_t c2 = binom_idx(B - 1, m - 1);
        total += c1 * c2;
    }
    return total;
}
