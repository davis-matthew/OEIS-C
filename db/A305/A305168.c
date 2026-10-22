/*
 * A305168  Number of non-isomorphic graphs on 4n vertices whose edges are the
 *          union of two n-edge matchings.
 * https://oeis.org/A305168
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A002513(2n) = Sum_{i=0..n} b(2i)*b(n-i) where b(k) is the
 *            number of partitions of k. The function computes the partition
 *            numbers using Euler's pentagonal theorem (O(n^1.5) time)
 * Range:     u8 n <= 6, u16 n <= 15, u32 n <= 34, u64 n <= 34, u128 n <= 34,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

/* External types are supplied by the build system. */
extern idx_t _floor_isqrt_u64(idx_t n);   /* may be used if desired */

/* The function a is defined with external linkage. */
seq_t a(idx_t n)
{
    /* Cache of partition numbers p(0..2n).  The cache grows on demand
       and retains its values across calls. */
    static seq_t *p = NULL;      /* partition array */
    static size_t cap = 0;       /* allocated size, number of elements */
    static idx_t max_comp = 0;   /* largest m for which p[m] is computed */

    /* We need partition numbers up to m = 2*n. */
    idx_t limit = 2 * n;

    /* Ensure array is large enough. */
    if (limit + 1 > cap) {
        size_t new_cap = (size_t)(limit + 1);
        seq_t *tmp = (seq_t *)realloc(p, new_cap * sizeof(seq_t));
        if (!tmp) {
            /* Realloc failed – aborting is acceptable for the test harness. */
            exit(1);
        }
        p = tmp;
        cap = new_cap;
    }

    /* Initialize the base case p[0] = 1 once. */
    if (max_comp == 0 && (limit >= 0)) {
        p[0] = (seq_t)1;
        max_comp = 0;
    }

    /* Compute missing partition numbers using Euler's pentagonal
       number theorem.  The recurrence is
           p(m) = sum_{k=1..} (-1)^{k-1} [p(m - k(3k-1)/2) + p(m - k(3k+1)/2)]
       with terms dropped when the index is negative. */
    if (limit > max_comp) {
        for (idx_t m = max_comp + 1; m <= limit; ++m) {
            seq_t sum = 0;
            /* The pentagonal numbers grow as k*(3k±1)/2. */
            for (int k = 1; ; ++k) {
                /* type cast to avoid overflow on 32‑bit systems */
                idx_t pent1 = (idx_t)((long long)k * (3LL * k - 1LL) / 2);
                idx_t pent2 = (idx_t)((long long)k * (3LL * k + 1LL) / 2);
                if (pent1 > m && pent2 > m)
                    break;

                if (pent1 <= m) {
                    if (k & 1)
                        sum += p[m - pent1];   /* + for odd k */
                    else
                        sum -= p[m - pent1];   /* - for even k */
                }
                if (pent2 <= m) {
                    if (k & 1)
                        sum += p[m - pent2];
                    else
                        sum -= p[m - pent2];
                }
            }
            p[m] = sum;
        }
        max_comp = limit;
    }

    /* Compute a(n) = Sum_{i=0..n} p(2i) * p(n-i). */
    seq_t result = 0;
    for (idx_t i = 0; i <= n; ++i) {
        result += p[2 * i] * p[n - i];
    }
    return result;
}
