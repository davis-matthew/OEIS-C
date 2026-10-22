/*
 * A381315  Numbers whose prime factorization exponents include exactly one 3
 *          and no exponent greater than 3.
 * https://oeis.org/A381315
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   This is not an explicit formula, but a generation by search.
 *            Sequence: Numbers whose prime factorization exponents include
 *            exactly one 3 and no exponent greater than 3. Offset: 1 (a(1) =
 *            8)
 * Range:     u8 n <= 22, u16 n <= 5872, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   my_alloc, my_free
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/my_alloc.h"
#include "../utils/my_free.h"

/* Compatibility helpers for C / C++ */
#ifdef __cplusplus

#else

#endif

/* Global cache for the sequence values.
   cache_len  – number of terms stored
   cache_cap  – allocated capacity
   next_val   – smallest integer that has not yet been tested
*/
static seq_t  *cache   = NULL;
static size_t  cache_len = 0;
static size_t  cache_cap = 0;
static uint64_t next_val = 8;   /* first candidate (a(1) = 8) */

/* Ensure that the cache can store at least `need` elements. */
static void ensure_cache(size_t need)
{
    if (need <= cache_cap) return;

    size_t new_cap = cache_cap ? cache_cap * 2 : 16;
    while (new_cap < need) new_cap *= 2;

    seq_t *tmp = (seq_t*)my_alloc(new_cap * sizeof(seq_t));
    if (!tmp) { my_free(cache); exit(1); }   /* fatal if memory allocation fails */
    /* copy existing elements */
    for (size_t i = 0; i < cache_len; ++i) tmp[i] = cache[i];

    /* free old array */
    my_free(cache);
    cache = tmp;
    cache_cap = new_cap;
}

/* Test whether a single integer satisfies the definition. */
static bool good_number(uint64_t x)
{
    uint64_t tmp = x;
    int exact_threes = 0;

    for (uint64_t p = 2; p <= tmp / p; ++p) {
        if (tmp % p == 0) {
            int exp = 0;
            while (tmp % p == 0) {
                tmp /= p;
                ++exp;
                if (exp > 3) return false;      /* exponent too large */
            }
            if (exp == 3) ++exact_threes;
        }
    }

    /* Remaining factor > 1 is a prime with exponent 1 */
    if (tmp > 1) {
        /* exp is 1, automatically <=3, so nothing to check here */
    }

    return exact_threes == 1;
}

/* Sequence function. */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;   /* undefined for the sequence offset 1 */

    while (cache_len < (size_t)n) {
        if (good_number(next_val)) {
            ensure_cache(cache_len + 1);
            cache[cache_len++] = (seq_t)next_val;
        }
        ++next_val;
    }

    return cache[(size_t)n - 1];
}
