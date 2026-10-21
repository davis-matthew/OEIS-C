/*
 * A290386  Increasing sequence a(n)>a(n-1) where a(n)=smallest integer not
 *          yet in the sequence with no digits shared with the previous term
 *          a(n-1), no repeated digits, and no 0-digit allowed.
 * https://oeis.org/A290386
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, fini, full
 * Formula:   not stated
 * Range:     u8 n <= 17, u16 n <= 27, u32 n <= 27, u64 n <= 27, u128 n <= 27,
 *            big any n
 * Helpers:   _digits_mask
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Term-by-term generation: 
   a(1)=1; for n>1 a(n)=smallest integer > a(n-1) not containing 0,
   with no repeated digits, and with no digit common with a(n-1). */
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/_digits_mask.h"

/* Return the n-th term of A290386.  Cached terms are stored in a static array. */
seq_t a(idx_t n)
{
    static seq_t *cache = NULL;      /* allocated array of terms             */
    static size_t  cap   = 0;        /* current capacity of cache array      */
    static size_t  len   = 0;        /* number of terms currently stored     */

    /* Extend the cache until we have at least n terms. */
    while (len < n) {
        /* Grow the cache if necessary. */
        if (len == cap) {
            size_t new_cap = (cap == 0) ? 8 : cap * 2;
            seq_t *newptr = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
            if (!newptr) exit(1);   /* allocation failed */
            cache = newptr;
            cap   = new_cap;
        }

        /* First term is 1. */
        if (len == 0) {
            cache[0] = (seq_t)1;
            len = 1;
            continue;
        }

        /* Compute the next term. */
        seq_t prev      = cache[len - 1];
        unsigned int pmask = _digits_mask(prev);

        seq_t cand = prev + 1;
        for (;;) {
            seq_t tmp   = cand;
            unsigned int cmask = 0;
            bool bad = false;

            while (tmp > 0) {
                int d = (int)(tmp % 10);
                tmp /= 10;
                if (d == 0) {           /* zero digit not allowed */
                    bad = true;
                    break;
                }
                unsigned int bit = 1u << d;
                if (cmask & bit) {      /* repeated digit in the candidate */
                    bad = true;
                    break;
                }
                cmask |= bit;
            }

            if (!bad && ((cmask & pmask) == 0)) {
                cache[len++] = cand;
                break;                /* next term found */
            }
            ++cand;                    /* try next integer */
        }
    }

    return cache[n - 1];
}
