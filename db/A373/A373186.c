/*
 * A373186  Expansion of Sum_{k>=0} x^(3^k) / (1 - x^(3^k))^3.
 * https://oeis.org/A373186
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n*(n+1)/2 if n % 3 != 0 a(n) = n*(n+1)/2 + a(n/3) if n %
 *            3 == 0 a(1) = 1. This follows directly from the recurrence
 *            a(3*n+1) = T(3*n+1), a(3*n+2) = T(3*n+2), a(3*n+3) = T(3*n+3) +
 *            a(n+1), where T(k) = k*(k+1)/2 is the n‑th triangular number
 * Range:     u8 n <= 20, u16 n <= 55, u32 n <= 55, u64 n <= 55, u128 n <= 55,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>

/* Auxiliary: triangular number */
static inline seq_t triangular(idx_t n)
{
    return (seq_t)n * ((seq_t)n + 1u) / 2;
}

/* Dynamic cache */
static seq_t *cache = NULL;          /* cache[0] unused, cache[1] = a(1) ... */
static size_t cached_size = 0;        /* number of allocated slots */

/* Ensure cache can hold index n */
static void ensure_cache(idx_t n)
{
    size_t needed = (size_t)n + 1;
    if (needed <= cached_size)
        return;

    size_t new_size = needed;
    seq_t *new_cache = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
    if (!new_cache) {
        /* On allocation failure, abort */
        abort();
    }
    /* Zero the newly allocated part */
    memset(new_cache + cached_size, 0, (new_size - cached_size) * sizeof(seq_t));
    cache = new_cache;
    cached_size = new_size;
}

seq_t a(idx_t n)
{
    if (n == 0) /* undefined; return 0 as sentinel */
        return 0;

    ensure_cache(n);

    if (cache[n] != 0)          /* already computed */
        return cache[n];

    if (n % 3u == 0) {
        /* a(n) = T(n) + a(n/3) */
        cache[n] = triangular(n) + a(n / 3u);
    } else {
        /* a(n) = T(n) */
        cache[n] = triangular(n);
    }

    return cache[n];
}
