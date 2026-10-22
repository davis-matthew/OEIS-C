/*
 * A320393  First members of the Cunningham chains of the first kind whose
 *          length is a prime.
 * https://oeis.org/A320393
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is the n‑th prime p such that the Cunningham chain of * the
 *            first kind starting at p has a prime length
 * Range:     u8 n <= 5, u16 n <= 60, u32 n <= 60, u64 n <= 60, u128 n <= 60,
 *            big any n
 * Helpers:   is_prime_big, is_prime_small, seq_alloc, seq_realloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include "../utils/is_prime_big.h"
#include "../utils/is_prime_small.h"
#include "../utils/seq_alloc.h"
#include "../utils/seq_realloc.h"

/* cache of found terms */
static seq_t *cache      = NULL;  /* array of seq_t values          */
static size_t cache_cap  = 0;     /* capacity of 'cache'            */
static size_t cache_len  = 0;     /* number of stored values        */
static seq_t  next_prime = 0;     /* next integer to test for prime*/

/* enlarge the cache if needed */
static void ensure_cache_cap(void)
{
    if (cache_len >= cache_cap) {
        size_t new_cap = cache_cap ? cache_cap * 2 : 16;
        seq_t *tmp = seq_realloc(cache, cache_cap, new_cap);
        if (!tmp) abort();
        cache     = tmp;
        cache_cap = new_cap;
    }
}

/* Main sequence function: returns the n‑th prime p such that the
 * Cunningham chain of the first kind starting at p has a prime length. */
seq_t a(idx_t n)
{
    if (n < 1) return 0;                   /* not defined for n<1 */

    if (next_prime == 0) next_prime = 2;   /* lazy initialization */

    while (cache_len < (size_t)n) {
        /* find the next prime */
        while (!is_prime_big(next_prime))
            next_prime += 1;

        seq_t cand = next_prime;   /* candidate prime */
        idx_t  len = 1;            /* current chain length (including cand) */
        seq_t cur = cand;

        /* build the Cunningham chain of the first kind */
        while (true) {
            seq_t nxt = cur * 2 + 1;
            if (is_prime_big(nxt)) {
                ++len;
                cur = nxt;
            } else
                break;
        }

        /* if the length itself is prime, record the starting prime */
        if (is_prime_small((uint64_t)len)) {
            ensure_cache_cap();
            cache[cache_len++] = cand;
        }

        next_prime += 1; /* continue searching after this candidate */
    }

    return cache[(size_t)n - 1];
}
