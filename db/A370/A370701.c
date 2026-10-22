/*
 * A370701  Expansion of e.g.f. exp(-x^3)/(1-x).
 * https://oeis.org/A370701
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n! * Σ_{k=0..⌊n/3⌋} (-1)^k/k!
 * Range:     u8 n <= 5, u16 n <= 8, u32 n <= 13, u64 n <= 20, u128 n <= 22,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the nth term of A370701.
 * Uses memoisation: values are computed in increasing order up to the
 * largest n requested so far. Recurrence:
 *   a(0) = 1,
 *   for n > 0:
 *     let f[n] = n!  (computed incrementally)
 *     if n % 3 != 0:
 *          a[n] = n * a[n-1]
 *     else:
 *          term = f[n] / f[n/3]
 *          if (n/3) even: a[n] = n * a[n-1] + term
 *          else:          a[n] = n * a[n-1] - term
 */

#include <stdlib.h>

static seq_t *fact_cache = NULL;   /* fact_cache[i] = i!   */
static seq_t *a_cache   = NULL;   /* a_cache[i]   = a(i)  */
static size_t cache_len = 0;       /* number of entries stored */

/* Ensure caches are initialised with a(0)=1, f(0)=1 */
static void init_cache(void)
{
    if (cache_len == 0) {
        cache_len = 1;
        fact_cache = (seq_t*)malloc(sizeof(seq_t));
        a_cache   = (seq_t*)malloc(sizeof(seq_t));
        if (!fact_cache || !a_cache) {
            /* allocation failed – just abort; memory is small here */
            abort();
        }
        fact_cache[0] = 1;
        a_cache[0]    = 1;
    }
}

/* Grow caches to contain indices up to n (inclusive) */
static void grow_to(idx_t n)
{
    if ((size_t)n + 1 <= cache_len) return;

    size_t new_len = (size_t)n + 1;
    fact_cache = (seq_t*)realloc(fact_cache, new_len * sizeof(seq_t));
    a_cache   = (seq_t*)realloc(a_cache,   new_len * sizeof(seq_t));
    if (!fact_cache || !a_cache) abort();

    for (size_t i = cache_len; i < new_len; ++i) {
        /* compute factorial */
        fact_cache[i] = fact_cache[i-1] * (seq_t)i;

        if (i % 3 == 0) {
            /* extra term from the summation */
            seq_t term = fact_cache[i] / fact_cache[i/3];
            if ((i/3) % 2 == 0) {
                a_cache[i] = (seq_t)i * a_cache[i-1] + term;
            } else {
                a_cache[i] = (seq_t)i * a_cache[i-1] - term;
            }
        } else {
            a_cache[i] = (seq_t)i * a_cache[i-1];
        }
    }
    cache_len = new_len;
}

seq_t a(idx_t n)
{
    init_cache();
    grow_to(n);
    return a_cache[n];
}
