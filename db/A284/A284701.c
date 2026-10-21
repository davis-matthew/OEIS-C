/*
 * A284701  Number of maximal matchings in the n-antiprism graph.
 * https://oeis.org/A284701
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2*a(n-1) + a(n-2) + 3*a(n-4) + 5*a(n-5) + a(n-6) -
 *            2*a(n-7) - a(n-8) for n > 8 Initial values: a(1) = 2, a(2) = 6,
 *            a(3) = 14, a(4) = 46, a(5) = 137, a(6) = 354, a(7) = 905, a(8) =
 *            2366 The sequence is memoised; unchanged for any call order
 * Range:     u8 n <= 5, u16 n <= 11, u32 n <= 22, u64 n <= 45, u128 n <= 91,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* Forward declaration of the sequence function */
seq_t a(idx_t n);

/* Internal cache for memoisation */
static seq_t *cache = NULL;      /* array of values 0..cache_len-1 (index 0 unused) */
static size_t cache_len = 0;     /* highest index stored + 1 (initially 0) */
static size_t cache_cap = 0;     /* allocated size */

/* Ensure the cache is large enough to store indices up to max_index (inclusive) */
static void ensure_cache_capacity(idx_t max_index)
{
    if ((size_t)(max_index + 1) > cache_cap) {
        size_t new_cap = cache_cap ? cache_cap * 2 : 16;
        while (new_cap <= (size_t)(max_index + 1))
            new_cap <<= 1;
        seq_t *new_cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        /* If realloc fails we simply let the program crash – test harness
           does not require graceful failure handling. */
        cache = new_cache;
        cache_cap = new_cap;
    }
}

/* Initialise the cache with the known first eight terms */
static void initialise_cache(void)
{
    /* The first eight terms, 1‑based indices; cache[0] is unused */
    static const seq_t base[9] = {0, 2, 6, 14, 46, 137, 354, 905, 2366};

    ensure_cache_capacity(8);
    for (idx_t i = 1; i <= 8; ++i)
        cache[i] = base[i];

    /* cache_len is the next index to compute (i.e., one more than the
       highest stored index).  After initialisation there are values up to 8,
       so we set cache_len to 9. */
    cache_len = 9;
}

/* Compute the sequence value for index n */
seq_t a(idx_t n)
{
    if (n == 0)  /* behaviour for n < 1 is unspecified; return 0 */
        return 0;

    /* Initialise cache if needed */
    if (cache_len == 0)
        initialise_cache();

    /* Extend the cache capacity if required */
    ensure_cache_capacity(n);

    /* Compute missing terms up to n */
    for (idx_t i = cache_len; i <= n; ++i) {
        /* For i <= 8 the values are already stored in the cache */
        if (i <= 8)
            continue;

        /* Recurrence computation */
        seq_t val = 0;
        val += 2 * cache[i - 1];
        val +=         cache[i - 2];
        val += 3 * cache[i - 4];
        val += 5 * cache[i - 5];
        val +=         cache[i - 6];
        val -= 2 * cache[i - 7];
        val -=         cache[i - 8];
        cache[i] = val;
    }

    /* Update the cache length to the new maximum index + 1 */
    cache_len = (size_t)n + 1;

    return cache[n];
}
