/*
 * A398168  Cubefree numbers k such that k+1 is squarefree.
 * https://oeis.org/A398168
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) is the n‑th cube‑free integer k such that k+1 is
 *            square‑free
 * Range:     u8 n <= 127, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   build_primes_upto, int_cube_root, is_squarefree
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  The implementation is a simple brute‑force search, memoising the
    results in a static cache.  The search tests each integer k for
    the two conditions:
      * k has no prime factor of exponent ≥ 3  →  cube‑free
      * k + 1 has no repeated prime factor →  square‑free
    The tests use the utility functions provided by the build:
      * build_primes_upto   – for the list of primes up to a bound
      * int_cube_root       – to decide how far we need primes
      * is_squarefree       – to test square‑freeness
*/

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/build_primes_upto.h"
#include "../utils/int_cube_root.h"
#include "../utils/is_squarefree.h"

/* ------------------------------------------------------------------ */
/*  Helper: keep a list of primes up to the required bound            */
/* ------------------------------------------------------------------ */
static uint64_t *primes = NULL;       /* array of primes */
static size_t  prime_cnt  = 0;        /* number of primes */
static idx_t   prime_limit = 0;       /* current maximum limit of stored primes */

/* Ensure primes up to int_cube_root(max_val)+1 are available */
static void
ensure_primes(idx_t max_val)
{
    idx_t lim = int_cube_root(max_val) + 1ULL;
    if (lim <= prime_limit) {
        return;
    }
    /* free previous list if any */
    if (primes != NULL) {
        free(primes);
    }
    /* build new list */
    build_primes_upto(lim, &primes, &prime_cnt);
    prime_limit = lim;
}

/* Test whether n is cube‑free (no prime factor of exponent ≥ 3) */
static bool
is_cube_free(idx_t n)
{
    if (n < 8ULL) {          /* 1 – 7 are trivially cube‑free */
        return true;
    }
    ensure_primes(n);
    for (size_t i = 0; i < prime_cnt; ++i) {
        uint64_t p = primes[i];
        /* compute p^3 with 128‑bit to avoid overflow */
        __int128 p3x = (__int128)p * p * p;
        if (p3x > n) {
            break;            /* further primes have larger cubes */
        }
        idx_t p3 = (idx_t)p3x;
        if (n % p3 == 0) {
            return false;     /* divisible by a cube */
        }
    }
    return true;
}

/* ------------------------------------------------------------------ */
/*  Sequence cache                                                  */
/* ------------------------------------------------------------------ */
static seq_t *cache   = NULL;  /* array of found terms */
static size_t cache_len = 0;   /* number of terms stored so far */
static size_t cache_cap = 0;   /* allocated capacity for cache */
static idx_t  next_candidate = 1;  /* next integer to test */

/* Return the n‑th element of the sequence (1‑based indexing) */
seq_t
a(idx_t n)
{
    if (n == 0) {
        return 0;             /* undefined but guard against misuse */
    }
    while (cache_len < n) {
        seq_t cand = (seq_t)next_candidate;

        /* test cube‑free and square‑free conditions */
        if (is_cube_free(next_candidate) &&
            is_squarefree(cand + (seq_t)1)) {

            /* grow cache if needed */
            if (cache_len == cache_cap) {
                cache_cap = cache_cap ? cache_cap * 2 : 16;
                cache = (seq_t *)realloc(cache, cache_cap * sizeof(seq_t));
            }
            cache[cache_len++] = cand;
        }
        ++next_candidate;
    }
    return cache[n - 1];
}
