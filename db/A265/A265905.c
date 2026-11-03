/*
 * A265905  a(1) = 1; for n > 1, a(n) = a(n-1) + A153880(a(n-1)).
 * https://oeis.org/A265905
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(1) = 1; for n > 1, a(n) = a(n-1) + A153880(a(n-1)). A153880(x)
 *            = sum_{k=1..m} d_k * (k+1)! where d_k are the factorial‑base
 *            digits of x
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 12, u64 n <= 20, u128 n <= 33,
 *            big any n
 * Validated: standard, pass, 40 terms through n = 117 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/mem.h"

/* helper: compute A153880(x) for a given seq_t value */
static seq_t A153880(seq_t x)
{
    if (x == 0) return (seq_t)0;

    /* find m such that m! <= x < (m+1)! */
    seq_t fact = (seq_t)1;   /* 1! */
    int k = 1;
    while (1) {
        seq_t next_fact = fact * (seq_t)(k + 1);  /* (k+1)! */
        if (next_fact > x) break;
        fact = next_fact;
        k++;
    }
    /* now fact == m! with current k = m */
    seq_t sum = (seq_t)0;
    seq_t rem = x;

    /* iterate k from m down to 1 */
    for (int cur = k; cur >= 1; --cur) {
        seq_t next_fact = fact * (seq_t)(cur + 1);      /* (cur+1)! */
        seq_t digit      = rem / fact;                 /* d_cur */
        rem              = rem % fact;
        sum              += digit * next_fact;
        /* move to next lower factorial: (cur-1)! = cur! / cur */
        fact = fact / (seq_t)cur;
    }
    return sum;
}

/* static cache for memoisation */
static seq_t *cache = NULL;
static size_t cache_len = 0;   /* number of terms stored (a(1) at index 0) */
static size_t cache_cap = 0;   /* allocated capacity */

/* ensure the cache contains terms up to 'n' (1‑based indexing) */
static void ensure_cache(idx_t n)
{
    /* n is at least 1 */
    if (n == 0) return;

    /* first term a(1) */
    if (cache_len == 0) {
        /* allocate space for at least one term */
        if (cache_cap < 1) {
            cache_cap = 1;
            cache = mem_new(seq_t, cache_cap);
        }
        cache[0] = (seq_t)1;      /* a(1) */
        cache_len = 1;
    }

    /* grow capacity if necessary */
    if (cache_cap < (size_t)n) {
        size_t new_cap = cache_cap * 2;
        if (new_cap < (size_t)n) new_cap = (size_t)n;
        cache = mem_resize(seq_t, cache, new_cap);
        cache_cap = new_cap;
    }

    /* compute missing terms */
    for (size_t i = cache_len; i < (size_t)n; ++i) {
        seq_t prev = cache[i - 1];           /* a(i) = a(i-1) + A153880(a(i-1)) */
        seq_t added = A153880(prev);
        cache[i] = prev + added;
    }
    cache_len = (size_t)n;
}

/* PUBLIC API ------------------------------------------------------- */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;        /* out of bounds, not used */
    ensure_cache(n);
    return cache[(size_t)(n - 1)];
}
