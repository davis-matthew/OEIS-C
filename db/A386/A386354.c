/*
 * A386354  Primes without {6, 7} as digits.
 * https://oeis.org/A386354
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 34, u16 n <= 2356, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Search: primes that have no digits 6 or 7 */
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>           /* for uint8_t */
#include "../utils/is_prime.h"   /* is_prime(idx_t) returns bool */

seq_t a(idx_t n)
{
    /* Static cache for previously found values */
    static seq_t *cache  = NULL;   /* array of values */
    static size_t cache_cap = 0;   /* allocated capacity of cache */
    static size_t cache_sz  = 0;   /* number of stored terms */
    static idx_t  candidate = 2;   /* next number to test */

    /* n <= 0 is outside the OEIS offset – return 0 */
    if (n == 0) return 0;

    /* Generate terms until we have at least n of them */
    while (cache_sz < n)
    {
        /* Find next prime that does not contain 6 or 7 */
        while (1)
        {
            if (is_prime(candidate))
            {
                idx_t x = candidate;
                bool ok = true;
                while (x > 0)
                {
                    uint8_t d = (uint8_t)(x % 10);
                    if (d == 6 || d == 7)
                    {
                        ok = false;
                        break;
                    }
                    x /= 10;
                }
                if (ok) break;  /* found next sequence element */
            }
            ++candidate;        /* try next number */
        }

        /* Ensure cache capacity */
        if (cache_sz + 1 > cache_cap)
        {
            /* enlarge to at least twice the current size, or to required size */
            size_t new_cap = cache_cap ? cache_cap * 2 : 16;
            if (new_cap < cache_sz + 1) new_cap = cache_sz + 1;
            /* In C++ we use static_cast, in C a simple cast – conditional on language */
#if defined(__cplusplus)
            seq_t *tmp = static_cast<seq_t*>(realloc(cache, new_cap * sizeof(seq_t)));
#else
            seq_t *tmp = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
#endif
            cache = tmp;
            cache_cap = new_cap;
        }

        /* Store the found prime and advance the candidate */
        cache[cache_sz++] = (seq_t)candidate;
        ++candidate;
    }

    return cache[n - 1];
}
