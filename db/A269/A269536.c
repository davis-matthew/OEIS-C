/*
 * A269536  Number of length-n 0..7 arrays with no repeated value differing
 *          from the previous repeated value by other than one.
 * https://oeis.org/A269536
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 31*a(n-1) - 355*a(n-2) + 1651*a(n-3) - 870*a(n-4) -
 *            13188*a(n-5) + 7883*a(n-6) + 59449*a(n-7) + 59366*a(n-8) +
 *            18088*a(n-9) Initial terms: a(1)=8, a(2)=64, a(3)=504,
 *            a(4)=3934, a(5)=30464, a(6)=234358, a(7)=1792788, a(8)=13648124,
 *            a(9)=103462888
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 10, u64 n <= 21, u128 n <= 44,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>

/* Compute the n-th term of A269536 (offset 1). */
seq_t a(idx_t n)
{
    static seq_t *cache     = NULL; /* cache[0] unused, cache[1].. cache[last] filled */
    static idx_t  last      = 0;    /* largest n with cache[n] valid */
    static size_t cap       = 0;    /* allocated size of cache */

    if (n < 1)                   /* below the offset, return 0 */
        return (seq_t)0;

    /* Initialise cache lazily with the first 9 known terms. */
    if (cache == NULL) {
        /* Allocate space for indices 0..9 (10 entries). */
        cap = 10;
        cache = (seq_t *)malloc(cap * sizeof(seq_t));
        if (!cache) return (seq_t)0;   /* allocation failure */

        cache[0] = (seq_t)0;
        cache[1] = (seq_t)8;
        cache[2] = (seq_t)64;
        cache[3] = (seq_t)504;
        cache[4] = (seq_t)3934;
        cache[5] = (seq_t)30464;
        cache[6] = (seq_t)234358;
        cache[7] = (seq_t)1792788;
        cache[8] = (seq_t)13648124;
        cache[9] = (seq_t)103462888;
        last = 9;
    }

    /* Grow the cache if necessary. */
    if ((size_t)n + 1 > cap) {
        size_t new_cap = (size_t)n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) return (seq_t)0;   /* reallocation failure */
        /* The newly allocated entries are uninitialized; we will fill them later. */
        cache = tmp;
        cap = new_cap;
    }

    /* Return early if already computed. */
    if (n <= last)
        return cache[n];

    /* Compute terms up to n using the recurrence. */
    for (idx_t i = last + 1; i <= n; ++i) {
        /* All arithmetic is done with seq_t, allowing big‑integer types. */
        seq_t val = (seq_t)31 * cache[i - 1];
        val -= (seq_t)355 * cache[i - 2];
        val += (seq_t)1651 * cache[i - 3];
        val -= (seq_t)870 * cache[i - 4];
        val -= (seq_t)13188 * cache[i - 5];
        val += (seq_t)7883 * cache[i - 6];
        val += (seq_t)59449 * cache[i - 7];
        val += (seq_t)59366 * cache[i - 8];
        val += (seq_t)18088 * cache[i - 9];
        cache[i] = val;
    }

    last = n;
    return cache[n];
}
