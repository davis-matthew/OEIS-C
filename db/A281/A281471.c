/*
 * A281471  Number of 3 X n 0..1 arrays with no element equal to more than two
 *          of its horizontal, diagonal or antidiagonal neighbors and with new
 *          values introduced in order 0 sequentially upwards.
 * https://oeis.org/A281471
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2*a(n-1) – a(n-2) + 2*a(n-3) – 2*a(n-4) (n>8) Initial
 *            values (n = 1..8): a(1) = 4, a(2) = 25, a(3) = 24, a(4) = 39,
 *            a(5) = 57, a(6) = 87, a(7) = 145, a(8) = 241
 * Range:     u8 n <= 8, u16 n <= 18, u32 n <= 39, u64 n <= 81, u128 n <= 165,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

static seq_t *cache = NULL;          /* dynamically grow‑able array of terms */
static idx_t cache_size = 0;         /* allocated size (number of elements) */

/* Hard‑coded initial values for indices 1..8 */
static const seq_t init_values[9] = {
    (seq_t)0,         /* unused, 0th element */
    (seq_t)4,  (seq_t)25, (seq_t)24, (seq_t)39,
    (seq_t)57, (seq_t)87, (seq_t)145, (seq_t)241
};

/* Ensure that the cache can hold at least new_cap elements (including index 0). */
static void ensure_cache_size(idx_t new_cap)
{
    if (cache_size >= new_cap)
        return;
    /* Grow: double the size or at least new_cap */
    size_t cap = cache_size < 16 ? 16 : cache_size;
    while (cap < new_cap)
        cap *= 2;
    seq_t *new_cache = (seq_t *)malloc(sizeof(seq_t) * cap);
    if (new_cache == NULL) {
        /* Out of memory – abort the program (no I/O allowed). */
        abort();
    }
    if (cache != NULL) {
        for (idx_t i = 0; i < cache_size; ++i)
            new_cache[i] = cache[i];
        free(cache);
    }
    cache = new_cache;
    cache_size = cap;
}

/* Compute a[n], caching results for future calls. */
seq_t a(idx_t n)
{
    if (n == 0) /* not part of the sequence but harmless */
        return (seq_t)0;

    /* Minimum cache size needed is n+1 (indices 0..n). */
    ensure_cache_size(n + 1);

    /* Compute any missing terms up to n. */
    for (idx_t i = 1; i <= n; ++i) {
        if (cache[i] != (seq_t)0)            /* already computed */
            continue;
        if (i <= 8) {                        /* use hard‑coded initial values */
            cache[i] = init_values[i];
        } else {
            /* recurrence: 2*a(i-1) – a(i-2) + 2*a(i-3) – 2*a(i-4) */
            seq_t t1 = 2 * cache[i - 1];
            seq_t t2 = cache[i - 2];
            seq_t t3 = 2 * cache[i - 3];
            seq_t t4 = 2 * cache[i - 4];
            cache[i] = t1 - t2 + t3 - t4;
        }
    }
    return cache[n];
}
