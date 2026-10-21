/*
 * A245745  Primes p such that p - d is also prime, where d is the smallest
 *          nonzero digit of p.
 * https://oeis.org/A245745
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 198, u32 n <= 45, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   grow_seq, is_prime, is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This function returns the n‑th term of OEIS A245745:  
   primes p such that the smallest non‑zero digit d of p
   satisfies p - d is also prime.  The function caches the
   sequence terms and is agnostic to the order of calls.
*/

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

/* Utilities from the shared library */
#include "../utils/is_prime.h"
#include "../utils/is_prime_big.h"
#include "../utils/grow_seq.h"

/* Static array caching the sequence terms.
   Terms are stored in 0‑based order: cache[0] = a(1).   */
static seq_t *cache = NULL;      /* array of found terms */
static size_t cache_size = 0;    /* number of terms stored */
static size_t cache_cap  = 0;    /* allocated capacity of cache array */
static idx_t last_checked = 1;   /* largest integer that was tested already */

/* Extend the cache until it holds at least n terms. */
static void generate_until(idx_t n)
{
    /* cache already large enough? */
    if (cache_size >= n)
        return;

    /* start checking from the first integer that has not yet been tested */
    idx_t cand = last_checked + 1;

    while (1) {
        /* skip non‑primes */
        if (!is_prime(cand)) {
            cand++;
            continue;
        }

        /* find the smallest non‑zero digit of candidate */
        seq_t d = 10;               /* more than any decimal digit */
        idx_t t = cand;
        while (t > 0) {
            seq_t dig = t % 10;
            if (dig != 0 && dig < d)
                d = dig;
            t /= 10;
        }

        /* p - d must also be prime */
        seq_t diff = (seq_t)cand - d;
        if (diff >= 2 && is_prime_big(diff)) {
            /* add to cache */
            if (cache_size == cache_cap)
                grow_seq(&cache, &cache_cap, cache_size + 1);
            cache[cache_size++] = (seq_t)cand;

            /* we now have enough terms? */
            if (cache_size >= n) {
                last_checked = cand;
                break;
            }
        }
        cand++;
    }
}

/* Public function expected by the harness. */
seq_t a(idx_t n)
{
    if (n == 0)          /* offset 1: a(1) is the first item */
        return 0;

    generate_until(n);
    return cache[n - 1];
}
