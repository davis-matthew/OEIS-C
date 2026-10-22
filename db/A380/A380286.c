/*
 * A380286  Number of distinct values of the number of regions between the
 *          free polyominoes with n cells and their bounding boxes.
 * https://oeis.org/A380286
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = A004525(n+4) for n ≥ 5. It satisfies the linear
 *            recurrence a(n) = 2*a(n-1) – 2*a(n-2) + 2*a(n-3) – a(n-4) for n
 *            ≥ 9 with initial values a(1)=1, a(2)=1, a(3)=2, a(4)=3, a(5)=5,
 *            a(6)=5, a(7)=5, a(8)=6
 * Range:     u8 n <= 507, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;        /* cached values, index 0 unused */
static idx_t cache_size = 0;       /* maximum index currently stored */

/* Ensure that cache[n] is computed for all indices up to n. */
static void ensure_cache(idx_t n) {
    if (n <= cache_size)
        return;

    size_t new_cap = (size_t)(n + 1);          /* allocate indices 0..n */
    seq_t *new_cache = (seq_t *)realloc(cache,
                                         new_cap * sizeof(seq_t));
    if (!new_cache) {
        /* Memory allocation failed – abort the program. */
        abort();
    }
    cache = new_cache;

    /* Initial values for indices 1..8 (index 0 unused). */
    static const seq_t init[9] = {0, 1, 1, 2, 3, 5, 5, 5, 6};

    idx_t prev = cache_size;
    for (idx_t i = prev + 1; i <= n; ++i) {
        if (i <= 8) {
            cache[i] = init[i];
        } else {
            seq_t t1 = (seq_t)(2 * cache[i - 1] + 2 * cache[i - 3]);
            seq_t t2 = (seq_t)(2 * cache[i - 2] + cache[i - 4]);
            cache[i] = t1 - t2;
        }
    }
    cache_size = n;
}

seq_t a(idx_t n)
{
    if (n == 0) {
        return (seq_t)0;   /* undefined for n=0, return 0 for safety */
    }
    ensure_cache(n);
    return cache[n];
}
