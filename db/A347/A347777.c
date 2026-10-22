/*
 * A347777  Number of compositions (ordered partitions) of n into at most 2
 *          squarefree parts.
 * https://oeis.org/A347777
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = [n is squarefree] + Σ_{i=1}^{n-1} [i squarefree]·[n-i
 *            squarefree] (a(0) = 1)
 * Range:     u8 n <= 70, u16 n <= 70, u32 n <= 70, u64 n <= 70, u128 n <= 70,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    /* static buffers for memoisation */
    static uint8_t *sqfree = NULL;      /* 1 if the index is square‑free, else 0 */
    static seq_t  *cache  = NULL;      /* cached a(n) values      */
    static size_t  cap    = 0;          /* current capacity       */

    /* trivial case */
    if (n == 0) return (seq_t)1;

    /* if the requested index is outside cached range, extend the tables   */
    if ((size_t)n >= cap) {
        size_t newCap = (size_t)n + 1;

        /* enlarge buffers */
        sqfree = (uint8_t*)realloc(sqfree, newCap * sizeof(uint8_t));
        cache  = (seq_t *)realloc(cache,  newCap * sizeof(seq_t));

        /* initialise all entries as square‑free */
        for (size_t i = 0; i < newCap; ++i)
            sqfree[i] = 1;
        if (newCap > 0) sqfree[0] = 0;   /* 0 is not square‑free */

        /* mark multiples of perfect squares as non‑square‑free */
        for (idx_t p = 2; (p * p) < (idx_t)newCap; ++p) {
            idx_t sq = p * p;
            for (idx_t j = sq; j < (idx_t)newCap; j += sq)
                sqfree[j] = 0;
        }

        /* compute a(m) for all m < newCap */
        cache[0] = (seq_t)1;
        for (idx_t m = 1; m < (idx_t)newCap; ++m) {
            seq_t count = 0;
            for (idx_t i = 1; i < m; ++i)
                if (sqfree[i] && sqfree[m - i])
                    ++count;
            if (sqfree[m])
                ++count;
            cache[m] = count;
        }

        cap = newCap;
    }

    return cache[n];
}
