/*
 * A321459  Expansion of Product_{1 <= i <= j <= k} 1/(1 - x^(i^2 + j^2 +
 *          k^2)).
 * https://oeis.org/A321459
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 47, u16 n <= 65, u32 n <= 65, u64 n <= 65, u128 n <= 65,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A321459: expansion of
 *      Π_{1 ≤ i ≤ j ≤ k} 1/(1 - x^(i² + j² + k²))
 *  Returns the coefficient a(n) of xⁿ in this series.
 */

#include <stdlib.h>
#include <string.h>

/* Types seq_t and idx_t are supplied by the build.  They are
 * integer types; seq_t may be an unsigned integer or an
 * arbitrary‑precision class that supports the basic arithmetic
 * operators.  No other assumptions are made. */

/* ----------  Static data  ---------- */
static seq_t *dp      = NULL;   /* coefficients up to current limit   */
static size_t dp_size = 0;      /* number of allocated slots           */
static idx_t *carr    = NULL;   /* multiplicities c(m)                 */
static size_t carr_size = 0;    /* number of allocated slots of carr   */

/* ----------  Recompute data up to limit ---------- */
static void recompute_up_to(idx_t limit)
{
    /* Allocate / reallocate coefficient array */
    size_t new_dp_size = (size_t)(limit + 1);
    seq_t *new_dp = (seq_t*)realloc(dp, new_dp_size * sizeof(seq_t));
    if (!new_dp) return;                       /* out of memory – ignore */
    dp = new_dp;
    dp_size = new_dp_size;

    /* Zero all entries and set dp[0] = 1 */
    for (idx_t i = 0; i <= limit; ++i) dp[i] = 0;
    dp[0] = 1;

    /* Allocate / reallocate multiplicity array */
    size_t new_carr_size = (size_t)(limit + 1);
    idx_t *new_carr = (idx_t*)realloc(carr, new_carr_size * sizeof(idx_t));
    if (!new_carr) return; /* out of memory – ignore */
    carr = new_carr;
    carr_size = new_carr_size;
    for (idx_t i = 0; i <= limit; ++i) carr[i] = 0;

    /* Compute multiplicities c(m) for all m ≤ limit.
     * Enumerate all unordered triples i ≤ j ≤ k with i² + j² + k² = m. */
    for (idx_t i = 1; i * i <= limit; ++i) {
        idx_t i2 = i * i;
        for (idx_t j = i; i2 + j * j <= limit; ++j) {
            idx_t sumij = i2 + j * j;
            for (idx_t k = j; sumij + k * k <= limit; ++k) {
                idx_t t = sumij + k * k;
                ++carr[t];
            }
        }
    }

    /* Dynamic programming: for each part size m, apply the factor
     * (1 - x^m)^(-c(m)).  This is equivalent to treating each of the
     * c(m) identical factors 1/(1 - x^m) independently. */
    for (idx_t m = 1; m <= limit; ++m) {
        idx_t reps = carr[m];
        for (idx_t r = 0; r < reps; ++r) {
            for (idx_t s = m; s <= limit; ++s) {
                dp[s] += dp[s - m];
            }
        }
    }
}

/* ----------  Public sequence function ---------- */
seq_t a(idx_t n)
{
    if (n >= dp_size)                      /* need to extend data */
        recompute_up_to(n);
    return dp[n];
}
