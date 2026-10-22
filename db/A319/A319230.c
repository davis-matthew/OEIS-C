/*
 * A319230  Triple prime Fibonacci numbers: Fibonacci numbers f such that (1)
 *          f is prime, (2) f's index is prime, and (3) the sum of f's digits
 *          is prime.
 * https://oeis.org/A319230
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) is the nth Fibonacci number f such that f is prime, the
 *            index of f is prime, and the sum of the decimal digits of f is
 *            prime. The function *a* searches successive Fibonacci indices
 *            and caches each qualifying Fibonacci number
 * Range:     u8 n <= 3, u16 n <= 3, u32 n <= 3, u64 n <= 6, u128 n <= 6,
 *            big any n
 * Helpers:   digit_sum, fib, is_prime, is_prime_big, is_prime_small
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/fib.h"            /* seq_t fib(idx_t n); */
#include "../utils/is_prime.h"       /* bool is_prime(idx_t n); */
#include "../utils/is_prime_big.h"   /* bool is_prime_big(seq_t n); */
#include "../utils/is_prime_small.h" /* bool is_prime_small(uint64_t n); */
#include "../utils/digit_sum.h"     /* seq_t digit_sum(seq_t x); */

/* Cache of found numbers */
static seq_t *cache = NULL;
static size_t cache_cnt = 0;
static size_t cache_cap = 0;
static idx_t   next_index = 3; /* next Fibonacci index to examine */

/* Grow the cache to at least *needed* entries */
static void grow_cache(size_t needed)
{
    if (cache_cnt >= needed) return;
    if (cache_cap == 0) cache_cap = 4;
    while (cache_cap < needed) cache_cap <<= 1;
    seq_t *tmp = (seq_t *)realloc(cache, cache_cap * sizeof(seq_t));
    if (!tmp) abort();           /* unsafe but acceptable in this context */
    cache = tmp;
}

/* Main sequence function */
seq_t a(idx_t n)
{
    if (n < 1) return 0; /* Undefined for n==0, return 0 as placeholder */

    /* Ensure cache can hold requested term */
    grow_cache(n);

    /* Expand cache until we have at least n elements */
    while (cache_cnt < (size_t)n)
    {
        /* Check prime index */
        if (is_prime(next_index))
        {
            seq_t f = fib(next_index);

            /* Quick check: small numbers may fit in uint64_t but fib may be large */
            if (is_prime_big(f))
            {
                seq_t sum_seq = digit_sum(f);
                /* sum of digits is guaranteed to fit in 64 bits */
                uint64_t sum = (uint64_t)sum_seq;
                if (is_prime_small(sum))
                {
                    /* Store qualified Fibonacci number */
                    cache[cache_cnt++] = f;
                }
            }
        }
        ++next_index;
    }

    return cache[n - 1];
}
