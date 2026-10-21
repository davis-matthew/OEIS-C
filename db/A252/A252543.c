/*
 * A252543  Number of (n+2) X (1+2) 0..3 arrays with every 3 X 3 subblock row
 *          and diagonal sum equal to 1 3 4 6 or 7 and every 3 X 3 column and
 *          antidiagonal sum not equal to 1 3 4 6 or 7.
 * https://oeis.org/A252543
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 18*a(n-3) - 32*a(n-6) for n>8 with initial values
 *            a(1)=478, a(2)=850, a(3)=2142, a(4)=4644, a(5)=13104,
 *            a(6)=33852, a(7)=73800, a(8)=208992
 * Range:     u8 none, u16 n <= 6, u32 n <= 18, u64 n <= 42, u128 n <= 90,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

seq_t a(idx_t n)
{
    if (n == 0) return 0;

    /* static cache for previously computed terms */
    static seq_t *cache      = NULL;
    static size_t cap        = 0;   /* current allocated capacity */
    static size_t computed   = 0;   /* number of terms stored (most recent index + 1) */

    /* Ensure the cache can hold n terms (index 0 corresponds to a(1)). */
    if (cap < (size_t)n) {
        size_t newcap = cap ? (cap << 1) : 8;
        if (newcap < (size_t)n) newcap = n;
        seq_t *tmp = (seq_t *)realloc(cache, newcap * sizeof(seq_t));
        if (!tmp) return 0;          /* allocation failed */
        cache = tmp;
        cap   = newcap;
    }

    /* Base values for the first eight terms. */
    static const seq_t base[8] = {
        (seq_t)478,  (seq_t)850,  (seq_t)2142,
        (seq_t)4644, (seq_t)13104, (seq_t)33852,
        (seq_t)73800, (seq_t)208992
    };

    /* Compute all missing terms up to n. */
    for (size_t i = computed; i < (size_t)n; ++i) {
        if (i < 8) {
            cache[i] = base[i];
        } else {
            cache[i] = (seq_t)18 * cache[i - 3] - (seq_t)32 * cache[i - 6];
        }
    }
    computed = (size_t)n;

    return cache[(size_t)n - 1];
}
