/*
 * A345663  Numbers k such that A001414(k) is divisible by A007953(k).
 * https://oeis.org/A345663
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = the nth integer k ≥ 1 such that the sum of k’s prime *
 *            factors (with multiplicity) is divisible by the sum of its
 *            decimal * digits
 * Range:     u8 n <= 36, u16 n <= 3241, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stddef.h>

/* Cache of previously computed terms. */
static seq_t *cache = NULL;
static size_t cache_len = 0;
static size_t cache_cap = 0;
static uint64_t next_start = 1;        /* next integer to test */

/* Sum of prime factors of n (with multiplicity).  Uses trial division
 * and a 64‑bit accumulator; the result fits in seq_t. */
static seq_t sum_prime_factors(uint64_t n)
{
    seq_t sum = 0;
    if (n <= 1)
        return 0;

    uint64_t m = n;

    /* factor 2 */
    while ((m & 1u) == 0)
    {
        sum += (seq_t)2;
        m >>= 1;
    }

    /* odd factors */
    for (uint64_t p = 3; p <= m / p; p += 2)
    {
        while (m % p == 0)
        {
            sum += (seq_t)p;
            m /= p;
        }
    }

    if (m > 1)
        sum += (seq_t)m;

    return sum;
}

/* Sum of decimal digits of n. */
static seq_t digit_sum(uint64_t n)
{
    seq_t sum = 0;
    while (n > 0)
    {
        sum += (seq_t)(n % 10);
        n /= 10;
    }
    return sum;
}

/* Grow the cache until it contains at least n terms. */
static void ensure_cache(idx_t n)
{
    while (cache_len < n)
    {
        uint64_t k = next_start++;
        seq_t ds = digit_sum(k);
        if (ds == 0)                /* never happens for k>=1, safety */
            continue;
        seq_t sf = sum_prime_factors(k);

        if (sf % ds == 0)           /* k satisfies the condition */
        {
            if (cache_len == cache_cap)
            {
                cache_cap = cache_cap ? (cache_cap << 1) : 4;
                seq_t *tmp = (seq_t *)realloc(cache,
                                               cache_cap * sizeof(seq_t));
                if (!tmp)          /* allocation failure: abort growth */
                    return;
                cache = tmp;
            }
            cache[cache_len++] = (seq_t)k;
        }
    }
}

/* External linkage function required by the build. */
seq_t a(idx_t n)
{
    if (n == (idx_t)0)
        return (seq_t)0;  /* undefined for indices < 1, safety return */
    ensure_cache(n);
    return cache[n - 1];
}
