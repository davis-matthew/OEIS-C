/*
 * A271970  Linear recurrence, with both signature and original terms =
 *          1,0,1,0,1.
 * https://oeis.org/A271970
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n‑1) + a(n‑3) + a(n‑5) with initial values a(1)=1,
 *            a(2)=0, a(3)=1, a(4)=0, a(5)=1 (offset 1)
 * Range:     u8 n <= 16, u16 n <= 28, u32 n <= 43, u64 n <= 43, u128 n <= 43,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

/* Preallocate a cache big enough for the typical use‑case (n ≤ 10000). */
static seq_t cache[10001];
static idx_t max_comp = 0;                /* highest index for which cache is valid */
static bool   initialized = false;

/* Initialise the first five terms once. */
static void init_cache(void)
{
    if (initialized)
        return;
    cache[1] = (seq_t)1;
    cache[2] = (seq_t)0;
    cache[3] = (seq_t)1;
    cache[4] = (seq_t)0;
    cache[5] = (seq_t)1;
    max_comp = 5;
    initialized = true;
}

/* --- Public function ----------------------------------------------------- */
seq_t a(idx_t n)
{
    init_cache();

    /* Return immediately for already computed values. */
    if (n <= max_comp)
        return cache[n];

    /* If the requested n is beyond the statically allocated size,
       compute the missing terms on the fly but keep the cache size fixed. */
    if (n > 10000) {
        seq_t val = 0;
        for (idx_t k = max_comp + 1; k <= n; ++k) {
            val = cache[k-1] + cache[k-3] + cache[k-5];
            if (k <= 10000)               /* keep the cache small */
                cache[k] = val;
        }
        max_comp = 10000;
        return val;
    }

    /* Compute and store missing terms up to n. */
    for (idx_t k = max_comp + 1; k <= n; ++k) {
        cache[k] = cache[k-1] + cache[k-3] + cache[k-5];
    }
    max_comp = n;
    return cache[n];
}
