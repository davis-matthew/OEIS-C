/*
 * A342564  Numbers k such that 6*k + 1 is a prime that can be written as p*q
 *          + 2, with p and q being consecutive primes.
 * https://oeis.org/A342564
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = k, where k = (p*q+2-1)/6, p and q are consecutive primes,
 *            p*q+2 is prime, and p*q+2 ≡ 1 (mod 6)
 * Range:     u8 n <= 4, u16 n <= 21, u32 n <= 38, u64 n <= 38, u128 n <= 38,
 *            big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The implementation keeps a static cache of the values found so far
   and generates consecutive primes incrementally.  For each new prime
   pair it checks whether p*q+2 is a prime of the form 6k+1; if so,
   k is stored.  The function works for any seq_t (uint8_t..128‑bit,
   arbitrary precision) because all arithmetic needed for the primes
   itself is performed in the native 64‑bit type. */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/is_prime.h"   /* provides is_prime(idx_t) */

static seq_t *k_cache = NULL;   /* cache of found k values           */
static size_t k_cnt   = 0;       /* number of values stored so far   */
static size_t k_cap   = 0;       /* current capacity of k_cache      */

static bool init_done  = false;  /* initialization flag              */
static uint64_t last_prime = 2;  /* previous prime in the pair       */
static uint64_t candidate = 3;   /* next odd candidate to test       */

/* Expand the cache so that it contains at least n values. */
static void
ensure_cache(idx_t n)
{
    if (!init_done) {
        init_done = true;
        k_cap   = 64;
        k_cache = (seq_t *)malloc(k_cap * sizeof(seq_t));
    }

    while (k_cnt < (size_t)n) {
        /* Find next prime number */
        while (!is_prime(candidate))
            candidate += 2;            /* only odd numbers after 2      */

        uint64_t new_prime = candidate;
        candidate += 2;                /* advance to the next candidate */

        /* Compute p*q + 2 for the consecutive primes p and q */
        uint64_t prod = last_prime * new_prime + 2ULL;

        /* We need prod to be a prime of the form 6k+1. */
        if ((prod % 6ULL == 1ULL) && is_prime(prod)) {
            seq_t k = (seq_t)((prod - 1ULL) / 6ULL);

            if (k_cnt == k_cap) {      /* grow cache if necessary */
                k_cap *= 2;
                k_cache = (seq_t *)realloc(k_cache, k_cap * sizeof(seq_t));
            }
            k_cache[k_cnt++] = k;
        }

        last_prime = new_prime;        /* update for next pair          */
    }
}

/* Public sequence function.  For n ≥ 1 returns a(n). */
seq_t
a(idx_t n)
{
    if (n == 0)          /* undefined for n < 1; return 0 safely   */
        return 0;

    ensure_cache(n);
    return k_cache[n - 1];    /* offset is 1, so a(1) is first term */
}
