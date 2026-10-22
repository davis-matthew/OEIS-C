/*
 * A372591  Numbers whose binary weight (A000120) plus bigomega (A001222) is
 *          even.
 * https://oeis.org/A372591
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the n-th positive integer m such that
 *            popcount(m)+bigomega(m) is even
 * Range:     u8 n <= 63, u16 n <= 63, u32 n <= 63, u64 n <= 63, u128 n <= 63,
 *            big any n
 * Helpers:   bigomega
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>
#include "../utils/bigomega.h"

seq_t a(idx_t n)
{
    /* Cache of previous values – enables arbitrary calling order */
    static seq_t *cache = NULL;   /* array of seq_t values */
    static size_t cap   = 0;      /* capacity of the cache array   */
    static size_t len   = 0;      /* number of values cached       */
    static idx_t  next  = 1;      /* last integer examined (1‑based) */

    /* Grow cache until we have at least n elements */
    while (len < n) {
        idx_t x = ++next;                     /* candidate integer */

        /* ----- popcount parity ----- */
        uint8_t pop_parity = 0;
        idx_t t = x;
        /* fast popcount using Brian Kernighan’s trick */
        while (t) { pop_parity ^= 1; t &= t - 1; }

        /* ----- bigomega parity ----- */
        seq_t bo = bigomega(x);               /* bigomega is seq_t */
        uint8_t bo_parity = (bo & 1) ? 1 : 0;

        /* Accept if the sum of parities is even (xor zero) */
        if ((pop_parity ^ bo_parity) == 0) {
            if (len == cap) {                 /* double capacity if full */
                size_t new_cap = cap ? cap * 2 : 16;
                seq_t *tmp = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
                if (!tmp) abort();            /* allocation failure */
                cache = tmp;
                cap   = new_cap;
            }
            cache[len++] = (seq_t)x;           /* store in cache */
        }
    }

    return cache[n - 1];                         /* 1‑based offset */
}
