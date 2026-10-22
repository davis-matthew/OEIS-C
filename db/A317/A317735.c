/*
 * A317735  Number of nX2 0..1 arrays with every element unequal to 2 or 3
 *          horizontally, vertically or antidiagonally adjacent elements, with
 *          upper left element zero.
 * https://oeis.org/A317735
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1)=0, a(2)=1, a(3)=1, a(4)=2, a(5)=4, a(6)=9; recurrence: a(n)
 *            = 2*a(n-1)+a(n-2)+a(n-3)-a(n-4)-a(n-5)-a(n-6) (n>=7)
 * Range:     u8 n <= 9, u16 n <= 15, u32 n <= 28, u64 n <= 52, u128 n <= 101,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>

static seq_t *cache = NULL;          /* array of computed a(i) values, 1‑based */
static idx_t cached_until = 0;       /* largest n that has been cached */
static idx_t cache_cap    = 0;       /* current allocation size of cache */

static void ensure_cache(idx_t n)
{
    if (n <= cache_cap)
        return;

    idx_t new_cap = n;
#ifdef __cplusplus
    seq_t *new_cache = new seq_t[new_cap];
    if (cache) {
        for (idx_t i = 0; i < cached_until; ++i)
            new_cache[i] = cache[i];
        delete[] cache;
    }
    cache = new_cache;
    cache_cap = new_cap;
#else
    seq_t *new_cache = realloc(cache, new_cap * sizeof(seq_t));
    if (!new_cache)
        abort();
    cache = new_cache;
    cache_cap = new_cap;
#endif
}

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0; /* undefined for n<1, return 0 */

    if (n <= cached_until)
        return cache[n-1];

    ensure_cache(n);

    for (idx_t k = cached_until + 1; k <= n; ++k) {
        if (k == 1) {
            cache[0] = (seq_t)0;
        } else if (k == 2) {
            cache[1] = (seq_t)1;
        } else if (k == 3) {
            cache[2] = (seq_t)1;
        } else if (k == 4) {
            cache[3] = (seq_t)2;
        } else if (k == 5) {
            cache[4] = (seq_t)4;
        } else if (k == 6) {
            cache[5] = (seq_t)9;
        } else {
            /* recurrence for n >= 7 */
            cache[k-1] = 2 * cache[k-2] + cache[k-3] + cache[k-4]
                          - cache[k-5] - cache[k-6] - cache[k-7];
        }
    }

    cached_until = n;
    return cache[n-1];
}
