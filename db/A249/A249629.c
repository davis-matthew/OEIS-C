/*
 * A249629  Number of strings of length n over a 4-letter alphabet that begin
 *          with a nontrivial palindrome.
 * https://oeis.org/A249629
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn, walk
 * Formula:   a(0)=0, a(1)=0, a(n+1)=4*a(n)+4^ceil((n+1)/2)-a(ceil((n+1)/2))
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 16, u64 n <= 32, u128 n <= 64,
 *            big any n
 * Validated: standard, pass, 44 terms through n = 882 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Uses recurrence from OEIS A249629, memoizing results in a static array */

#include <stdlib.h>
#include "../utils/mem.h"

static inline seq_t pow4(idx_t exp) {
    seq_t result = 1;
    seq_t base   = 4;
    idx_t e = exp;
    while (e) {
        if (e & 1) result *= base;
        base *= base;
        e >>= 1;
    }
    return result;
}

static seq_t *cache = NULL;          /* memoization array */
static size_t cap   = 0;             /* allocated capacity */
static idx_t   max_cached = 1;       /* highest index computed so far */

/* Ensure the cache can address indices 0 .. n */
static void
ensure_cache(idx_t n)
{
    if (n + 1 <= cap)
        return;                     /* already large enough */

    size_t new_cap = cap == 0 ? 4 : cap;
    while (new_cap <= (size_t)n)
        new_cap <<= 1;               /* double until big enough */

    seq_t *tmp = mem_resize(seq_t, cache, new_cap);
    if (!tmp) abort();               /* allocation failure handled by abort */
    cache = tmp;
    cap   = new_cap;

    /* initialise base cases after a new allocation */
    cache[0] = 0;
    cache[1] = 0;
}

/* Main sequence function */
seq_t a(idx_t n)
{
    if (n < 2)
        return 0;                     /* a(0)=a(1)=0 */

    ensure_cache(n);

    if (n > max_cached) {
        /* compute all values up to n, starting from max_cached+1 */
        for (idx_t i = max_cached; i < n; ++i) {
            idx_t k = (i + 2) / 2;          /* ceil((i+1)/2) */
            seq_t term = cache[i] * 4 + pow4(k) - cache[k];
            cache[i + 1] = term;
        }
        max_cached = n;
    }

    return cache[n];
}
