/*
 * A307473  SanD-50 primes: primes p such that p+d is also prime and sum of
 *          digits A007953(p(p+d)) = d, with d = 50.
 * https://oeis.org/A307473
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 none, u16 none, u32 n <= 602, u64 n <= 10000, u128 n <= 10000,
 *            big any n
 * Helpers:   digit_sum, is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * SanD-50 primes: primes p such that p+50 is also prime and digit_sum(p*(p+50)) = 50.
 * The function a(idx_t n) returns the n-th SanD-50 prime (1‑based).
 * The sequence is computed by a search; a cache of found primes is maintained.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/is_prime.h"
#include "../utils/digit_sum.h"

static size_t cache_len = 0;     /* number of primes stored */
static size_t cache_cap = 0;     /* capacity of cache array */
static seq_t *cache = NULL;      /* dynamic array of primes */
static idx_t next_to_check = 2;  /* next integer candidate to test */

/* ensure that cache contains at least n primes */
static void ensure_cache(size_t n)
{
    if (cache_len >= n) return;

    idx_t p = next_to_check;
    if (p <= 2) p = 2;
    else if ((p & 1) == 0) p++;          /* skip even numbers except 2 */

    while (cache_len < n) {
        if (is_prime(p)) {
            idx_t pplus = p + 50;
            if (is_prime(pplus) &&
                digit_sum((seq_t)p * (seq_t)pplus) == 50) {

                /* grow cache if necessary */
                if (cache_len == cache_cap) {
                    size_t new_cap = cache_cap ? cache_cap * 2 : 64;
                    seq_t *tmp = (seq_t *)realloc(cache,
                                                  new_cap * sizeof(seq_t));
                    if (!tmp) abort();
                    cache = tmp;
                    cache_cap = new_cap;
                }
                cache[cache_len++] = (seq_t)p;
            }
            /* move to next odd candidate after 2, or next odd after p otherwise */
            if (p == 2) p = 3;
            else p += 2;
        } else {
            /* skip to the next odd candidate */
            if (p == 2) p = 3;
            else p += 2;
        }
    }

    next_to_check = p;
}

/* return the n-th SanD-50 prime (a(1) is the first prime listed in OEIS A307473) */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;           /* offset is 1 */
    ensure_cache((size_t)n);
    return cache[(size_t)(n - 1)];
}
