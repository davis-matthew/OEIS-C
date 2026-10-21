/*
 * A260348  Numbers n such that n is divisible by (10^k - digitsum(n)), where
 *          k equals the number of digits of digitsum(n).
 * https://oeis.org/A260348
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, less
 * Formula:   not stated
 * Range:     u8 n <= 50, u16 n <= 1649, u32 n <= 9827, u64 n <= 9827,
 *            u128 n <= 9827, big any n
 * Helpers:   digit_len, digit_sum, pow10
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: A260348
   a(n) = the n-th positive integer such that
         n % (10^k − digit_sum(n)) == 0,
   where k is the number of digits of digit_sum(n).               */
#include <stddef.h>
#include <stdlib.h>
#include "../utils/digit_sum.h"
#include "../utils/digit_len.h"
#include "../utils/pow10.h"

/* Cache of already found terms. */
static seq_t *cache      = NULL;
static size_t cache_cap  = 0;
static size_t cache_len  = 0;

/* Next integer to test. */
static seq_t next_candidate = 1;

/* Expand the cache capacity. */
static void grow_cache(void)
{
    size_t new_cap = cache_cap ? cache_cap * 2 : 8;
    seq_t *p = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!p) exit(1);
    cache = p;
    cache_cap = new_cap;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;                /* undefined, but harmless */

    /* Generate terms until we have at least n of them. */
    while (cache_len < n) {
        for (;;) {
            seq_t ds = digit_sum(next_candidate);
            int k   = digit_len(ds);
            seq_t pw = pow10(k);
            seq_t M  = pw - ds;        /* positive for all candidates */
            if (M != 0 && next_candidate % M == 0) {
                if (cache_len == cache_cap) grow_cache();
                cache[cache_len++] = next_candidate;
                ++next_candidate;
                break;                  /* found one, proceed to next */
            }
            ++next_candidate;
        }
    }
    return cache[n-1];                   /* zero‑based indexing in cache */
}
