/*
 * A306377  a(n) = n^(n+1) - Sum_{k=1..n-1} k^(k+1).
 * https://oeis.org/A306377
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n^(n+1) - Sum_{k=1..n-1} k^(k+1)
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 9, u64 n <= 15, u128 n <= 18,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    /* static cache for prefix sums S(k) = Σ_{i=1..k} i^(i+1) */
    static seq_t *prefix = NULL;
    static size_t prefix_cap = 0;
    static idx_t prefix_len = 0;  /* highest k with S(k) stored (prefix_len = 0 -> only S(0)=0) */

    /* initialise cache */
    if (prefix == NULL) {
        prefix_cap = 16;
        prefix = (seq_t *)malloc(prefix_cap * sizeof(seq_t));
        if (!prefix) return 0;           /* allocation failed */
        prefix[0] = (seq_t)0;           /* S(0) = 0 */
        prefix_len = 0;
    }

    /* extend cache up to k = n-1 (if needed) */
    while (prefix_len < n-1) {
        idx_t i = prefix_len + 1;
        seq_t term = big_pow((seq_t)i, (seq_t)(i+1));
        seq_t new_sum = prefix[prefix_len] + term;

        if (prefix_cap <= prefix_len + 1) {   /* need more space */
            size_t new_cap = prefix_cap * 2;
            if (new_cap <= prefix_len + 1) new_cap = prefix_len + 2;
            seq_t *tmp = (seq_t *)realloc(prefix, new_cap * sizeof(seq_t));
            if (!tmp) return 0;   /* allocation failed */
            prefix = tmp;
            prefix_cap = new_cap;
        }
        prefix[prefix_len + 1] = new_sum;
        prefix_len++;
    }

    /* compute a(n) = n^(n+1) - S(n-1) */
    seq_t term_n = big_pow((seq_t)n, (seq_t)(n+1));
    return term_n - prefix[n-1];
}
