/*
 * A258442  9-gonal numbers (A001106) that are the sum of eight consecutive
 *          9-gonal numbers.
 * https://oeis.org/A258442
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 1536796803*a(n-1) - 1536796803*a(n-2) + a(n-3) with
 *            initial values a(1) = 2484, a(2) = 3706711688304, a(3) =
 *            5696462668411740751524 (Sequence A258442)
 * Range:     u8 none, u16 n <= 1, u32 n <= 1, u64 n <= 2, u128 n <= 4,
 *            big any n
 * Helpers:   grow_seq_array, seq_from_str
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include "../utils/seq_from_str.h"    /* for seq_from_str to initialise a(3) */
#include "../utils/grow_seq_array.h"  /* for grow_seq_array to grow cache */

seq_t a(idx_t n)
{
    /* static cache of computed terms */
    static seq_t *cache = NULL;
    static size_t size   = 0;   /* current array size (allocated slots)   */
    static idx_t max_comp = 0;  /* largest index that has been computed    */
    const seq_t COEF = (seq_t)1536796803;

    if (n == 0) { /* Sequence is defined from index 1; return 0 for safety */
        return 0;
    }

    /* Ensure we can store up to index n */
    if (size < (size_t)n + 1) {
        grow_seq_array(&cache, &size, (size_t)n + 1);
    }

    /* Initialise the first three terms if not already done */
    if (max_comp < 3) {
        cache[1] = (seq_t)2484;
        cache[2] = (seq_t)3706711688304ULL;
        cache[3] = seq_from_str("5696462668411740751524");
        max_comp = 3;
    }

    /* Compute any missing terms up to n */
    for (idx_t i = max_comp + 1; i <= n; ++i) {
        cache[i] = COEF * cache[i - 1] - COEF * cache[i - 2] + cache[i - 3];
    }

    max_comp = n;
    return cache[n];
}
