/*
 * A301347  a(n) = n^(n-1) + (n-1)!.
 * https://oeis.org/A301347
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n^(n-1) + (n-1)!
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 10, u64 n <= 16, u128 n <= 27,
 *            big any n
 * Helpers:   big_pow, mul_small
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/mul_small.h"
#include "../utils/big_pow.h"

/* cache factorials: fact_cache[i] = i!   for i >= 0 */
static seq_t *fact_cache = NULL;
static size_t fact_cap   = 0;   /* capacity of the cache array          */
static size_t fact_max   = 0;   /* highest index computed in the cache   */

seq_t a(idx_t n)
{
    /* n should be >= 1; for safety allow n==0 */
    if (n == 0) return 0;

    /* initialise cache on first call */
    if (fact_cache == NULL) {
        fact_cache = (seq_t *)malloc(sizeof(seq_t));
        if (!fact_cache) abort();
        fact_cache[0] = 1;               /* 0! = 1 */
        fact_cap   = 1;
        fact_max   = 0;
    }

    /* target factorial index: (n-1)! */
    idx_t target = n - 1;

    /* Extend cache capacity if required */
    if ((size_t)target > fact_max) {
        size_t needed = (size_t)target + 1;
        if (needed > fact_cap) {
            size_t new_cap = fact_cap ? fact_cap * 2 : 1;
            while (new_cap < needed) new_cap <<= 1;
            seq_t *tmp = (seq_t *)realloc(fact_cache, new_cap * sizeof(seq_t));
            if (!tmp) abort();
            fact_cache = tmp;
            fact_cap   = new_cap;
        }

        /* Compute missing factorials */
        for (size_t i = fact_max + 1; i <= (size_t)target; ++i) {
            seq_t val = mul_small(fact_cache[i - 1], (unsigned int)i);
            fact_cache[i] = val;
        }
        fact_max = (size_t)target;
    }

    seq_t fact_n1 = fact_cache[target];          /* (n-1)! */
    seq_t pow_val = big_pow((seq_t)n, (seq_t)target);   /* n^(n-1) */

    return fact_n1 + pow_val;
}
