/*
 * A375040  The maximum exponent in the prime factorization of 2*n.
 * https://oeis.org/A375040
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = maximum exponent of a prime factor in 2*n
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 121 terms through n = 9904 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Maximum exponent in prime factorisation of 2*n  (OEIS A375040) */
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <stdbool.h>
#include "../utils/mem.h"

/* Compute a(n) for a single value, without caching */
static seq_t compute_a_value(idx_t n)
{
    /* n >= 1  */
    /* exponent of 2 in 2*n : 1 + v2(n) */
    unsigned int cnt2 = __builtin_ctzll((uint64_t)n);   /* n>0 */
    seq_t ans = (seq_t)(cnt2 + 1);                      /* at least 1 */

    /* odd part of n */
    uint64_t m = n >> cnt2;     /* m is odd, or 1 */

    if (m == 1) return ans;     /* no odd factor */

    /* factor the odd part and keep maximum exponent */
    /* use a copy so that the loop bound is fixed */
    uint64_t t = m;
    for (uint64_t p = 3; p <= t / p; p += 2)          /* p^2 <= t */
    {
        if (t % p == 0)
        {
            unsigned int exp = 0;
            do { t /= p; ++exp; } while (t % p == 0);
            if ((seq_t)exp > ans) ans = exp;
        }
    }
    /* any remaining prime factor has exponent 1, which cannot exceed ans>=1 */
    return ans;
}

static seq_t *a_cache = NULL;      /* cache of computed values */
static idx_t a_cap   = 0;          /* size of a_cache */

static void ensure_cache(idx_t n)
{
    if (n < a_cap) return;                /* already big enough */
    idx_t old_cap = a_cap;
    idx_t new_cap = (a_cap == 0) ? 32 : a_cap * 2;
    if (new_cap <= n) new_cap = n + 1;
    seq_t *tmp = mem_resize(seq_t, a_cache, new_cap);
    if (!tmp) abort();                    /* allocation failed */
    /* zero‑out the newly allocated portion */
    if (new_cap > old_cap)
        mem_set(tmp + old_cap, 0, (new_cap - old_cap) * sizeof(seq_t));
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
