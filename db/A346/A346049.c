/*
 * A346049  a(0) = ... = a(4) = 1; a(n) = Sum_{k=1..n-5} a(k) * a(n-k-5).
 * https://oeis.org/A346049
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0)=a(1)=a(2)=a(3)=a(4)=1; a(n)=Σ_{k=1}^{n-5} a(k)·a(n-k-5)
 *            (n≥5)
 * Range:     u8 n <= 22, u16 n <= 37, u32 n <= 65, u64 n <= 120,
 *            u128 n <= 228, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the OEIS sequence A346049.  The implementation uses
   a lazy, memoised array; the cache grows as needed.  It works
   for all supported integer sizes and arbitrary-precision integer
   classes. */

#include <stdlib.h>
#include <stdint.h>

static seq_t *cache = NULL;     /* memoisation array */
static idx_t  cache_cap = 0;    /* allocated size (in elements) */
static idx_t  next      = 0;    /* next index to compute */

/* Ensure the array can hold indices up to n inclusive. */
static void ensure_cache(idx_t n)
{
    if (n < cache_cap)  return;          /* already big enough */

    size_t new_cap = (size_t)(n + 1);
    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!tmp) abort();                   /* memory allocation failed */
    cache      = tmp;
    cache_cap  = new_cap;
}

/* Sequence a(n). */
seq_t a(idx_t n)
{
    if (n < next)                      /* already computed */
        return cache[n];

    ensure_cache(n);                   /* make sure storage exists */

    for (idx_t i = next; i <= n; ++i)  /* compute missing elements */
    {
        if (i <= 4) {
            cache[i] = (seq_t)1;
        } else {
            seq_t sum = (seq_t)0;
            /* summation: a(i) = Σ_{k=1}^{i-5} a(k)·a(i-k-5) */
            for (idx_t k = 1; k <= i - 5; ++k)
                sum += cache[k] * cache[i - k - 5];
            cache[i] = sum;
        }
    }

    next = n + 1;                      /* all indices up to n are now valid */
    return cache[n];
}
