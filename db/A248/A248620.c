/*
 * A248620  Lesser of twin primes of (29n + 1, 29n + 3).
 * https://oeis.org/A248620
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the n‑th integer of the form 29k+1 such that 29k+1 and
 *            29k+3 are both prime (the lesser of the twin primes). The
 *            function searches for twin primes of the form 29k+1, 29k+3 and
 *            caches the 29k+1 values
 * Range:     u8 n <= 1, u16 n <= 23, u32 n <= 28, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/is_prime_big.h"   /* bool is_prime_big(seq_t) */

static seq_t *cache     = NULL;   /* cached 29k+1 values */
static size_t cache_len = 0;      /* number of stored terms      */
static size_t cache_cap = 0;      /* allocated capacity          */
static idx_t  last_k    = 1;      /* last k examined (start at 1) */

/*--- dynamic array helpers ----------------------------------------*/
static void ensure_capacity(size_t needed)
{
    if (needed <= cache_cap)
        return;
    size_t new_cap = (needed > 4) ? needed * 2 : 4;   /* start reasonably */
    seq_t *new_buf = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!new_buf)               /* allocation failed */
        abort();
    cache = new_buf;
    cache_cap = new_cap;
}

/*--- generate terms up to n_needed --------------------------------*/
static void generate_up_to(idx_t n_needed)
{
    size_t needed = (size_t)n_needed;
    while (cache_len < needed) {
        ++last_k;                            /* try next k          */
        seq_t p  = (seq_t)29 * (seq_t)last_k + (seq_t)1;
        seq_t p2 = p + (seq_t)2;             /* 29k+3               */
        if (is_prime_big(p) && is_prime_big(p2)) {
            ensure_capacity(cache_len + 1);
            cache[cache_len++] = p;          /* store 29k+1         */
        }
    }
}

/*--- public function ----------------------------------------------*/
seq_t a(idx_t n)
{
    if (n == 0)            /* outside offset, return 0 as safe default */
        return (seq_t)0;
    generate_up_to(n);     /* ensure we have at least n terms */
    return cache[(size_t)n - 1];   /* zero‑based array index */
}
