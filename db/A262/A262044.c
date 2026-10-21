/*
 * A262044  Partial sum of the first n odd composite numbers.
 * https://oeis.org/A262044
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Sum_{i=1..n} A071904(i) (sum of the first n odd composite
 *            numbers)
 * Range:     u8 n <= 9, u16 n <= 198, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>
#include <stdint.h>
#include "../utils/is_prime.h"

seq_t a(idx_t n)
{
    /* Memoization state */
    static seq_t *cache   = NULL;    /* partial sums for 1..cache_len */
    static size_t cache_cap = 0;     /* current allocated capacity of cache   */
    static size_t cache_len = 0;     /* number of computed terms stored       */
    static uint64_t last_checked = 7;/* odd number whose primality was last tested (next will be 9)*/
    static seq_t sum_so_far   = 0;   /* cumulative sum of odd composites found */

    /* Offset 1: undefined for n==0 – return 0 gracefully */
    if (n == 0)
        return 0;

    /* Convert n to size_t for array access – n is expected to be small in practice */
    size_t target = (size_t)n;

    /* Return cached value if already computed */
    if (cache_len >= target)
        return cache[target - 1];

    /* Grow the cache if necessary */
    if (cache_cap < target) {
        size_t new_cap = cache_cap > 0 ? cache_cap << 1 : 64;
        if (new_cap < target)
            new_cap = target;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) {
            /* Allocation failure – terminate the program */
            abort();
        }
        cache = tmp;
        cache_cap = new_cap;
    }

    /* Compute missing terms */
    for (size_t i = cache_len; i < target; ++i) {
        /* Find the next odd composite number */
        while (1) {
            last_checked += 2;                     /* skip to next odd */
            if (last_checked <= 1) continue;       /* ignore 1 (not composite) */
            if (!is_prime(last_checked)) break;    /* composite found */
        }

        /* Update the running sum and store the partial sum */
        sum_so_far += (seq_t)last_checked;
        cache[i] = sum_so_far;
    }

    cache_len = target;
    return cache[target - 1];
}
