/*
 * A307609  Number of partitions of n^3 into consecutive positive cubes.
 * https://oeis.org/A307609
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 22, u32 n <= 1003, u64 n <= 9492,
 *            u128 n <= 9492, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Algorithm: For each n, count the number of ways to write n^3 as a
   sum of consecutive positive cubes.  For a given ending index j
   (the largest cube in the sum) we need
   P[j] - P[i-1] = n^3   where   P[k] = (k(k+1)/2)^2
   is the sum of cubes from 1 to k.  Thus we look for a pair (i,j)
   with i-1<j and P[i-1] = P[j]-n^3.  Because j^3 ≤ n^3 we only
   need to consider j ≤ n.  All required P[k] values are stored
   in a growing static array so that repeated calls are fast. */

#include <stdlib.h>

seq_t a(idx_t n) {
    /* static table of cumulative cube sums P[k] = (k(k+1)/2)^2 */
    static seq_t *pref = NULL;
    static size_t pref_cap = 0;          /* current capacity of pref array */

    /* ensure pref contains values up to k = n */
    if ((size_t)n + 1 > pref_cap) {
        size_t new_cap = (size_t)n + 1;
        seq_t *tmp = (seq_t *)realloc(pref, new_cap * sizeof(seq_t));
        if (!tmp) {                      /* allocation failed */
            if (pref) free(pref);
            pref = NULL;
            pref_cap = 0;
            return 0;                    /* cannot compute */
        }
        pref = tmp;
        /* compute new entries */
        for (idx_t k = (idx_t)pref_cap; k <= n; ++k) {
            seq_t t = (seq_t)k * ((seq_t)k + 1);  /* k(k+1) */
            t /= 2;                              /* triangular number */
            t *= t;                              /* square of it */
            pref[(size_t)k] = t;
        }
        pref_cap = new_cap;
    }

    /* n^3 as seq_t */
    seq_t sn = (seq_t)n;
    seq_t n_sq = sn * sn;
    seq_t n_cu = n_sq * sn;

    seq_t count = 0;

    /* iterate over possible last cube index j */
    for (idx_t j = 1; j <= n; ++j) {
        if (pref[(size_t)j] < n_cu)   /* insufficient sum, skip */
            continue;

        seq_t target = pref[(size_t)j] - n_cu;   /* P[j] - n^3 */

        /* binary search pref[0 .. j-1] for target */
        size_t lo = 0;
        size_t hi = (size_t)j;           /* exclusive upper bound */
        while (lo < hi) {
            size_t mid = (lo + hi) >> 1;
            if (pref[mid] < target)
                lo = mid + 1;
            else
                hi = mid;
        }
        if (lo < (size_t)j && pref[lo] == target)
            ++count;                    /* found a valid (i,j) pair */
    }

    return count;
}
