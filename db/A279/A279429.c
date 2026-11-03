/*
 * A279429  Numbers k such that k^2 has an odd number of digits and the middle
 *          digit is 9.
 * https://oeis.org/A279429
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 17, u16 n <= 2309, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 63 terms through n = 8754 (2026-11-03)
 * Helpers:   mem, num_digits_u64_to_u32
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A279429: numbers k such that k^2 has an odd number of digits
   and the middle digit of k^2 is 9.  The function generates the terms
   by brute‑force search, caching results for subsequent calls. */

#include <stdlib.h>
#include "../utils/num_digits_u64_to_u32.h"
#include "../utils/mem.h"

static unsigned long long pow10_ull(unsigned int e)
{
    unsigned long long p = 1;
    while (e--) p *= 10;
    return p;
}

/*---  static helper tables  --------------------------------------------*/

static seq_t *cache = NULL;   /* array of cached terms                */
static size_t cache_len = 0;  /* how many terms are cached             */
static size_t cache_cap = 0;  /* allocated capacity of cache array     */
static idx_t  last_k   = 0;   /* largest k examined so far            */

/*---  dynamic array management ----------------------------------------*/

static void ensure_capacity(size_t need)
{
    if (need <= cache_cap) return;
    size_t new_cap = cache_cap ? cache_cap * 2 : 32;
    while (new_cap < need) new_cap <<= 1;
    seq_t *new_cache = mem_resize(seq_t, cache, new_cap);
    if (!new_cache) exit(EXIT_FAILURE);   /* unrecoverable failure */
    cache = new_cache;
    cache_cap = new_cap;
}

/*---  helpers working on unsigned long long ---------------------------*/

/*---  sequence generator ---------------------------------------------*/

seq_t a(idx_t n)
{
    if (n == 0) return 0;                     /* safety for n < 1 */

    /* return cached value if already available */
    if ((size_t)n <= cache_len) return cache[n-1];

    /* search until we have at least n terms */
    for (idx_t k = last_k + 1; cache_len < (size_t)n; ++k)
    {
        unsigned long long sq = (unsigned long long)k * (unsigned long long)k;
        unsigned int digits = num_digits_u64_to_u32(sq);

        if (digits & 1U)                       /* odd number of digits? */
        {
            unsigned int m = (digits - 1U) >> 1; /* position of middle digit */
            unsigned long long base = pow10_ull(m);
            unsigned long long middle = (sq / base) % 10ULL;

            if (middle == 9ULL)
            {
                ensure_capacity(cache_len + 1);
                cache[cache_len++] = (seq_t)k;
            }
        }
        last_k = k;
    }

    return cache[n-1];
}
