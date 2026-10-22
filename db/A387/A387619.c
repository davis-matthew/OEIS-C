/*
 * A387619  Number of ways to tile a 2 X n strip with squares, dominoes, and
 *          (straight and bent) trominoes.
 * https://oeis.org/A387619
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 14, u64 n <= 24, u128 n <= 24,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A387619:
 *   a(n) = 3*a(n-1) + 6*a(n-2) + 7*a(n-3) + 2*a(n-4) - a(n-6)
 *   a(0) = 1, a(1) = 2, a(2) = 11, a(3) = 51,
 *   a(4) = 235, a(5) = 1092
 *
 *   Computes values modulo the bit‑width of seq_t – exact when
 *   the result fits in seq_t.
 */

#include <stdlib.h>
#include <stdint.h>

/* Multiply a seq_t value by a small unsigned int, using a wider type
 * if available to avoid overflow in intermediate operations.  */
static inline seq_t mul_small(seq_t x, unsigned int m)
{
#ifdef __SIZEOF_INT128__
    unsigned __int128 prod = (unsigned __int128)x * m;
    return (seq_t)prod;
#else
    uint64_t prod = (uint64_t)x * m;
    return (seq_t)prod;
#endif
}

static seq_t *cache = NULL;   /* memoisation array               */
static size_t cap   = 0;       /* allocated capacity of cache     */
static size_t last_computed = 0; /* largest index that has been computed */

/* Ensure the cache holds values up to index n and that all values up
 * to n are computed.  */
static void ensure_cache(idx_t n)
{
    if (n <= last_computed)
        return;

    size_t needed = (size_t)n + 1;
    if (needed > cap) {
        size_t newcap = cap ? cap * 2 : 16;
        if (newcap < needed)
            newcap = needed;
        seq_t *tmp = (seq_t *)realloc(cache, newcap * sizeof(seq_t));
        if (!tmp)
            abort();                       /* unrecoverable failure */
        cache = tmp;
        cap   = newcap;
    }

    /* Initialise first six values if not already done.             */
    if (last_computed < 5) {
        static const seq_t init[6] = { 1, 2, 11, 51, 235, 1092 };
        size_t upto = needed < 6 ? needed : 6;
        for (size_t i = 0; i < upto; ++i)
            cache[i] = init[i];
        last_computed = upto - 1;
    }

    /* Compute any missing terms up to n.                           */
    for (idx_t i = (idx_t)last_computed + 1; i <= n; ++i) {
        seq_t val = mul_small(cache[i-1], 3);
        val += mul_small(cache[i-2], 6);
        val += mul_small(cache[i-3], 7);
        val += mul_small(cache[i-4], 2);
        val -= cache[i-6];
        cache[i] = val;
    }
    last_computed = n;
}

/* Public function: return a(n).  */
seq_t a(idx_t n)
{
    if (n < 6) {
        static const seq_t init[6] = { 1, 2, 11, 51, 235, 1092 };
        return init[n];
    }
    ensure_cache(n);
    return cache[n];
}
