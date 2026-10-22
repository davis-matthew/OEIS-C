/*
 * A307993  G.f. A(x) satisfies: A(x) = x + x^2 + x^3 * (1 + A(x) + A(x^2) +
 *          A(x^3) + ...).
 * https://oeis.org/A307993
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1)=a(2)=a(3)=1; for n >= 1, a(n+3) = Σ_{d|n} a(d)
 * Range:     u8 n <= 53, u16 n <= 69, u32 n <= 69, u64 n <= 69, u128 n <= 69,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The sequence is computed iteratively in O(n log n) time. */

#include <stdlib.h>
#include <stdint.h>

static seq_t *cache = NULL;      /* array holding a[1 .. n]  */
static idx_t cache_cap = 0;      /* current capacity of 'cache' */

/* Recompute the sequence values up to at least 'n' (inclusive). */
static void compute_to(idx_t n)
{
    /* Allocate or enlarge the cache if necessary.  The array is 1‑indexed. */
    if (cache_cap < n + 1) {
        cache = (seq_t *)realloc(cache, (n + 1) * sizeof(seq_t));
        if (!cache) abort();
        cache_cap = n + 1;
    }

    /* Zero the whole relevant part. */
    for (idx_t i = 0; i <= n; ++i) cache[i] = 0;

    /* Initial values. */
    if (n >= 1) cache[1] = 1;
    if (n >= 2) cache[2] = 1;
    if (n >= 3) cache[3] = 1;

    /* Fill the remaining values using the divisor recurrence. */
    if (n >= 4) {
        for (idx_t d = 1; d <= n - 3; ++d) {
            seq_t val = cache[d];
            for (idx_t t = d; t <= n - 3; t += d) {
                cache[t + 3] += val;
            }
        }
    }
}

/* Public function called by the test harness. */
seq_t a(idx_t n)
{
    if (n < 1) return 0;  /* undefined for n < 1, but harmless */
    compute_to(n);
    return cache[n];
}
