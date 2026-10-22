/*
 * A341286  Numbers k such that k plus the sum of the fifth powers of the
 *          digits of k is a cube.
 * https://oeis.org/A341286
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn, less
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 1, u32 n <= 36, u64 n <= 36, u128 n <= 36,
 *            big any n
 * Helpers:   floor_cube_root
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A341286
    Numbers k such that k plus the sum of the fifth powers of the digits of k is a cube.
    This implementation computes terms on demand by scanning integers k in ascending order.
*/

#include <stdint.h>
#include <stdlib.h>
#include "../utils/floor_cube_root.h"

/* Pre‑computed fifth powers of the digits 0‑9 */
static const seq_t pow5[10] = {
    0,      /* 0^5 */
    1,      /* 1^5 */
    32,     /* 2^5 */
    243,    /* 3^5 */
    1024,   /* 4^5 */
    3125,   /* 5^5 */
    7776,   /* 6^5 */
    16807,  /* 7^5 */
    32768,  /* 8^5 */
    59049   /* 9^5 */
};

/* Static cache of found terms */
static seq_t *cache = NULL;   /* array storing terms */
static idx_t  cache_len = 0;  /* number of cached terms */
static idx_t  cache_cap = 0;  /* allocated capacity */

/* Next integer k to examine */
static idx_t next_k_start = 0;

/* Helper: ensure capacity for appending terms */
static void ensure_cache_capacity(idx_t need)
{
    if (cache_cap >= need) return;
    /* grow capacity: double or set to need, whichever larger */
    idx_t new_cap = cache_cap ? cache_cap * 2 : 64;
    if (new_cap < need) new_cap = need;
    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!tmp) exit(EXIT_FAILURE);
    cache = tmp;
    cache_cap = new_cap;
}

/* Main function requested by the build */
seq_t a(idx_t n)
{
    if (n == 0) return 0; /* sequence has offset 1, but guard against zero */

    /* Expand cache until it contains at least n terms */
    while (cache_len < n) {
        seq_t k = (seq_t)next_k_start;
        /* compute sum of fifth powers of digits of k */
        seq_t sum = 0;
        seq_t tmp = k;
        while (tmp > 0) {
            unsigned d = (unsigned)(tmp % 10);
            sum += pow5[d];
            tmp /= 10;
        }
        seq_t t = k + sum;
        seq_t r = floor_cube_root(t);
        if (r * r * r == t) {
            ensure_cache_capacity(cache_len + 1);
            cache[cache_len++] = k;
        }
        next_k_start++;
        /* safety for sequences beyond 64‑bit; break if k overflows seq_t */
        if (next_k_start == SIZE_MAX) break;
    }

    return cache[n - 1];
}
