/*
 * A267678  Decimal representation of the n-th iteration of the "Rule 197"
 *          elementary cellular automaton starting with a single ON (black)
 *          cell.
 * https://oeis.org/A267678
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 15, u64 n <= 31, u128 n <= 63,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  Sequence A267678: decimal representation of the n‑th iteration
    of Rule 197 elementary cellular automaton starting with a single
    ON (black) cell.
    Formula (simple linear recurrence, valid for n > 5):
        a(n) = 6*a(n-1) - 7*a(n-2) - 6*a(n-3) + 8*a(n-4)
    Initial values:
        a(0) = 1, a(1) = 2, a(2) = 20, a(3) = 104,
        a(4) = 468, a(5) = 1960
    The function stores computed values in a static cache to support
    arbitrary call order.  All arithmetic is performed using the
    provided seq_t type – it can be a native unsigned integer
    (uint8_t … uint64_t) or an arbitrary‑precision integer class.
*/

#include <stdlib.h>

static seq_t *cache = NULL;      /* cache of computed terms */
static size_t cache_len = 0;     /* number of terms currently stored */

/* initial values for n = 0 … 5 */
static const seq_t init_vals[6] = {1, 2, 20, 104, 468, 1960};

seq_t a(idx_t n)
{
    /* Handles the trivial cases directly */
    if (n <= 5)
        return init_vals[n];

    /* Ensure cache is large enough and initialized */
    if (cache == NULL) {
        size_t cap = 64;
        cache = (seq_t *)malloc(cap * sizeof(seq_t));
        if (cache == NULL) abort();          /* allocation failure */
        for (size_t i = 0; i < 6; ++i)
            cache[i] = init_vals[i];
        cache_len = 6;
    }

    size_t idx = (size_t)n;

    if (idx >= cache_len) {
        /* Grow cache if necessary */
        if (idx >= cache_len) {
            size_t new_cap = cache_len ? cache_len * 2 : 64;
            if (new_cap <= idx + 1)
                new_cap = idx + 1;
            seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
            if (tmp == NULL) abort();
            cache = tmp;
        }

        /* Compute terms up to idx using the recurrence relation */
        for (size_t i = cache_len; i <= idx; ++i) {
            seq_t t1 = 6 * cache[i - 1];
            seq_t t2 = 7 * cache[i - 2];
            seq_t t3 = 6 * cache[i - 3];
            seq_t t4 = 8 * cache[i - 4];

            /* compute t1 + t4 - t2 - t3 safely */
            seq_t res = t1 + t4;
            res = res - t2;
            res = res - t3;
            cache[i] = res;
        }
        cache_len = idx + 1;
    }

    return cache[idx];
}
