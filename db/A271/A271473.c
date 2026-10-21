/*
 * A271473  a(1)=0; thereafter a(2k)=k+a(k), a(2k+1)=k+a(6k+4).
 * https://oeis.org/A271473
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 14, u16 n <= 326, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence:
   a(1) = 0;
   a(2k)   = k + a(k);
   a(2k+1) = k + a(6k + 4);
*/
#include <stdlib.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    /* Allocate a small, expandable cache for computed values.
       cache[n] holds a(n); known[n] flags whether a(n) is cached.
    */
    static seq_t *cache = NULL;
    static unsigned char *known = NULL;
    static idx_t cap = 0;        /* capacity of the arrays (>= largest n cached +1) */

    /* Grow cache to accommodate index n (1‑based). */
    if (n >= cap) {
        idx_t new_cap = n + 1;
        cache  = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        known  = (unsigned char *)realloc(known, new_cap * sizeof(unsigned char));
        /* If realloc failed, terminate – the framework guarantees enough space. */
        if (!cache || !known) abort();
        /* Initialize newly allocated part to zeros. */
        for (idx_t i = cap; i < new_cap; ++i) known[i] = 0;
        cap = new_cap;
    }

    /* Return cached value if available. */
    if (known[n]) return cache[n];

    /* Compute the value according to the recurrence. */
    seq_t val;
    if (n == 1) {
        val = 0;
    } else if ((n & 1ULL) == 0) {          /* n is even: n = 2k */
        idx_t k = n >> 1;
        val = (seq_t)k + a(k);
    } else {                               /* n is odd: n = 2k+1 */
        idx_t k = (n - 1) >> 1;
        val = (seq_t)k + a((idx_t)(6 * k + 4));
    }

    /* Cache and return. */
    cache[n] = val;
    known[n] = 1;
    return val;
}
