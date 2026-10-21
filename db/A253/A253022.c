/*
 * A253022  Number of (n+2)X(5+2) 0..3 arrays with every consecutive three
 *          elements in every row and column not having exactly two distinct
 *          values, and in every diagonal and antidiagonal having exactly two
 *          distinct values, and new values 0 upwards introduced in row major
 *          order.
 * https://oeis.org/A253022
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 23, u32 n <= 63, u64 n <= 141, u128 n <= 210,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A253022
   Recurrence: a(n) = 5*a(n-3) - 6*a(n-6) - 2*a(n-9) + 2*a(n-12) + 3*a(n-15) + a(n-18)
   for n > 22.
   Initial terms (a(1) … a(22)):
   105, 133, 219, 230, 327, 537, 554, 753, 1193, 1250, 1721,
   2747, 2894, 3987, 6343, 6694, 9207, 14609, 15474, 21273,
   33681, 35802.  The first term is a(1); a(0) is unused.  */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

/* The array contains an unused 0 entry so that a(n) can be accessed
   directly with its OEIS index.  Each constant is cast to seq_t, so
   the value is stored modulo the width of seq_t, which is exactly
   the required behaviour for small types. */
static const seq_t base[23] = {
    0,
    (seq_t)105, (seq_t)133, (seq_t)219, (seq_t)230, (seq_t)327, (seq_t)537,
    (seq_t)554, (seq_t)753, (seq_t)1193, (seq_t)1250, (seq_t)1721,
    (seq_t)2747, (seq_t)2894, (seq_t)3987, (seq_t)6343, (seq_t)6694,
    (seq_t)9207, (seq_t)14609, (seq_t)15474, (seq_t)21273,
    (seq_t)33681, (seq_t)35802
};

seq_t a(idx_t n)
{
    /* Return the known initial terms directly. */
    if (n <= 22)
        return base[n];

    /* Static cache for already computed values. */
    static seq_t *cache = NULL;
    static size_t cache_cap = 0;
    static idx_t last_computed = 22;

    /* Ensure the cache is large enough. */
    if (cache == NULL) {
        cache_cap = (size_t)n + 1;
        cache = (seq_t*)malloc(cache_cap * sizeof(seq_t));
        if (cache == NULL) /* allocation failure; fall back */
            return base[22];
        for (idx_t i = 0; i <= 22; ++i)
            cache[i] = base[i];
    } else if ((size_t)n + 1 > cache_cap) {
        size_t new_cap = (size_t)n + 1;
        seq_t *tmp = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
        if (tmp == NULL) /* allocation failure; keep old cache */
            return cache[last_computed];
        cache = tmp;
        cache_cap = new_cap;
    }

    /* Compute missing terms using the recurrence. */
    for (idx_t i = last_computed + 1; i <= n; ++i) {
        /* All arithmetic is performed in seq_t; overflow
           behaves modulo 2^k where k is the width of seq_t. */
        seq_t val = 5 * cache[i - 3];
        val = val - 6 * cache[i - 6];
        val = val - 2 * cache[i - 9];
        val = val + 2 * cache[i - 12];
        val = val + 3 * cache[i - 15];
        val = val + cache[i - 18];
        cache[i] = val;
    }

    last_computed = n;
    return cache[n];
}
