/*
 * A341405  Number of nonnegative solutions to (x_1)^2 + (x_2)^2 + ... +
 *          (x_10)^2 <= n.
 * https://oeis.org/A341405
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = #{(x_1,…,x_10)∈ℕ^10 : x_1^2+…+x_10^2 ≤ n}
 * Range:     u8 n <= 3, u16 n <= 17, u32 n <= 36, u64 n <= 36, u128 n <= 36,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>

/* Count nonnegative integer solutions to x_1^2 + … + x_10^2 ≤ n.  */
seq_t a(idx_t n)
{
    /* Allocate DP arrays, zero‑initialised.  */
    size_t size = (size_t)(n + 1);
    seq_t *ways = (seq_t *)calloc(size, sizeof(seq_t));
    seq_t *next = (seq_t *)calloc(size, sizeof(seq_t));
    if (!ways || !next) {
        free(ways);
        free(next);
        return (seq_t)0;
    }

    /* One way to get sum 0: all variables are zero. */
    ways[0] = (seq_t)1;

    /* Process the ten variables one by one. */
    for (idx_t var = 0; var < 10; ++var) {
        /* Reset the next array. */
        memset(next, 0, size * sizeof(seq_t));

        for (idx_t s = 0; s <= n; ++s) {
            seq_t w = ways[(size_t)s];
            if (w == (seq_t)0) continue;

            /* Add squares k² up to the remaining budget n‑s. */
            for (idx_t k = 0;; ++k) {
                idx_t ksq = k * k;
                if (ksq > n - s) break;
                idx_t t = s + ksq;          /* 0 ≤ t ≤ n */
                next[(size_t)t] += w;
            }
        }

        /* Prepare for next variable: swap arrays. */
        seq_t *tmp = ways;
        ways = next;
        next = tmp;
    }

    /* Sum all ways with sum ≤ n. */
    seq_t result = (seq_t)0;
    for (idx_t s = 0; s <= n; ++s) {
        result += ways[(size_t)s];
    }

    free(ways);
    free(next);
    return result;
}
