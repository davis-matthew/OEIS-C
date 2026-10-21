/*
 * A296440  a(1) = 2, a(2) = a(3) = 3, a(4) = 4, a(5) = a(6) = 6; a(n) =
 *          a(n-a(n-1)) + a(n-a(n-2)) + a(n-a(n-3)) for n > 6.
 * https://oeis.org/A296440
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, look
 * Formula:   a(1) = 2, a(2)=a(3)=3, a(4)=4, a(5)=a(6)=6; a(n) = a(n-a(n-1)) +
 *            a(n-a(n-2)) + a(n-a(n-3)) for n > 6
 * Range:     u8 n <= 376, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;   /* memoisation array */
static idx_t  cap   = 0;      /* current capacity of cache */

/* initial known values */
static const seq_t init_vals[6] = {2, 3, 3, 4, 6, 6};

/* ensure cache can hold index n (1‑based) */
static void ensure_cap(idx_t n)
{
    if (n <= cap) return;
    idx_t new_cap = (cap > 0) ? cap * 2 : 8;
    while (new_cap < n) new_cap *= 2;
    seq_t *tmp = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
    if (!tmp) return;        /* allocation failed – leave cache unchanged */
    /* initialise newly allocated part to zero (un‑computed flag) */
    for (idx_t i = cap; i < new_cap; ++i) tmp[i] = 0;
    cache = tmp;
    cap   = new_cap;
}

seq_t a(idx_t n)
{
    /* base values for n = 1..6 */
    if (n <= 6) {
        /* store in cache if not already present */
        ensure_cap(n);
        if (!cache[n-1])
            cache[n-1] = init_vals[n-1];
        return cache[n-1];
    }

    /* ensure we have space to memoise this n */
    ensure_cap(n);
    if (cache[n-1])          /* already computed */
        return cache[n-1];

    /* recursive computation of the three required terms */
    seq_t t1 = a(n-1);
    seq_t t2 = a(n-2);
    seq_t t3 = a(n-3);

    idx_t idx1 = n - (idx_t)t1;
    idx_t idx2 = n - (idx_t)t2;
    idx_t idx3 = n - (idx_t)t3;

    seq_t result = a(idx1) + a(idx2) + a(idx3);
    cache[n-1] = result;
    return result;
}
