/*
 * A296107  Twin prime pairs both of which have the same number of prime
 *          digits.
 * https://oeis.org/A296107
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 188, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   ensure_capacity, is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Twin prime pairs where both primes have the same number of prime digits. */
#include <stdbool.h>
#include <stddef.h>
#include "../utils/is_prime.h"
#include "../utils/ensure_capacity.h"

/* static cache for the sequence values */
static seq_t *cache = NULL;
static size_t cache_len = 0;
static size_t cache_cap = 0;
static idx_t next_candidate = 2;

/* count the digits of a number that are prime (2,3,5,7) */
static inline int prime_digit_count(seq_t x)
{
    int cnt = 0;
    while (x > 0)
    {
        seq_t d = x % 10;
        if (d == 2 || d == 3 || d == 5 || d == 7)
            cnt++;
        x = x / 10;
    }
    return cnt;
}

/* extend the cache until it contains at least n terms */
static void extend_cache(idx_t n)
{
    while (cache_len < (size_t)n)
    {
        /* find next prime */
        idx_t p = next_candidate;
        while (!is_prime(p))
            ++p;
        next_candidate = p + 1;

        if (p < 3)
            continue;              /* 2 has no twin partner */

        if (is_prime(p - 2))
        {
            int c1 = prime_digit_count((seq_t)(p - 2));
            int c2 = prime_digit_count((seq_t)p);
            if (c1 == c2)
            {
                size_t required = cache_len + 2;
                if (required > cache_cap)
                {
                    size_t new_cap = cache_cap ? cache_cap * 2 : 16;
                    if (new_cap < required)
                        new_cap = required;
                    ensure_capacity(new_cap, &cache_cap,
                                    (void **)&cache, sizeof(seq_t));
                }
                cache[cache_len++] = (seq_t)(p - 2);
                cache[cache_len++] = (seq_t)p;
            }
        }
    }
}

seq_t a(idx_t n)
{
    if (n == 0)          /* offset 1; n<1 is undefined */
        return 0;
    extend_cache(n);
    return cache[n - 1];
}
