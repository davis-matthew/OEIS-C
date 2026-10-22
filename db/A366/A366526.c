/*
 * A366526  Prime powers (A246655) q such that 2 is a nonzero square in the
 *          finite field F_q.
 * https://oeis.org/A366526
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 30, u16 n <= 3330, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   small_prime_factor
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Generate OEIS sequence A366526: prime powers q such that
 * 2 is a nonzero square in the finite field F_q, i.e. q≡1 or 7 (mod 8).
 * The sequence lists each such prime power in increasing order.
 * Term a(1)=7, a(2)=9, a(3)=17, … as given in the OEIS entry.
 *
 * The function keeps a cache of discovered terms, so repeated calls
 * or out‑of‑order indices are handled efficiently.  The cache is built
 * by scanning successive integers and testing the prime‑power
 * condition with a simple trial division routine.
 *
 * Note: the helper is_prime_power() uses a minimal smallest‑prime‑factor
 * routine suitable for the range needed for n≤10 000 (q is well below
 * 10¹⁰).  It does not use any external utility headers.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/small_prime_factor.h"

/* ---- small_prime_factor ------------------------------------- */

/* ---- is_prime_power ------------------------------------------ */
/* Return true if q ≥ 2 is a prime power (q = p^e, e ≥ 1).  */
static bool is_prime_power(uint64_t q)
{
    if (q < 2) return false;
    uint64_t p = small_prime_factor(q);
    if (p == q) return true;            /* q itself is prime */
    uint64_t t = q;
    while (t % p == 0) t /= p;
    return t == 1;                      /* remainder 1 → prime power */
}

/* ---- cache utilities ----------------------------------------- */
static seq_t *cache   = NULL;     /* array of computed terms */
static size_t cache_len = 0;      /* number of stored terms */
static size_t cache_cap = 0;      /* current capacity */
static uint64_t processed_q = 0;  /* largest q examined so far */

/* Grow the cache capacity if needed */
static void grow_cache(size_t needed)
{
    if (cache_cap >= needed) return;
    size_t new_cap = cache_cap ? cache_cap * 2 : 16;
    while (new_cap < needed) new_cap <<= 1;
    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!tmp) exit(EXIT_FAILURE); /* allocation failure */
    cache = tmp;
    cache_cap = new_cap;
}

/* Populate the cache until it holds at least n terms. */
static void ensure_terms(idx_t n)
{
    while (cache_len < (size_t)n) {
        uint64_t limit = processed_q ? (processed_q << 1) : 32;
        for (uint64_t q = processed_q + 1; q <= limit; ++q) {
            if (is_prime_power(q)) {
                uint64_t m = q & 7ULL;                 /* q mod 8 */
                if ((m == 1) || (m == 7)) {
                    if (q >= 7) {                      /* skip small ones */
                        grow_cache(cache_len + 1);
                        cache[cache_len++] = (seq_t)q;
                    }
                }
            }
        }
        processed_q = limit;
    }
}

/* ---- public sequence function --------------------------------- */
seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* sequence is undefined for index 0 */
    ensure_terms(n);
    return cache[(size_t)(n - 1)];
}
