/*
 * A262950  Expansion of q * chi(-q^3) * chi(-q^13) / (chi(-q) * chi(-q^39))
 *          in powers of q where chi() is a Ramanujan theta function.
 * https://oeis.org/A262950
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 48, u16 n <= 129, u32 n <= 390, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Formula: a(n) = coefficient of q^n in q * chi(-q^3) * chi(-q^13) / (chi(-q) * chi(-q^39))
 * Equivalent combinatorial interpretation: a(n) equals the number of partitions of n‑1
 * into parts that are NOT divisible by 2, 3, or 13.
 * The algorithm builds the partition numbers by dynamic programming,
 * recomputing the array from scratch whenever a larger n is requested.
 */

#include <stdlib.h>

seq_t a(idx_t n) {
    /* Static storage for partition numbers up to the largest n-1 computed so far. */
    static seq_t *dp = NULL;       /* dp[i] = #partitions of i with allowed parts */
    static idx_t dp_size = 0;      /* current allocated size (covers indices 0..dp_size-1) */
    static idx_t dp_max = 0;       /* largest index computed (n-1) so far */

    if (n == 0) return 0;          /* undefined for n=0, return 0 as a safeguard */

    idx_t m = n - 1;               /* we need partitions of m */

    /* Ensure the table is large enough and recompute if we have not yet seen this m. */
    if (m >= dp_size) {
        /* Allocate a new array of size m+1. */
        seq_t *tmp = (seq_t *)realloc(dp, (m + 1) * sizeof(seq_t));
        if (tmp == NULL) return 0;          /* allocation failed */
        dp = tmp;
        dp_size = m + 1;

        /* Initialise all entries to 0 and set dp[0] = 1 (empty partition). */
        for (idx_t i = 0; i <= m; ++i) dp[i] = 0;
        dp[0] = 1;
    }

    /* If we have never computed up to m, or we are recomputing after an expansion,
     * perform the partition DP from scratch up to m. */
    if (m > dp_max) {
        /* Re‑zero entries that were not part of the old table. */
        for (idx_t i = dp_max + 1; i <= m; ++i) dp[i] = 0;

        /* Dynamic programming for partitions with allowed parts (not divisible by 2,3,13). */
        for (idx_t part = 1; part <= m; ++part) {
            /* Skip forbidden parts. */
            if ((part & 1) == 0 || part % 3 == 0 || part % 13 == 0) continue;

            /* For each allowed part, add its contributions to larger indices. */
            for (idx_t i = part; i <= m; ++i) {
                dp[i] = dp[i] + dp[i - part];
            }
        }
        dp_max = m;   /* we have now calculated up to m */
    }

    return dp[m];
}
