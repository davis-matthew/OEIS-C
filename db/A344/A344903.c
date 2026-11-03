/*
 * A344903  a(n) is the number of optimal strategies for Player I in the
 *          Penney-Ante game with strings of length n.
 * https://oeis.org/A344903
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2*a(n-1) - (-1)^n * a(floor(n/2)+1) Initial values:
 *            a(3)=4, a(4)=2, a(5)=2
 * Range:     u8 n <= 12, u16 n <= 20, u32 n <= 36, u64 n <= 68, u128 n <= 132,
 *            big any n
 * Validated: standard, pass, 52 terms through n = 3310 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>
#include "../utils/mem.h"

/* Make sure the cache is large enough */
static void ensure_cache(idx_t n, seq_t **cache_ptr, size_t *cap_ptr) {
    seq_t *cache = *cache_ptr;
    size_t cap = *cap_ptr;
    if (cap <= (size_t)n) {
        size_t new_cap = cap ? (cap << 1) : 16;
        while (new_cap <= (size_t)n)
            new_cap <<= 1;
        seq_t *tmp = mem_resize(seq_t, cache, new_cap);
        if (!tmp && new_cap > 0) {
            /* Allocation failed – abort the program. */
            exit(1);
        }
        if (tmp) {
            /* Initialize newly allocated slots to zero. */
            if (!cache) {
                for (size_t i = 0; i < new_cap; ++i)
                    tmp[i] = 0;
            } else {
                for (size_t i = cap; i < new_cap; ++i)
                    tmp[i] = 0;
            }
            *cache_ptr = tmp;
            *cap_ptr = new_cap;
        }
    }
}

seq_t a(idx_t n)
{
    if (n < 3) return 0;                      /* undefined part of the sequence */

    static seq_t *cache = NULL;
    static size_t cache_cap = 0;
    ensure_cache(n, &cache, &cache_cap);       /* allocate enough space, zeroed */

    if (cache[n] != 0) return cache[n];        /* memoized value */

    seq_t result;
    if (n == 3)          result = 4;
    else if (n == 4)     result = 2;
    else if (n == 5)     result = 2;
    else {
        seq_t prev = a(n - 1);
        idx_t half = (n >> 1) + 1;             /* floor(n/2)+1 */
        seq_t mid  = a(half);
        if ((n & 1) == 0)                     /* n even: (-1)^n = 1 */
            result = 2 * prev - mid;
        else                                   /* n odd: (-1)^n = -1 */
            result = 2 * prev + mid;
    }

    cache[n] = result;
    return result;
}
