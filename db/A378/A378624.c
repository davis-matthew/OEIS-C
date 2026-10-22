/*
 * A378624  Numbers whose binary representation interpreted as being base-3 is
 *          divisible by 5.
 * https://oeis.org/A378624
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) is the nth non‑negative integer whose binary
 *            representation, interpreted as a base‑3 number, is divisible by
 *            5
 * Range:     u8 n <= 52, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    /* Pattern of 3^i modulo 5 (period 4): 1,3,4,2 */
    static const uint8_t pat[4] = {1, 3, 4, 2};

    /* Cache of previously found terms */
    static seq_t *cache = NULL;
    static size_t cache_cap = 0;
    static idx_t terms_found = 0;   /* number of terms stored in cache */
    static idx_t next_candidate = 0;/* next integer to test */

    /* Grow cache lazily */
    if (cache == NULL) {
        cache_cap = 64;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (cache == NULL) return (seq_t)0; /* allocation failure */
    }

    /* Generate terms until we have the requested one */
    while (terms_found < n) {
        idx_t x = next_candidate;
        next_candidate++;

        /* Compute f(x) mod 5 where f(x) = Σ bit_i * 3^i (mod 5) */
        uint8_t r = 0;          /* residue modulo 5 */
        idx_t    t = x;         /* copy for bit scanning */
        int      pos = 0;       /* bit position */

        while (t) {
            if (t & (idx_t)1) {
                r += pat[pos & 3];
                if (r >= 5) r -= 5; /* keep r modulo 5 */
            }
            t >>= 1;
            ++pos;
        }

        /* x = 0 gives r=0, so it is included */
        if (r == 0) {
            /* Grow cache if necessary */
            if (terms_found == cache_cap) {
                size_t new_cap = cache_cap * 2;
                seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
                if (tmp == NULL) return (seq_t)0; /* allocation failure */
                cache = tmp;
                cache_cap = new_cap;
            }
            cache[terms_found] = (seq_t)x;
            ++terms_found;
        }
    }

    return cache[n - 1];
}
