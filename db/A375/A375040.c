/*
 * A375040  The maximum exponent in the prime factorization of 2*n.
 * https://oeis.org/A375040
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = maximum exponent of a prime factor in 2*n
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   compute_a_value
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Maximum exponent in prime factorisation of 2*n  (OEIS A375040) */
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <stdbool.h>
#include "../utils/compute_a_value.h"

static seq_t *a_cache = NULL;      /* cache of computed values */
static idx_t a_cap   = 0;          /* size of a_cache */

static void ensure_cache(idx_t n)
{
    if (n < a_cap) return;                /* already big enough */
    idx_t old_cap = a_cap;
    idx_t new_cap = (a_cap == 0) ? 32 : a_cap * 2;
    if (new_cap <= n) new_cap = n + 1;
    seq_t *tmp = (seq_t *)realloc(a_cache, new_cap * sizeof(seq_t));
    if (!tmp) abort();                    /* allocation failed */
    /* zero‑out the newly allocated portion */
    if (new_cap > old_cap)
        memset(tmp + old_cap, 0, (new_cap - old_cap) * sizeof(seq_t));
    a_cache = tmp;
    a_cap   = new_cap;
}

/* External entry point */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;      /* undefined part of the sequence */

    ensure_cache(n);

    if (a_cache[n] == 0)             /* 0 is unused – all a(n) >= 1 */
        a_cache[n] = compute_a_value(n);

    return a_cache[n];
}
