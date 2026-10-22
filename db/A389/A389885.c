/*
 * A389885  Decimal expansion of 1/7142857.
 * https://oeis.org/A389885
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cons, easy
 * Formula:   a(n) is the nth decimal digit of 1/7142857. (Decimal expansion)
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

/* Cache of computed digits.  Each element is a single byte because
   digits are 0–9.  The cache is grown lazily and reused by
   subsequent calls, so the function works for arbitrary call order. */
static uint8_t *digit_cache = NULL;   /* digits 0 … cache_len-1 stored */
static idx_t cache_len  = 0;          /* number of cached digits  */
static idx_t cache_cap  = 0;          /* allocated capacity          */
static uint64_t cache_rem = 1ULL;     /* remainder after last digit  */

/* Ensure the cache contains digits 0 … n. */
static void ensure_cache(idx_t n)
{
    if (n < cache_len)
        return;                       /* already available */

    if (cache_cap == 0) {              /* first allocation */
        cache_cap = 16;
        digit_cache = (uint8_t *)malloc(cache_cap * sizeof(uint8_t));
        if (!digit_cache)
            exit(EXIT_FAILURE);
    }

    for (idx_t i = cache_len; i <= n; ++i) {
        /* One long‑division step: multiply remainder by 10. */
        cache_rem *= 10ULL;

        /* The next decimal digit. */
        uint8_t d = (uint8_t)(cache_rem / 7142857ULL);

        /* Update remainder for the next step. */
        cache_rem %= 7142857ULL;

        /* Grow the buffer if required. */
        if (cache_len == cache_cap) {
            idx_t new_cap = cache_cap * 2;
            if (new_cap <= i + 1)
                new_cap = i + 1;
            uint8_t *tmp = (uint8_t *)realloc(digit_cache, new_cap * sizeof(uint8_t));
            if (!tmp)
                exit(EXIT_FAILURE);
            digit_cache = tmp;
            cache_cap = new_cap;
        }

        digit_cache[cache_len++] = d;
    }
}

/* Return the n‑th decimal digit of 1/7142857. */
seq_t a(idx_t n)
{
    ensure_cache(n);
    return (seq_t)digit_cache[n];
}
