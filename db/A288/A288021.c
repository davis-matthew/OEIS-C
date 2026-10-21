/*
 * A288021  Prime p1 of consecutive primes p1, p2, where p2 - p1 = 4, and p1,
 *          p2 are in different decades.
 * https://oeis.org/A288021
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the nth prime p such that p+4 is prime, p+2 composite,
 *            and (p/10)!=(p+4)/10, i.e. consecutive primes with difference 4
 *            spanning a decade
 * Range:     u8 n <= 9, u16 n <= 433, u32 n <= 483, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#include "../utils/is_prime_big.h"

static seq_t *cache = NULL;
static size_t cache_len = 0;
static size_t cache_cap = 0;
static seq_t next_candidate = 7;

static void grow_cache(size_t needed)
{
    if (cache_cap >= needed) return;
    size_t new_cap = cache_cap ? cache_cap * 2 : 128;
    while (new_cap < needed) new_cap <<= 1;
    seq_t *new_ptr = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!new_ptr) abort();
    cache = new_ptr;
    cache_cap = new_cap;
}

static void extend_cache_to(idx_t n)
{
    while (cache_len < n)
    {
        seq_t candidate = next_candidate;
        if (is_prime_big(candidate))
        {
            seq_t next = candidate + 4;
            if (is_prime_big(next) &&
                !is_prime_big(candidate + 2) &&
                (candidate / 10) != (next / 10))
            {
                grow_cache(cache_len + 1);
                cache[cache_len++] = candidate;
            }
        }
        next_candidate = candidate + 2;
    }
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;  /* offset is 1, undefined for n<1 */
    extend_cache_to(n);
    return cache[n - 1];
}
