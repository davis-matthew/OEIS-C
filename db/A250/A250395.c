/*
 * A250395  Numbers k such that 11410337850553 + 4609098694200*k is prime.
 * https://oeis.org/A250395
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = the n‑th non‑negative integer k such that 11410337850553
 *            + 4609098694200*k is prime. Computation method: sequential
 *            search with memoisation
 * Range:     u8 none, u16 none, u32 none, u64 n <= 10000, u128 n <= 10000,
 *            big any n
 * Helpers:   isPrimeGeneric
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include "../utils/isPrimeGeneric.h"

static seq_t *cache = NULL;          /* array of found k values          */
static uint64_t cache_len = 0;       /* number of cached values          */
static uint64_t cache_cap = 0;       /* current allocation capacity      */
static uint64_t next_k = 0;          /* next k to test                    */

static const seq_t BASE = (seq_t)11410337850553ULL;
static const seq_t STEP = (seq_t)4609098694200ULL;

/* Double cache capacity when needed. */
static void ensure_capacity(void)
{
    if (cache_len >= cache_cap) {
        uint64_t new_cap = (cache_cap == 0) ? 16 : cache_cap << 1;
        seq_t *new_ptr = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!new_ptr) abort();          /* out of memory */
        cache = new_ptr;
        cache_cap = new_cap;
    }
}

/* Find and store the next prime‑producing k. */
static void generate_next_term(void)
{
    for (;;) {
        seq_t test = BASE + STEP * (seq_t)next_k;
        if (isPrimeGeneric(test)) {
            ensure_capacity();
            cache[cache_len++] = next_k;
            ++next_k;
            break;                      /* prime found, exit loop */
        }
        ++next_k;
    }
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;        /* undefined, but return 0 */

    while (cache_len < n) {
        generate_next_term();
    }
    return cache[n - 1];
}
