/*
 * A293895  Number of proper divisors of n of the form 3k+1.
 * https://oeis.org/A293895
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = number of proper divisors of n that are ≡ 1 (mod 3)
 * Range:     u8 n <= 20000, u16 n <= 20000, u32 n <= 20000, u64 n <= 20000,
 *            u128 n <= 20000, big any n
 * Validated: standard, pass, 121 terms through n = 9807 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h> /* malloc, calloc, free, abort */
#include "../utils/mem.h"

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static idx_t cache_size = 0;

    if (n == 0) return 0;          /* safety for zero index */

    if (n > cache_size) {
        idx_t new_size = n;
        seq_t *new_cache = mem_new(seq_t, new_size + 1);
        if (!new_cache) abort();  /* allocation failed */

        /* compute counts for all numbers up to new_size */
        for (idx_t d = 1; d < new_size; ++d) {
            if (d % (idx_t)3 == 1) {
                for (idx_t m = d * 2; m <= new_size; m += d)
                    ++new_cache[m];
            }
        }

        mem_free(cache);
        cache = new_cache;
        cache_size = new_size;
    }

    return cache[n];
}
