/*
 * A362195  Number of Grassmannian permutations of size n that avoid a
 *          pattern, sigma, where sigma is a pattern of size 8 with exactly
 *          one descent.
 * https://oeis.org/A362195
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 1 + Σ_{k=2..7} binomial(n, k) (k runs over 2,3,4,5,6,7)
 * Range:     u8 n <= 8, u16 n <= 18, u32 n <= 37, u64 n <= 37, u128 n <= 37,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>   /* for bool, if needed in the build */

/* Small binomial helper: works for k ≤ 7, avoids overflow by division each step */
static seq_t binom_small(idx_t n, idx_t k) {
    if (k > n) return 0;
    if (k > n - k) k = n - k;          /* use symmetry to keep k small */
    seq_t result = 1;
    for (idx_t i = 1; i <= k; ++i) {
        /* multiply first to keep integer division exact */
        result = result * (seq_t)(n - k + i) / (seq_t)i;
    }
    return result;
}

/* Sequence function for A362195 */
seq_t a(idx_t n) {
    seq_t total = 1;   /* start with the ‘+1’ term */
    for (idx_t k = 2; k <= 7; ++k) {
        if (n < k) break;      /* binomial(n, k) is zero when k > n */
        total += binom_small(n, k);
    }
    return total;
}
