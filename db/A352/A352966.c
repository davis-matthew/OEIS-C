/*
 * A352966  Primes of the form A321513(k) + 1 for some k > 0.
 * https://oeis.org/A352966
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 42, u32 n <= 42, u64 n <= 42, u128 n <= 42,
 *            big any n
 * Helpers:   has_periodic, is_prime_big, seq_alloc, seq_realloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The function computes OEIS A352966: primes p such that p-1, when written
 * in base‑4, can be padded with leading zeros so that the resulting string
 * is periodic with at least two identical repetitions. */

/* Needed utilities */
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include "../utils/is_prime_big.h"
#include "../utils/seq_alloc.h"
#include "../utils/seq_realloc.h"
#include "../utils/has_periodic.h"

/* Cache for memoised sequence values (1‑based indexing). */
static seq_t *cache      = NULL;   /* array of found primes            */
static size_t cache_cap  = 0;      /* current allocated capacity       */
static idx_t  cache_len  = 0;      /* number of stored primes in cache */
static idx_t  next_start = 2;      /* next integer to test for prime  */

/* Grow the cache array when it is full. */
static void grow_cache(void)
{
    if (cache_cap == 0) {
        cache_cap = 16;
        cache = seq_alloc(cache_cap);
    } else {
        size_t old_cap = cache_cap;
        cache_cap *= 2;
        cache = seq_realloc(cache, old_cap, cache_cap);
    }
}

/* Return the n‑th term of the sequence (a(1) = 11).  This routine
 * works for arbitrary‑precision seq_t as well as for built‑in integer
 * types. */
seq_t a(idx_t n)
{
    if (n < 1)
        return (seq_t)0;           /* undefined for n = 0; optional */

    /* Generate terms until we have at least n entries. */
    while (cache_len < n) {
        seq_t cand = (seq_t)next_start++;   /* candidate integer to test */

        if (!is_prime_big(cand))
            continue;                 /* skip composites */

        if (has_periodic(cand - 1)) {
            if (cache_len == cache_cap)
                grow_cache();
            cache[cache_len++] = cand;  /* store prime */
        }
    }
    return cache[n - 1];
}
