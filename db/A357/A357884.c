/*
 * A357884  a(1)=0; if a(n-1) shares any digits with n-1, then a(n) = a(n-1)
 *          with all copies of digits from n-1 removed. Otherwise, a(n) =
 *          a(n-1) + (n-1).
 * https://oeis.org/A357884
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy, look
 * Formula:   not stated
 * Range:     u8 n <= 51, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   get_digits_local
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(1)=0; for n>1 let m=n-1.
   If a(n-1) shares any decimal digit with m then a(n) is a(n-1) with
   all copies of digits from m removed; otherwise a(n)=a(n-1)+m. */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/get_digits_local.h"

static seq_t *cache = NULL;   /* cache[1..] holds a(k) values          */
static idx_t cache_n = 0;     /* largest n for which cache[n] is valid */

/* Extend cache so that cache[n] is valid for all n <= desired_n. */
static void compute_upto(idx_t desired_n)
{
    if (desired_n <= cache_n)
        return;

    /* Allocate space for indices 1..desired_n (index 0 unused). */
    size_t new_cap = (size_t)desired_n + 1;
    seq_t *p = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!p) abort();
    cache = p;

    if (cache_n == 0) {
        cache[1] = (seq_t)0;   /* a(1) = 0 */
        cache_n   = 1;
    }

    for (idx_t k = cache_n + 1; k <= desired_n; ++k) {
        seq_t prev = cache[k - 1];
        idx_t  idx_m = k - 1;            /* m = n-1, as idx_t */
        seq_t m   = (seq_t)idx_m;        /* for arithmetic with a(n-1) */

        /* --- build mask of digits appearing in m --- */
        unsigned char mask_m[10] = {0};
        if (idx_m == 0) {
            mask_m[0] = 1;
        } else {
            idx_t tmp = idx_m;
            while (tmp > 0) {
                unsigned d = (unsigned)(tmp % 10);
                mask_m[d] = 1;
                tmp /= 10;
            }
        }

        /* --- obtain digits of prev and build mask --- */
        uint8_t prev_digits[256];
        size_t prev_len;
        get_digits_local(prev, prev_digits, &prev_len);

        unsigned char mask_prev[10] = {0};
        for (size_t i = 0; i < prev_len; ++i) {
            unsigned d = (unsigned)prev_digits[i];
            mask_prev[d] = 1;
        }

        /* --- check whether any digit is shared --- */
        int share = 0;
        for (int d = 0; d < 10; ++d) {
            if (mask_m[d] && mask_prev[d]) {
                share = 1;
                break;
            }
        }

        if (!share) {
            /* a(n) = a(n-1) + m */
            cache[k] = prev + m;
        } else {
            /* remove from prev any digit that appears in m */
            seq_t res = (seq_t)0;
            for (size_t i = prev_len; i-- > 0; ) {   /* most significant first */
                unsigned d = (unsigned)prev_digits[i];
                if (!mask_m[d]) {
                    res = res * (seq_t)10 + (seq_t)d;
                }
            }
            cache[k] = res;
        }
    }
    cache_n = desired_n;
}

seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;     /* safety: offsets start at 1 */
    compute_upto(n);
    return cache[n];
}
