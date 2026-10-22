/*
 * A301837  Number of nX4 0..1 arrays with every element equal to 0, 1 or 2
 *          horizontally or antidiagonally adjacent elements, with upper left
 *          element zero.
 * https://oeis.org/A301837
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 10, u64 n <= 20, u128 n <= 41,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(1) = 8, a(2) = 81, a(3) = 678, a(4) = 5748, a(5) = 48802,
   a(6) = 414385;
   a(n) = 13*a(n-1) - 46*a(n-2) + 72*a(n-3) - 57*a(n-4) + 16*a(n-5)  for n > 6 */
#include <stdlib.h>

seq_t a(idx_t n)
{
    if (n == 0) /* undefined, but harmless */
        return (seq_t)0;

    /* one‑time local cache, indices start at 1 */
    static seq_t *cache   = NULL;
    static size_t cache_cap = 0;   /* allocated length (array size) */
    static size_t cache_len = 0;   /* largest n computed so far (inclusive) */

    /* Ensure we have space for indices up to n (index 0 unused). */
    size_t required = (size_t)n + 1;        /* we need slots 0..n */
    if (required > cache_cap) {
        size_t new_cap = cache_cap ? cache_cap : 8;
        while (new_cap < required)
            new_cap <<= 1;                /* double until sufficient */
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp)                         /* allocation failure aborts */
            abort();
        cache = tmp;
        cache_cap = new_cap;
    }

    /* Compute values up to n if not already present. */
    for (size_t i = cache_len + 1; i <= (size_t)n; ++i) {
        if (i <= 6) {
            static const seq_t init[6] = {8, 81, 678, 5748, 48802, 414385};
            cache[i] = init[i - 1];
        } else {
            seq_t term1 = 13 * cache[i - 1];
            seq_t term2 = 46 * cache[i - 2];
            seq_t term3 = 72 * cache[i - 3];
            seq_t term4 = 57 * cache[i - 4];
            seq_t term5 = 16 * cache[i - 5];

            cache[i] = term1 - term2 + term3 - term4 + term5;
        }
    }
    cache_len = (size_t)n;

    return cache[n];
}
