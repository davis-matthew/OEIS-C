/*
 * A293817  Numbers k such that m=2*k is the middle side in a Heronian
 *          triangle with sides m-13, m , m+13.
 * https://oeis.org/A293817
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 4*a(n-3)-a(n-6), a(0)=13, a(1)=14, a(2)=19, a(3)=26,
 *            a(4)=37, a(5)=62
 * Range:     u8 n <= 8, u16 n <= 20, u32 n <= 46, u64 n <= 96, u128 n <= 197,
 *            big any n
 * Helpers:   grow_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>
#include "../utils/grow_seq.h"

/* Compute a(n) for OEIS sequence A293817.
   Uses the recurrence a(n) = 4*a(n-3) - a(n-6) with
   initial values a(0)=13, a(1)=14, a(2)=19, a(3)=26, a(4)=37, a(5)=62.
   Caches computed values for fast arbitrary‑order access. */
seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static size_t cache_cap = 0; /* allocated capacity */
    static size_t cache_len = 0; /* number of filled entries */

    if (n < cache_len)
        return cache[n];

    /* Ensure space for all entries up to n */
    grow_seq(&cache, &cache_cap, (size_t)n + 1);

    /* Compute missing values from the first missing index up to n */
    for (idx_t i = cache_len; i <= n; ++i)
    {
        if (i == 0)       cache[i] = 13;
        else if (i == 1)  cache[i] = 14;
        else if (i == 2)  cache[i] = 19;
        else if (i == 3)  cache[i] = 26;
        else if (i == 4)  cache[i] = 37;
        else if (i == 5)  cache[i] = 62;
        else
            cache[i] = 4 * cache[i - 3] - cache[i - 6];
    }

    cache_len = (size_t)n + 1;
    return cache[n];
}
