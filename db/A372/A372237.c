/*
 * A372237  a(0) = 4; to obtain a(k), write out the base-(2^k) expansion of
 *          a(k-1), bump to base 2^(k+1), then subtract 1.
 * https://oeis.org/A372237
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, fini
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 13, u32 n <= 29, u64 n <= 61, u128 n <= 126,
 *            big any n
 * Helpers:   pow2, seq_alloc_new_array
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(0)=4; for k>0, a(k) = interpret the base-2^k digits of a(k-1)
   in base-2^(k+1) and then subtract 1. */
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include "../utils/seq_alloc_new_array.h"
#include "../utils/pow2.h"

/* Cache for memoised terms. */
static seq_t *cache      = NULL;  /* array of cached values */
static size_t cache_cap  = 0;     /* allocated capacity (in elements) */
static size_t cache_len  = 0;     /* number of values currently cached */

/* Ensure the cache can hold indices up to n. */
static void ensure_cache(idx_t n)
{
    if ((size_t)n < cache_cap) return;
    size_t new_cap = (size_t)n + 1;
    seq_t *tmp = seq_alloc_new_array(new_cap);
    if (cache) {
        memcpy(tmp, cache, cache_len * sizeof(seq_t));
        free(cache);
    }
    cache = tmp;
    cache_cap = new_cap;
}

/* Main sequence function. */
seq_t a(idx_t n)
{
    /* initialise a(0) if this is the first call */
    if (cache_len == 0) {
        ensure_cache(0);
        cache[0] = (seq_t)4;
        cache_len = 1;
    }

    if ((size_t)n >= cache_len) {
        ensure_cache(n);
        for (idx_t i = (idx_t)cache_len; i <= n; ++i) {
            seq_t prev = cache[i - 1];
            if (prev == (seq_t)0) {           /* once 0 is reached it stays 0 */
                cache[i] = (seq_t)0;
                continue;
            }

            seq_t base_k = pow2((idx_t)i);     /* base 2^k for extraction */

            /* Extract digits of prev in base_k. */
            seq_t *digits = NULL;
            size_t dcap   = 0;
            size_t dcnt   = 0;
            seq_t tmp = prev;
            while (tmp > (seq_t)0) {
                if (dcnt == dcap) {           /* grow array */
                    size_t newdcap = dcap == 0 ? 8 : dcap * 2;
                    seq_t *newt = (seq_t *)malloc(newdcap * sizeof(seq_t));
                    if (dcnt) memcpy(newt, digits, dcnt * sizeof(seq_t));
                    free(digits);
                    digits = newt;
                    dcap   = newdcap;
                }
                digits[dcnt++] = tmp % base_k;
                tmp /= base_k;
            }

            /* Reinterpret the digits in base 2^(k+1). */
            seq_t base_B = pow2((idx_t)i + 1);
            seq_t res = (seq_t)0;
            for (size_t j = dcnt; j > 0; --j) {
                res = res * base_B + digits[j - 1];
            }
            res -= (seq_t)1;
            cache[i] = res;

            free(digits);
        }
        cache_len = (size_t)n + 1;
    }

    return cache[(size_t)n];
}
