/*
 * A358034  Numbers k such that A234575(k,s) = s^2 where s = A007953(k).
 * https://oeis.org/A358034
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, fini, full
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 41, u32 n <= 41, u64 n <= 41, u128 n <= 41,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
   This function implements OEIS A358034.
   It generates numbers k such that for the digit sum s = A007953(k)
   the quotient q and remainder r from k / s satisfy q + r = s^2.
   The algorithm searches numbers in increasing order and memoises
   previously found terms, so repeated calls for arbitrary indices are
   efficient.
*/

#include <stdlib.h>
#include <stddef.h>

/* helper: compute digit sum of a seq_t value */
static seq_t digit_sum(seq_t x) {
    seq_t sum = 0;
    while (x != 0) {
        seq_t d = x % (seq_t)10;
        sum += d;
        x /= (seq_t)10;
    }
    return sum;
}

seq_t a(idx_t n) {
    /* static state for memoisation */
    static seq_t *cache = NULL;
    static size_t cache_cnt = 0;
    static size_t cache_cap = 0;
    static seq_t last_checked = 0;  /* last number examined */

    /* if requested term already computed, return it */
    if (n <= (idx_t)cache_cnt)
        return cache[n - 1];

    /* initialise cache if first call */
    if (cache == NULL) {
        cache_cap = 64;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (!cache) return 0;  /* failed allocation: return zero */
    }

    /* search for more terms */
    seq_t cand = last_checked + 1;
    while (cache_cnt < (size_t)n) {
        seq_t s = digit_sum(cand);
        if (s != 0) {
            seq_t q = cand / s;
            seq_t r = cand % s;
            if (q + r == s * s) {
                /* store found term */
                if (cache_cnt == cache_cap) {
                    size_t new_cap = cache_cap * 2;
                    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
                    if (!tmp) return 0;  /* allocation failed */
                    cache = tmp;
                    cache_cap = new_cap;
                }
                cache[cache_cnt++] = cand;
            }
        }
        cand += (seq_t)1;
    }

    last_checked = cand - (seq_t)1;
    return cache[n - 1];
}
