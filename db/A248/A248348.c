/*
 * A248348  a(n) = number of polynomials a_k*x^k + ... + a_1*x + a_0 with k >
 *          0, integer coefficients, only distinct integer roots, and a_0 =
 *          p^n (p is a prime).
 * https://oeis.org/A248348
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of subsets of the set {±p^i : 0≤i≤n} with *
 *            distinct roots whose exponents sum to at most n, minus the empty
 *            * subset. This counts the polynomials with constant term p^n and
 *            * distinct integer roots, as defined in A248348
 * Range:     u8 n <= 6, u16 n <= 23, u32 n <= 86, u64 n <= 328,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>

/* seq_t and idx_t are supplied by the build system. */
static seq_t *dp = NULL;
static idx_t dp_size = 0;

seq_t a(idx_t n)
{
    /* Allocate or resize the DP array if necessary. */
    if (n + 1 > dp_size) {
        if (dp) free(dp);
        dp = (seq_t *)calloc((size_t)(n + 1), sizeof(seq_t));
        dp_size = n + 1;
    } else {
        /* Zero the relevant part of the array. */
        memset(dp, 0, sizeof(seq_t) * (size_t)(n + 1));
    }

    /* Initial condition: start with the empty subset (sum 0). */
    dp[0] = 1;

    /* For each exponent i (0 through n) we have two distinct
     * roots ±p^i, each contributing a weight i to the total sum.
     * We process each of the two copies independently. */
    for (idx_t i = 0; i <= n; ++i) {
        for (int copy = 0; copy < 2; ++copy) {
            /* Update dp in reverse order to avoid double counting. */
            for (idx_t s = n; ; --s) {
                dp[s] += dp[s - i];
                if (s == i) break;
            }
        }
    }

    /* Sum all subsets with total exponent sum ≤ n. */
    seq_t total = 0;
    for (idx_t s = 0; s <= n; ++s) {
        total += dp[s];
    }

    /* Remove the empty subset from the count. */
    total -= 1;

    return total;
}
