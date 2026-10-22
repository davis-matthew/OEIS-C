/*
 * A324899  Odd numbers k for which sigma(k) == 3 (mod 4).
 * https://oeis.org/A324899
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 54, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   sigma_square_mod4
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  a(n) = A324909(n)^2  where A324909 lists odd s with sigma(s^2) ≡ 3 (mod 4).  */
/*  Computes the sequence by iterating over odd s, factoring s, and checking sigma(s^2) mod 4. */
#include <stdint.h>
#include <stdlib.h>
#include "../utils/sigma_square_mod4.h"

/*  a(n) function returning the n-th square for which sigma(k) ≡ 3 (mod 4).  */
seq_t a(idx_t n)
{
    static seq_t *cache   = NULL;   /* cached results */
    static size_t cache_sz = 0;     /* current cache size   */
    static size_t cache_cap = 0;    /* allocated capacity   */
    static unsigned long long last_s = 1;   /* next odd integer to test */

    /* Lazy initialization of cache */
    if (cache_cap == 0)
    {
        cache_cap = 256;
        cache = (seq_t *)malloc(sizeof(seq_t) * cache_cap);
    }

    while (cache_sz < n)
    {
        /* increase cache capacity if necessary */
        if (cache_sz >= cache_cap)
        {
            cache_cap *= 2;
            cache = (seq_t *)realloc(cache, sizeof(seq_t) * cache_cap);
        }

        /* Test current odd s */
        if (sigma_square_mod4(last_s) == 3)
        {
            /* store its square */
            cache[cache_sz++] = (seq_t)((last_s * last_s));
        }
        last_s += 2;   /* move to next odd integer */
    }

    return cache[n - 1];
}
