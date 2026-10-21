/*
 * A276599  Values of n such that n^2 + 5 is a triangular number (A000217).
 * https://oeis.org/A276599
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 6*a(n-2) - a(n-4) (n > 4), a(1)=1, a(2)=4, a(3)=10,
 *            a(4)=25
 * Range:     u8 n <= 6, u16 n <= 12, u32 n <= 25, u64 n <= 50, u128 n <= 101,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* External types supplied by the build system */
/* seq_t : unsigned integer (fixed or arbitrary‑precision) */
/* idx_t : unsigned 64‑bit integer for indices */

static seq_t *cache = NULL;   /* zero‑based array of a(k) values */
static idx_t cache_len = 0;   /* number of elements stored (<= cache capacity) */

static void ensure_cache(idx_t n)
{
    if (n <= cache_len) return;

    /* Grow cache to hold n elements (1‑based indices) */
    seq_t *tmp = (seq_t *)realloc(cache, sizeof(seq_t) * (size_t)n);
    if (tmp == NULL) abort();
    cache = tmp;

    /* Compute each new term */
    for (idx_t i = cache_len + 1; i <= n; ++i) {
        idx_t idx = i - 1;   /* zero‑based index for a(i) */

        if (i == 1)
            cache[idx] = (seq_t)1;
        else if (i == 2)
            cache[idx] = (seq_t)4;
        else if (i == 3)
            cache[idx] = (seq_t)10;
        else if (i == 4)
            cache[idx] = (seq_t)25;
        else {
            /* a(i) = 6*a(i-2) - a(i-4) */
            cache[idx] = (seq_t)6 * cache[idx - 2] - cache[idx - 4];
        }
    }
    cache_len = n;
}

/* Return a(n) for the sequence A276599 (offset 1). */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;   /* undefined for n < 1 */
    ensure_cache(n);
    return cache[n - 1];
}
