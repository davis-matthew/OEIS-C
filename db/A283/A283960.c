/*
 * A283960  a(n) = (Sum_{j=1..h-1} a(n-j) + a(n-1)*a(n-h+1))/a(n-h) with a(1),
 *          ..., a(h)=1, where h = 6.
 * https://oeis.org/A283960
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = * a(1)..a(6) = 1 * for 7 ≤ n ≤ 15: * a(n) =
 *            (a(n‑1)+…+a(n‑5) + a(n‑1)*a(n‑5)) / a(n‑6) * for n > 15: * a(n)
 *            = 399*a(n‑5) - 399*a(n‑10) + a(n‑15) * (h = 6)
 * Range:     u8 n <= 10, u16 n <= 13, u32 n <= 24, u64 n <= 42, u128 n <= 80,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>

/* static cache: cache[i] holds a(i), 1‑based index, cache[0] unused */
static seq_t *cache = NULL;
static idx_t cache_len = 0;      /* number of allocated slots (>= n+1)  */
static idx_t computed_until = 0; /* largest index that has been computed */

/* Ensure the cache has capacity for indices up to n (inclusive). */
static void ensure_cache(idx_t n)
{
    if (cache_len > n)
        return;

    idx_t new_len = n + 1;                  /* indices 0..n   */
    seq_t *tmp = (seq_t *)realloc(cache, new_len * sizeof(seq_t));
    if (!tmp)
        return;                            /* allocation failed – leave cache unchanged */

    cache = tmp;
    /* initialise new slots to zero */
    for (idx_t i = cache_len; i < new_len; ++i)
        cache[i] = (seq_t)0;

    cache_len = new_len;

    /* initialise the base values a(1)..a(6) = 1 (re‑set in case of realloc) */
    for (idx_t i = 1; i <= 6 && i < cache_len; ++i)
        cache[i] = (seq_t)1;
}

/* Compute a(n).  Supports calls in any order and memoises results. */
seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;                       /* undefined, return 0 */

    ensure_cache(n);

    if (n <= computed_until)
        return cache[n];

    /* compute missing terms from computed_until+1 up to n */
    for (idx_t i = computed_until + 1; i <= n; ++i) {
        if (i <= 6) {
            cache[i] = (seq_t)1;
        } else if (i <= 15) {
            /* sum of previous 5 terms */
            seq_t sum = (seq_t)0;
            for (idx_t k = 1; k <= 5; ++k)
                sum += cache[i - k];

            /* product a(i‑1) * a(i‑5) */
            seq_t prod = cache[i - 1] * cache[i - 5];

            cache[i] = (sum + prod) / cache[i - 6];
        } else {
            /* linear recurrence for n > 15:
             * a(n) = 399*a(n-5) - 399*a(n-10) + a(n-15)
             * Since a(n-5) > a(n-10), the subtraction is non‑negative. */
            cache[i] = (seq_t)399 * cache[i - 5]
                       - (seq_t)399 * cache[i - 10]
                       + cache[i - 15];
        }
    }
    computed_until = n;
    return cache[n];
}
