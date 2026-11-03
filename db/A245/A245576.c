/*
 * A245576  Primes p such that p and p^2 lack the zero digit in their decimal
 *          expansions.
 * https://oeis.org/A245576
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   not stated
 * Range:     u8 n <= 7, u16 n <= 39, u32 n <= 2625, u64 n <= 3001,
 *            u128 n <= 3001, big any n
 * Validated: standard, pass, 74 terms through n = 2941 (2026-11-01)
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A245576: primes p such that p and p^2 contain no digit 0 */
#include <stdbool.h>
#include <stddef.h>
/* helpers from the utilities */
#include <stdlib.h>
#include "../utils/is_prime.h"

/* Grow a dynamic array when needed.  The pointer is passed as (void **). */
static void ensure_capacity(size_t needed, size_t *cap, void **ptr, size_t elem_size)
{
    if (needed <= *cap) return;
    size_t new_cap = (*cap == 0) ? 4 : (*cap << 1);
    while (new_cap < needed) new_cap <<= 1;
    void *tmp = realloc(*ptr, new_cap * elem_size);
    if (!tmp) abort();               /* on OOM we abort as required */
    *ptr = tmp;
    *cap = new_cap;
}

static seq_t *cache = NULL;    /* stored primes satisfying the condition */
static size_t  cache_cap = 0;   /* capacity of the cache            */
static size_t  cache_len = 0;   /* number of elements in the cache  */
static idx_t   last_checked = 1;/* last integer examined */

/* return true if decimal representation of x contains a digit 0 */
static bool has_zero(seq_t x) {
    while (x) {
        if ((x % 10u) == 0u) return true;
        x /= 10u;
    }
    return false;
}

/*  ---------------------------------------------------  */
seq_t a(idx_t n) {
    if (n == 0) return 0; /* behaviour below offset not relevant */

    /* acquire more primes if needed */
    while (cache_len < (size_t)n) {
        /* search for next candidate */
        ++last_checked;
        /* skip even numbers greater than 2 */
        if (last_checked > 2 && !(last_checked & 1u))
            ++last_checked;

        if (!is_prime(last_checked)) continue;

        seq_t p = (seq_t)last_checked;
        if (!has_zero(p) && !has_zero(p * p)) {
            /* store it */
            if (cache_len == cache_cap)
                ensure_capacity(cache_len + 1, &cache_cap,
                                (void **)&cache, sizeof(seq_t));
            cache[cache_len++] = p;
        }
    }
    return cache[n - 1];
}
