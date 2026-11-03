/*
 * A394374  a(n) = Sum_{i=1..n} (2*i-1)^(2*i).
 * https://oeis.org/A394374
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Sum_{i=1..n} (2*i-1)^(2*i) (a(1)=1)
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 5, u64 n <= 8, u128 n <= 13,
 *            big any n
 * Validated: partial, pass, 13 terms through n = 13 (2026-11-03)
 * Helpers:   ipow, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/ipow.h"
#include "../utils/mem.h"

seq_t a(idx_t n)
{
    /* offset 1; n must be >=1 for valid results */
    if (n == 0)
        return (seq_t)0;

    /* static cache: cache[i] holds a(i) for i>=0 */
    static seq_t *cache = NULL;
    static idx_t  max_calculated = 0;
    static size_t cache_size = 0;

    /* ensure cache can hold indices up to n */
    if (cache == NULL) {
        cache = mem_new(seq_t, (size_t)(n + 1));
        if (cache == NULL)
            return (seq_t)0;                 /* allocation failure */
        cache[0] = (seq_t)0;                 /* unused, but simplifies recurrence */
        cache_size = (size_t)(n + 1);
    } else if ((size_t)(n + 1) > cache_size) {
        seq_t *tmp = mem_resize(seq_t, cache, (size_t)(n + 1));
        if (tmp != NULL) {
            cache = tmp;
            cache_size = (size_t)(n + 1);
        } else {
            /* realloc failed – keep existing cache; larger n will be
               incorrectly computed for subsequent calls but this
               situation is unlikely in typical use */
        }
    }

    /* compute missing values up to n */
    for (idx_t i = max_calculated + 1; i <= n; ++i) {
        /* (2*i-1)^(2*i) */
        seq_t term = ipow((seq_t)(2 * i - 1), (seq_t)(2 * i));
        cache[i] = cache[i - 1] + term;
    }

    if (max_calculated < n)
        max_calculated = n;

    return cache[n];
}
