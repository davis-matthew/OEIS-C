/*
 * A378287  Numbers not of the form m^k for some k>=3. Complement of A076467.
 * https://oeis.org/A378287
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 67, u16 n <= 67, u32 n <= 67, u64 n <= 67, u128 n <= 67,
 *            big any n
 * Helpers:   pow_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Numbers not of the form m^k for k >= 3 (OEIS A378287) */
/* This implementation fills a static cache of these numbers by testing
   each integer for being a perfect k‑th power with k ≥ 3 using only
   integer arithmetic.  It works for any integer seq_t type; 
   idx_t is uint64_t. */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/pow_seq.h"

/* static cache of already generated terms */
static seq_t *cache = NULL;          /* array of generated terms      */
static size_t cache_cap = 0;         /* allocated capacity of cache   */
static size_t cache_len = 0;         /* number of terms currently cached */
static idx_t  next_candidate = 2;    /* next integer to be tested     */

/* test whether x is a perfect power with exponent >= 3 */
static bool is_perfect_power(seq_t x)
{
    if (x < (seq_t)8)                    /* 2^3 = 8 is the smallest such power */
        return false;

    /* For each exponent e >= 3 such that 2^e <= x */
    for (idx_t exp = 3; ; ++exp) {
        if ((1ULL << exp) > (uint64_t)x)
            break;     /* no larger exponents can satisfy 2^e <= x */

        /* For each base b >= 2 compute b^exp and compare with x */
        seq_t base = 2;
        for (;;) {
            /* compute base^exp with overflow detection */
            seq_t p = 1;
            bool overflow = false;
            for (idx_t i = 0; i < exp; ++i) {
                if (p > x / base) {  /* further multiplication would overflow or exceed x */
                    overflow = true;
                    break;
                }
                p *= base;
            }
            if (!overflow && p == x)
                return true;          /* found a perfect power */

            if (overflow || p > x)
                break;                /* larger bases will only increase p */

            ++base;
        }
    }
    return false;
}

/* ensure capacity of cache */
static void grow_cache(void)
{
    size_t new_cap = (cache_cap == 0) ? 16 : cache_cap * 2;
    seq_t *tmp = (seq_t*)calloc(new_cap, sizeof(seq_t));
    if (!tmp)
        return;                      /* allocation failed – keep old cache */

    if (cache) {
        for (size_t i = 0; i < cache_len; ++i)
            tmp[i] = cache[i];
        free(cache);
    }
    cache = tmp;
    cache_cap = new_cap;
}

/* public function requested by the build – returns a(n) for n >= 1    */
seq_t a(idx_t n)
{
    if (n < 1)
        return 0;                    /* undefined for n < 1 */

    while (cache_len < (size_t)n) {
        if (!is_perfect_power((seq_t)next_candidate)) {
            if (cache_len + 1 > cache_cap)
                grow_cache();
            cache[cache_len++] = (seq_t)next_candidate;
        }
        ++next_candidate;
    }
    return cache[n - 1];
}
