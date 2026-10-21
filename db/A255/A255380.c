/*
 * A255380  Number of strings of n decimal digits that contain at least one
 *          string of exactly 10 consecutive "0" digits.
 * https://oeis.org/A255380
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   for n < 10 a(n)=0, a(10)=1; for n >= 11
 *            a(n)=9*(10^(n-11)-a(n-11)+sum_{i=10}^{n-1} a(i))
 * Range:     u8 n <= 11, u16 n <= 14, u32 n <= 18, u64 n <= 28, u128 n <= 47,
 *            big any n
 * Helpers:   pow10
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The implementation memoizes a(n) so that subsequent calls for any n
   (in any order) are O(1) after the first computation.  The recurrence
   is implemented exactly as stated, using pow10() for 10^(k).  All
   arithmetic is performed in seq_t, which may be an arbitrary‑precision
   integer.  No external libraries beyond the standard headers are
   required.  */

#include <stdlib.h>
#include "../utils/pow10.h"   /* provides seq_t pow10(idx_t e) */

seq_t a(idx_t n)
{
    /* static memoization storage */
    static seq_t *cache      = NULL;   /* a[i] */
    static seq_t *sum_cache  = NULL;   /* cumulative sum up to i */
    static size_t cap        = 0;      /* allocated size */
    static idx_t  max_comp   = (idx_t)-1;   /* highest index computed   */

    /* If we've already computed up to n, simply return the value. */
    if (max_comp != (idx_t)-1 && max_comp >= n)
        return cache[n];

    /* Ensure storage can hold indices 0 … n */
    while (cap <= (size_t)n) {
        size_t new_cap = cap ? cap * 2 : 16;
        while (new_cap <= (size_t)n)
            new_cap <<= 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) abort();               /* allocation failure */
        cache = tmp;
        tmp = (seq_t *)realloc(sum_cache, new_cap * sizeof(seq_t));
        if (!tmp) abort();
        sum_cache = tmp;
        /* Initialise the new slots to zero. */
        for (size_t i = cap; i < new_cap; ++i) {
            cache[i]    = 0;
            sum_cache[i] = 0;
        }
        cap = new_cap;
    }

    /* Compute missing values from (max_comp+1) up to n. */
    idx_t start = (max_comp == (idx_t)-1) ? 0 : max_comp + 1;
    for (idx_t i = start; i <= n; ++i) {
        seq_t val;
        if (i < 10) {                     /* a(0..9) = 0 */
            val = 0;
        } else if (i == 10) {              /* a(10) = 1 */
            val = 1;
        } else {                           /* recurrence for i >= 11 */
            seq_t pow10_val = pow10(i - 11);              /* 10^(i-11) */
            seq_t inner = pow10_val - cache[i - 11];      /* -a(i-11)   */
            inner += sum_cache[i - 1] - sum_cache[9];     /* +Σ_{10}^{i-1} a(j) */
            val = 9 * inner;
        }
        cache[i] = val;
        /* Update cumulative sum: sum_cache[i] = sum_cache[i-1] + val */
        if (i == 0)
            sum_cache[i] = val;
        else
            sum_cache[i] = sum_cache[i - 1] + val;
    }

    max_comp = n;
    return cache[n];
}
