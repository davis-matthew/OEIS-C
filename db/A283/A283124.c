/*
 * A283124  Number of nX2 0..1 arrays with no 1 equal to more than two of its
 *          horizontal and vertical neighbors.
 * https://oeis.org/A283124
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 3*a(n-1) + 2*a(n-2) + 2*a(n-3) - a(n-4) - a(n-5) with
 *            a(1)=4, a(2)=16, a(3)=57, a(4)=209, a(5)=768
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 16, u64 n <= 34, u128 n <= 68,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;
static idx_t cap = 0;
static idx_t last_computed = 0;

static void ensure_cache(idx_t n)
{
    idx_t needed = n + 1;               /* we use index 0 as unused */
    if (cap >= needed)
        return;
    idx_t newcap = cap ? cap * 2 : 5;    /* start with 5 for the first 5 terms */
    if (newcap < needed)
        newcap = needed;
    seq_t *newptr = (seq_t *)realloc(cache, sizeof(seq_t) * newcap);
    if (!newptr)
        abort();                         /* memory allocation failure */
    cache = newptr;
    for (idx_t i = cap; i < newcap; ++i)
        cache[i] = 0;
    cap = newcap;
}

seq_t a(idx_t n)
{
    if (n < 1)
        return 0;                       /* values below offset are undefined */

    /* initialise cache on first call */
    if (!cache)
    {
        cache = (seq_t *)malloc(sizeof(seq_t) * 6); /* indices 0..5 */
        if (!cache)
            abort();
        cap = 6;
        cache[0] = 0;
        cache[1] = (seq_t)4;
        cache[2] = (seq_t)16;
        cache[3] = (seq_t)57;
        cache[4] = (seq_t)209;
        cache[5] = (seq_t)768;
        last_computed = 5;
    }

    if (n <= last_computed)
        return cache[n];

    ensure_cache(n);

    for (idx_t i = last_computed + 1; i <= n; ++i)
    {
        cache[i] = (seq_t)3 * cache[i - 1]
                   + (seq_t)2 * cache[i - 2]
                   + (seq_t)2 * cache[i - 3]
                   - cache[i - 4]
                   - cache[i - 5];
    }
    last_computed = n;
    return cache[n];
}
