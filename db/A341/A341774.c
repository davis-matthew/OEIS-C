/*
 * A341774  Number of partitions of n into 3 nonzero tetrahedral numbers.
 * https://oeis.org/A341774
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of unordered triples of positive tetrahedral
 *            numbers that sum to n (offset 3)
 * Range:     u8 n <= 95, u16 n <= 95, u32 n <= 95, u64 n <= 95, u128 n <= 95,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *dp = NULL;          /* table of a(n) values up to current max   */
static size_t dp_cap = 0;         /* allocated size of dp                    */

/*  Re‑compute a(n) for all n ≤ max_n and store results in dp.  */
static void recompute_all(idx_t max_n)
{
    /*  Allocate or resize dp to hold indices 0 … max_n  */
    dp = (seq_t *)realloc(dp, (size_t)(max_n + 1) * sizeof(seq_t));
    if (!dp) abort();                          /* allocation failure */

    /*  Initialise all entries to 0  */
    for (size_t i = 0; i <= (size_t)max_n; ++i) dp[i] = 0;

    /*  Collect all tetrahedral numbers ≤ max_n.  */
    /*  There are far fewer than 80 of them for max_n ≤ 10000.  */
    seq_t tet[80];
    size_t len = 0;
    for (idx_t k = 1;; ++k) {
        seq_t t = ((seq_t)k * ((seq_t)k + 1) * ((seq_t)k + 2)) / 6;
        if (t > max_n) break;
        tet[len++] = t;
    }

    /*  Count unordered triples (i ≤ j ≤ k).  */
    for (size_t i = 0; i < len; ++i) {
        for (size_t j = i; j < len; ++j) {
            seq_t sum2 = tet[i] + tet[j];
            if (sum2 > max_n) break;                 /* increasing j  */
            for (size_t k = j; k < len; ++k) {
                seq_t sum = sum2 + tet[k];
                if (sum > max_n) break;              /* increasing k  */
                /*  sum fits in idx_t because sum ≤ max_n  */
                idx_t idx = (idx_t)sum;
                dp[idx] += (seq_t)1;
            }
        }
    }
}

seq_t a(idx_t n)
{
    if (n < 3) return 0;            /* values below the offset are irrelevant */

    if (dp_cap < (size_t)(n + 1)) {  /* not yet computed up to n */
        recompute_all(n);
        dp_cap = (size_t)(n + 1);
    }
    return dp[(size_t)n];
}
