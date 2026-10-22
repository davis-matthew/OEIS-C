/*
 * A312980  Coordination sequence Gal.6.248.2 where Gal.u.t.v denotes the
 *          coordination sequence for a vertex of type v in tiling number t in
 *          the Galebach list of u-uniform tilings.
 * https://oeis.org/A312980
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a(n-1) + a(n-11) - a(n-12) for n > 12, a(0) = 1, a(1) =
 *            4, a(2) = 9, a(3) = 13, a(4) = 19, a(5) = 24, a(6) = 30, a(7) =
 *            35, a(8) = 41, a(9) = 45, a(10) = 50, a(11) = 54, a(12) = 58
 * Range:     u8 n <= 49, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

#ifdef __cplusplus
#include <vector>
#endif

/* One cache is shared by all calls; it grows on demand */
seq_t a(idx_t n)
{
#ifdef __cplusplus
    /* C++ implementation – use std::vector which handles construction
       and destruction of arbitrary-precision types correctly.              */
    static std::vector<seq_t> cache;

    /* Ensure we have room for n+1 elements; at least 13 for the base
       values.                                                       */
    size_t current = cache.size();
    size_t target  = (size_t)n + 1;
    if (target < 13) target = 13;

    if (current < target)
    {
        /* Construct the missing part of the cache. */
        cache.resize(target);

        /* Initialise the first 13 terms if they are not yet present. */
        if (current == 0)
        {
            const seq_t init[13] = { (seq_t)1, (seq_t)4, (seq_t)9, (seq_t)13,
                                     (seq_t)19, (seq_t)24, (seq_t)30, (seq_t)35,
                                     (seq_t)41, (seq_t)45, (seq_t)50, (seq_t)54,
                                     (seq_t)58 };
            for (size_t i = 0; i < 13; ++i) cache[i] = init[i];
        }
        else if (current < 13)
        {
            const seq_t init[13] = { (seq_t)1, (seq_t)4, (seq_t)9, (seq_t)13,
                                     (seq_t)19, (seq_t)24, (seq_t)30, (seq_t)35,
                                     (seq_t)41, (seq_t)45, (seq_t)50, (seq_t)54,
                                     (seq_t)58 };
            for (size_t i = current; i < 13; ++i) cache[i] = init[i];
        }

        /* Compute the missing recurrence terms. */
        for (size_t i = current; i < target; ++i)
        {
            if (i <= 12) continue;              /* already set */
            cache[i] = cache[i - 1] + cache[i - 11] - cache[i - 12];
        }
    }

    return cache[(size_t)n];
#else
    /* C implementation – use malloc/realloc.  This is safe for
       plain integer types; for arbitrary-precision types the C
       compilation path is not used.                               */
    static seq_t *cache = NULL;
    static size_t cap   = 0;

    /* Grow the cache if necessary. */
    if (n >= cap)
    {
        size_t newcap = (n < 13) ? 13 : ((size_t)n + 1);
        seq_t *tmp = (seq_t *)realloc(cache, newcap * sizeof(seq_t));
        if (!tmp) exit(EXIT_FAILURE);                     /* allocation failure */
        cache = tmp;

        /* Initialise for first use. */
        if (cap == 0)
        {
            const seq_t init[13] = { (seq_t)1, (seq_t)4, (seq_t)9, (seq_t)13,
                                     (seq_t)19, (seq_t)24, (seq_t)30, (seq_t)35,
                                     (seq_t)41, (seq_t)45, (seq_t)50, (seq_t)54,
                                     (seq_t)58 };
            for (size_t i = 0; i < 13; ++i) cache[i] = init[i];
        }

        /* Compute any additional terms needed. */
        for (size_t i = cap; i < newcap; ++i)
        {
            if (i <= 12) continue;               /* already set */
            cache[i] = cache[i - 1] + cache[i - 11] - cache[i - 12];
        }
        cap = newcap;
    }

    return cache[(size_t)n];
#endif
}
