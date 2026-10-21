/*
 * A266755  Expansion of 1/((1-x^2)*(1-x^3)*(1-x^4)).
 * https://oeis.org/A266755
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = coefficient of x^n in 1/((1-x^2)(1-x^3)(1-x^4)), i.e. the
 *            number of partitions of n into parts 2, 3 and 4
 * Range:     u8 n <= 105, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   alloc_seq_array, seq_free_array
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/alloc_seq_array.h"
#include "../utils/seq_free_array.h"

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static idx_t cache_size = 0;

    if (n < cache_size)
        return cache[n];

    idx_t new_size = n + 1;
    seq_t *new_cache = alloc_seq_array(new_size);

    /* initialise all entries to zero */
    for (idx_t i = 0; i < new_size; ++i)
        new_cache[i] = 0;

    new_cache[0] = 1;          /* empty partition */

    /* count partitions using parts 2, 3 and 4; order of parts is fixed
       to avoid counting permutations multiple times. */
    for (idx_t part = 2; part <= 4; ++part) {
        for (idx_t i = part; i < new_size; ++i) {
            new_cache[i] += new_cache[i - part];
        }
    }

    if (cache != NULL)
        seq_free_array(cache);

    cache     = new_cache;
    cache_size = new_size;

    return cache[n];
}
