/*
 * A324899  Odd numbers k for which sigma(k) == 3 (mod 4).
 * https://oeis.org/A324899
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 54, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 67 terms through n = 9880 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  a(n) = A324909(n)^2  where A324909 lists odd s with sigma(s^2) ≡ 3 (mod 4).  */
/*  Computes the sequence by iterating over odd s, factoring s, and checking sigma(s^2) mod 4. */
#include <stdint.h>
#include <stdlib.h>
#include "../utils/mem.h"

/* Helper: compute sigma(s^2) modulo 4.  s is odd. */
static unsigned char sigma_square_mod4(unsigned long long s)
{
    unsigned char result = 1;                     /* multiplicative identity modulo 4 */

    unsigned long long temp = s;
    for (unsigned long long p = 3; p * p <= temp; p += 2)
    {
        if (temp % p == 0)
        {
            int e = 0;
            while (temp % p == 0)
            {
                temp /= p;
                ++e;
            }

            /* compute sigma(p^(2e)) mod 4 */
            unsigned char sum = 1;      /* term for exponent 0 */
            unsigned char pow = 1;      /* p^j mod 4 */
            unsigned char p_mod = p & 3;/* p mod 4  (since p is odd) */
            for (int j = 1; j <= 2 * e; ++j)
            {
                pow = (pow * p_mod) & 3;
                sum = (sum + pow) & 3;
            }
            result = (result * sum) & 3;
        }
    }

    if (temp > 1)   /* remaining prime factor > sqrt(s) */
    {
        /* exponent e = 1  (the remaining prime appears once in s) */
        unsigned char sum = 1;
        unsigned char pow = 1;
        unsigned char p_mod = temp & 3;
        for (int j = 1; j <= 2; ++j)      /* 2*e = 2 */
        {
            pow = (pow * p_mod) & 3;
            sum = (sum + pow) & 3;
        }
        result = (result * sum) & 3;
    }

    return result;
}

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
        cache = mem_new(seq_t, cache_cap);
    }

    while (cache_sz < n)
    {
        /* increase cache capacity if necessary */
        if (cache_sz >= cache_cap)
        {
            cache_cap *= 2;
            cache = mem_resize(seq_t, cache, cache_cap);
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
