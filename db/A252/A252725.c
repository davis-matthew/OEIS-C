/*
 * A252725  Number of (6+2) X (n+2) 0..3 arrays with every consecutive three
 *          elements in every row and diagonal having exactly two distinct
 *          values, and in every column and antidiagonal not having exactly
 *          two distinct values, and new values 0 upwards introduced in row
 *          major order.
 * https://oeis.org/A252725
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = -a(n-1) + 3*a(n-3) + 3*a(n-4) - 2*a(n-6) - 2*a(n-7) for n
 *            > 9 Initial values: a(1)=165, a(2)=154, a(3)=170, a(4)=178,
 *            a(5)=214, a(6)=262, a(7)=270, a(8)=354, a(9)=434
 * Range:     u8 n <= 5, u16 n <= 31, u32 n <= 79, u64 n <= 175, u128 n <= 210,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;     /* 1‑indexed cache (index 0 unused) */
static size_t cache_cap = 0;    /* highest index currently cached */

static void ensure_cache(idx_t n)
{
    if (n <= cache_cap)
        return;

    size_t new_cap = (size_t)n;
    seq_t *new_cache = NULL;

    if (cache == NULL) {
        new_cache = (seq_t *)malloc((new_cap + 1) * sizeof(seq_t));
    } else {
        new_cache = (seq_t *)realloc(cache, (new_cap + 1) * sizeof(seq_t));
    }
    if (!new_cache)
        abort();

    cache = new_cache;

    /* Base values for 1 ≤ n ≤ 9.  Index 0 is unused. */
    static const seq_t base[10] = {
        0,          /* dummy 0th element */
        165,        /* a(1) */
        154,        /* a(2) */
        170,        /* a(3) */
        178,        /* a(4) */
        214,        /* a(5) */
        262,        /* a(6) */
        270,        /* a(7) */
        354,        /* a(8) */
        434         /* a(9) */
    };

    for (size_t i = cache_cap + 1; i <= new_cap; ++i) {
        if (i <= 9) {
            cache[i] = base[(int)i];
        } else {
            /* Apply the recurrence using seq_t arithmetic. */
            seq_t v = -cache[i - 1];
            v += 3 * cache[i - 3];
            v += 3 * cache[i - 4];
            v -= 2 * cache[i - 6];
            v -= 2 * cache[i - 7];
            cache[i] = v;
        }
    }
    cache_cap = new_cap;
}

seq_t a(idx_t n)
{
    if (n < 1)
        return 0;          /* undefined for n < 1 */
    ensure_cache(n);
    return cache[(size_t)n];
}
