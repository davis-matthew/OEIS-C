/*
 * A305931  Powers of 3 having at least one digit '0' in their decimal
 *          representation.
 * https://oeis.org/A305931
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 1, u32 n <= 6, u64 n <= 19, u128 n <= 19,
 *            big any n
 * Helpers:   seq_alloc, seq_mul, seq_realloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Searches successive powers of 3 for a decimal digit 0 and caches
   the results so that a(n) can be retrieved in order of the
   OEIS index.  a(1) is the first power 3^k that contains a zero digit. */

#include <stdbool.h>
#include "../utils/seq_mul.h"     /* seq_mul */
#include "../utils/seq_alloc.h"   /* seq_alloc */
#include "../utils/seq_realloc.h" /* seq_realloc */

seq_t a(idx_t n)
{
    /* Protect against calls with index 0 – the behavior is
       irrelevant for indices below the offset.  */
    if (n == 0) return 0;

    /* Static state – cached results and current exponent. */
    static seq_t *cache   = NULL;   /* array of found powers of 3 */
    static size_t  cap    = 0;      /* capacity of the cache array */
    static idx_t   max_n  = 0;      /* number of terms cached so far */
    static idx_t   exp    = 0;      /* exponent of the last processed power */
    static seq_t   cur    = 1;      /* current power 3^exp */

    /* Initialise on first call. */
    if (cache == NULL) {
        cap   = 64;
        cache = seq_alloc(cap);
        max_n = 0;
        exp   = 0;
        cur   = 1;      /* 3^0 */
    }

    /* Build up the cache until we have at least n terms. */
    while (max_n < n) {
        /* Advance to the next power of 3. */
        exp++;
        cur = seq_mul(cur, 3);   /* cur = 3^exp */

        /* Check if cur contains a zero decimal digit. */
        bool has_zero = false;
        seq_t tmp = cur;
        /* tmp cannot be zero except for possible overflow; we skip it. */
        while (tmp != 0) {
            if (tmp % 10 == 0) {
                has_zero = true;
                break;
            }
            tmp /= 10;
        }

        if (has_zero) {
            /* Grow cache if needed. */
            if ((size_t)max_n >= cap) {
                size_t new_cap = cap * 2;
                cache = seq_realloc(cache, cap, new_cap);
                cap   = new_cap;
            }
            cache[max_n] = cur;
            ++max_n;
        }
    }

    return cache[n - 1];
}
