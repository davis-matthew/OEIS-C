/*
 * A272527  Numbers k such that prime(k) - 2 is the average of four
 *          consecutive odd squares.
 * https://oeis.org/A272527
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   ensure_capacity, floor_sqrt, is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * A272527: a(n) is the index k for which prime(k) = 4*m^2+8*m+11 (m≥1).  
 * The function generates primes incrementally and stores qualifying
 * indices in a static cache.  It returns the nth element of that cache.
 */
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/is_prime.h"
#include "../utils/floor_sqrt.h"
#include "../utils/ensure_capacity.h"

/* static cache of qualifying prime indices */
static seq_t *cache      = NULL;
static size_t cache_size = 0;
static size_t cache_cap  = 0;

/* state for prime generation */
static idx_t cur_candidate = 2;   /* integer value to test for primality */
static idx_t prime_index   = 0;   /* count of primes found so far */

/* generate primes until cache holds at least 'need' elements */
static void a_generate_to(idx_t need)
{
    while (cache_size < need) {
        /* find next prime */
        while (!is_prime(cur_candidate)) {
            cur_candidate++;
            if (cur_candidate > 2 && (cur_candidate & 1) == 0)
                cur_candidate++;
        }

        /* prime found at cur_candidate */
        prime_index++;

        seq_t p = (seq_t)cur_candidate;

        /* only primes ≥ 23 can satisfy p = 4m^2+8m+11 (m≥1) */
        if (p >= 23) {
            /* (p-11) must be divisible by 4 */
            if (((p - 11) & 3) == 0) {
                seq_t s = (p - 11) / 4;   /* s = m^2 + 2m          */
                seq_t t = s + 1;          /* t = (m+1)^2           */
                seq_t r = floor_sqrt(t);
                if (r * r == t) {         /* t is a perfect square */
                    ensure_capacity(cache_size + 1, &cache_cap,
                                    (void **)&cache, sizeof(seq_t));
                    cache[cache_size++] = prime_index;
                }
            }
        }

        /* advance to the next candidate */
        cur_candidate++;
        if (cur_candidate > 2 && (cur_candidate & 1) == 0)
            cur_candidate++;
    }
}

seq_t a(idx_t n)
{
    if (n < 1) return 0;
    if (cache_size < n) a_generate_to(n);
    return cache[n - 1];
}
