/*
 * A254335  Powers of 5 in base 60, concatenating the decimal values of the
 *          sexagesimal digits.
 * https://oeis.org/A254335
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = decimal concatenation of the base‑60 digits of 5^n,
 *            treating each base‑60 digit as two decimal digits and stripping
 *            leading zeros from the resulting string
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 12, u64 n <= 24, u128 n <= 49,
 *            big any n
 * Helpers:   seq_alloc, seq_realloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This implementation builds the sequence incrementally, caching terms
   so that repeated calls are O(1) after the initial build.  The array
   holding the cached values is grown by doubling its capacity using the
   helper seq_realloc, which is available when seq_t is an arbitrary‑
   precision type compiled as C++17.  For plain integer builds the
   helper behaves like realloc. */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <assert.h>

/* Utility for dynamic allocation of seq_t arrays; safe for C and C++. */
#if defined(__cplusplus)
/* seq_alloc and seq_realloc are provided by the build system. */
#  include "../utils/seq_alloc.h"
#  include "../utils/seq_realloc.h"
#else
/* For plain C builds, these macros just wrap malloc/realloc. */
#  define seq_alloc(cap)       ((seq_t*)malloc(sizeof(seq_t)*(cap)))
#  define seq_realloc(old, old_cap, new_cap) \
           ((seq_t*)realloc((old), sizeof(seq_t)*(new_cap)))
#endif

/* Function: computes a(n). */
seq_t a(idx_t n)
{
    /* Static cache of computed terms. */
    static seq_t *cache      = NULL;
    static size_t cache_cap  = 0;   /* allocated slots in cache */
    static size_t cache_size = 0;   /* number of terms actually stored */

    /* Static array for the base‑60 digits of the current power of 5. */
    static uint8_t *digits   = NULL;
    static size_t digits_cap = 0;   /* allocated length of digits */
    static size_t digits_len = 0;   /* number of used digits */

    /* Initialisation on first call. */
    if (cache == NULL)
    {
        /* Reserve a small initial capacity. */
        cache_cap  = 16;
        cache      = seq_alloc(cache_cap);
        assert(cache != NULL);

        /* Prepare the digits array for 5^0 = 1. */
        digits_cap = 16;
        digits     = (uint8_t*)malloc(sizeof(uint8_t) * digits_cap);
        assert(digits != NULL);

        digits_len = 1;
        digits[0]  = 1;        /* least‑significant base‑60 digit */

        cache_size = 1;        /* cache[0] = a(0) */
        cache[0]   = 1;        /* a(0) = 1 */
    }

    /* Convert idx_t to size_t for array indexing; the sequence is only
       tested for n up to about 10 000, so overflow is impossible here. */
    size_t target = (size_t)n;
    if (n > SIZE_MAX) abort();

    /* Extend the cache up to the requested index. */
    while (cache_size <= target)
    {
        /* Multiply the current base‑60 number by 5 to obtain 5^(k+1). */
        unsigned int carry = 0;
        for (size_t i = 0; i < digits_len; ++i)
        {
            unsigned int tmp = (unsigned int)digits[i] * 5u + carry;
            digits[i] = (uint8_t)(tmp % 60u);
            carry     = tmp / 60u;
        }
        while (carry)
        {
            if (digits_len == digits_cap)
            {
                digits_cap = digits_cap * 2;
                uint8_t *tmp = (uint8_t*)realloc(digits, digits_cap);
                assert(tmp != NULL);
                digits = tmp;
            }
            digits[digits_len++] = (uint8_t)(carry % 60u);
            carry /= 60u;
        }

        /* Translate the base‑60 digits into the concatenated decimal
           integer value.  Iterate from most‑significant digit to
           least‑significant. */
        seq_t val = 0;
        for (int i = (int)digits_len - 1; i >= 0; --i)
        {
            /* Each base‑60 digit contributes two decimal digits. */
            val = val * 100 + (seq_t)digits[i];
        }

        /* Ensure sufficient capacity in the cache. */
        if (cache_size == cache_cap)
        {
            size_t new_cap = cache_cap * 2;
            cache = seq_realloc(cache, cache_cap, new_cap);
            cache_cap = new_cap;
            assert(cache != NULL);
        }

        cache[cache_size++] = val;
    }

    return cache[n];
}
