/*
 * A273159  Numbers whose digit sum is divisible by 7.
 * https://oeis.org/A273159
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   a(n) = nth nonnegative integer whose digit sum is divisible by 7
 * Range:     u8 n <= 35, u16 n <= 9364, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   digit_sum
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/digit_sum.h"

/* Return the n‑th term (n ≥ 1) of A273159. */
seq_t a(idx_t n)
{
    /* Zero based index into the cache   */
    idx_t target;

    /* Stateless cache for memoisation     */
    static seq_t *cache      = NULL;   /* holds the terms          */
    static size_t cache_n    = 0;      /* number of terms stored   */
    static size_t cache_cap  = 0;      /* allocated capacity       */
    static seq_t next_val    = 0;      /* smallest number not yet tested */

    /* n == 0 is outside the defined domain – return 0 */
    if (n == 0)
        return 0;

    target = n - 1;   /* offset 1 → index 0 */

    /* Lazy allocation of the cache */
    if (cache == NULL) {
        cache_cap = 1024;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (cache == NULL)
            return 0;   /* allocation failure – unlikely in tests */
        cache_n = 0;
        next_val = 0;
    }

    /* Return cached value if already available */
    if (target < cache_n)
        return cache[target];

    /* Generate terms until the cache contains a(target) */
    while (cache_n <= target) {
        /* Search for the next integer whose digit sum ≡ 0 (mod 7) */
        while (true) {
            seq_t s = digit_sum(next_val);
            if ((s % 7U) == 0)
                break;
            next_val++;  /* try the next integer */
        }

        /* Ensure enough room in the cache */
        if (cache_n == cache_cap) {
            cache_cap <<= 1;  /* double capacity */
            cache = (seq_t *)realloc(cache, cache_cap * sizeof(seq_t));
            if (cache == NULL)
                return 0;   /* allocation failure */
        }

        /* Store the found term and advance to the next candidate */
        cache[cache_n++] = next_val;
        next_val++;
    }

    return cache[target];
}
