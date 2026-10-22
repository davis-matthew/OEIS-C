/*
 * A356498  Primes p such that 100*p + 11 is also prime.
 * https://oeis.org/A356498
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 19, u32 n <= 25, u64 n <= 57, u128 n <= 57,
 *            big any n
 * Helpers:   is_prime_big, next_prime, seq_alloc, seq_realloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: primes p such that 100*p + 11 is also prime. */
/* The function memoises found primes across calls. */
#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>
#include "../utils/is_prime_big.h"
#include "../utils/next_prime.h"
#include "../utils/seq_alloc.h"
#include "../utils/seq_realloc.h"

static seq_t *cache = NULL;           /* array of qualifying primes */
static size_t cap   = 0;              /* current capacity */
static size_t len   = 0;              /* number of stored primes */
static uint64_t last_checked = 0;     /* last candidate prime examined */

/* Ensure the cache can hold at least `needed` elements. */
static void grow_cache(size_t needed)
{
    if (cap < needed) {
        size_t new_cap = cap ? cap * 2 : 16;
        while (new_cap < needed) new_cap <<= 1;
        seq_t *tmp;
        if (cap == 0) {
            tmp = seq_alloc(new_cap);
        } else {
            tmp = seq_realloc(cache, cap, new_cap);
        }
        if (!tmp) abort();
        cache = tmp;
        cap   = new_cap;
    }
}

/* Find the next qualifying prime and append it to the cache. */
static void advance_cache(void)
{
    for (;;) {
        uint64_t cand = next_prime(last_checked);  /* next prime after last_checked */
        seq_t extended = ((seq_t)cand) * 100 + 11;
        if (is_prime_big(extended)) {
            grow_cache(len + 1);
            cache[len++] = (seq_t)cand;
            last_checked = cand;
            break;
        }
        /* If cand fails, continue searching from this prime */
        last_checked = cand;
    }
}

/* Return the n‑th element of the sequence (1‑based index). */
seq_t a(idx_t n)
{
    if (n == 0) abort();          /* sequence offset is 1 */
    while (len < n) advance_cache();
    return cache[n - 1];
}
