/*
 * A252723  Number of (4+2) X (n+2) 0..3 arrays with every consecutive three
 *          elements in every row and diagonal having exactly two distinct
 *          values, and in every column and antidiagonal not having exactly
 *          two distinct values, and new values 0 upwards introduced in row
 *          major order.
 * https://oeis.org/A252723
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = -a(n-1) + 3*a(n-3) + 3*a(n-4) - 2*a(n-6) - 2*a(n-7) for n
 *            > 8, with the first eight terms: a(1)=84, a(2)=76, a(3)=80,
 *            a(4)=97, a(5)=112, a(6)=132, a(7)=153, a(8)=196
 * Range:     u8 n <= 9, u16 n <= 34, u32 n <= 82, u64 n <= 178, u128 n <= 210,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>

/* Initial values for indices 1..8; index 0 is unused to keep 1‑based access. */
static const seq_t init_vals[9] = {0,
    84, 76, 80, 97, 112, 132, 153, 196};

seq_t a(idx_t n)
{
    /* Memoisation: cache the computed values. */
    static seq_t *cache = NULL;   /* 1‑based array of values, 0 entry unused */
    static idx_t cap = 0;         /* largest index currently cached */

    /* The sequence is defined for n >= 1.  For indices below that,
       return 0;  the caller must not use those values. */
    if (n == 0)
        return 0;

    /* If we have not computed up to n yet, grow the cache. */
    if (n > cap) {
        /* Allocate new storage: indices 0 .. n (size n+1). */
        seq_t *new_cache = (seq_t *)malloc((n + 1) * sizeof(seq_t));
        if (!new_cache) abort();   /* allocation failed */

        /* Copy any previously cached values. */
        if (cache) {
            memcpy(new_cache, cache, (cap + 1) * sizeof(seq_t));
            free(cache);
        }

        /* Compute the new entries. */
        for (idx_t i = (cap == 0 ? 1 : cap + 1); i <= n; ++i) {
            if (i <= 8) {
                new_cache[i] = init_vals[i];
            } else {
                /* Use a signed intermediate to avoid overflow when
                   seq_t is an unsigned integer type. */
                signed __int128 t =
                    -(signed __int128)new_cache[i - 1] +
                    3 * (signed __int128)new_cache[i - 3] +
                    3 * (signed __int128)new_cache[i - 4] -
                    2 * (signed __int128)new_cache[i - 6] -
                    2 * (signed __int128)new_cache[i - 7];
                new_cache[i] = (seq_t)t;
            }
        }

        /* Replace the old cache. */
        cache = new_cache;
        cap   = n;
    }

    return cache[n];
}
