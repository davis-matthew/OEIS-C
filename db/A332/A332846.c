/*
 * A332846  a(1) = 1; a(n+1) = Sum_{k=1..n} a(k) * ceiling(n/k).
 * https://oeis.org/A332846
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 7, u16 n <= 14, u32 n <= 26, u64 n <= 33, u128 n <= 33,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(1) = 1; a(n+1) = ∑_{k=1..n} a(k) * ceil(n/k)  (OEIS A332846) */
#include <stdint.h>
#include <stdlib.h>

/* Permanent cache for already computed values.  cache[0] is unused. */
static seq_t *cache      = NULL;   /* pointer to the array   */
static idx_t  cache_cap  = 0;      /* largest index allocated */
static idx_t  current_n  = 0;      /* highest n that has been computed */

/* ------------------------------------------------------------------ */
/* Ensure that cache can address indices up to n (inclusive).         */
/* ------------------------------------------------------------------ */
static void
ensure_cache(idx_t n)
{
    if (n <= cache_cap)
        return;

    /* Grow the allocation, at least doubling each time. */
    idx_t new_cap = cache_cap ? cache_cap : 1;
    while (new_cap < n)
        new_cap <<= 1;                 /* multiply by 2 */

    /* Reallocate memory.  cast the result of realloc to the build‑type. */
    seq_t *tmp = (seq_t *)realloc(cache, (new_cap + 1) * sizeof(seq_t));
    if (!tmp)
        abort();                      /* allocation failed */

    /* Initialise all newly extended slots to zero.  */
    for (idx_t i = cache_cap + 1; i <= n; ++i)
        tmp[i] = 0;

    cache     = tmp;
    cache_cap = new_cap;
}

/* ------------------------------------------------------------------ */
/* Sequence function.  Computes or retrieves a(n).                    */
/* ------------------------------------------------------------------ */
seq_t
a(idx_t n)
{
    if (n == 0)
        return 0;                     /* undefined; safe fallback     */

    ensure_cache(n);

    /* Initialise seed if first call. */
    if (current_n == 0)
    {
        cache[1] = 1;                 /* a(1) = 1 */
        current_n = 1;
    }

    /* Compute up to n, if necessary.  The order of calls is arbitrary. */
    while (current_n < n)
    {
        idx_t  next = current_n + 1;         /* we will compute a(next) */
        seq_t  sum  = 0;

        for (idx_t k = 1; k <= current_n; ++k)
        {
            /* ceil((next-1)/k) = (next-1 + k-1)/k             */
            idx_t val = (next - 1 + k - 1) / k;
            sum += cache[k] * ((seq_t)val);
        }

        cache[next] = sum;
        current_n = next;
    }

    return cache[n];
}
