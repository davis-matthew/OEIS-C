/*
 * A301593  n can be represented the sum of a(n) distinct factorials. (If
 *          there is no such representation, a(n) = 0.)
 * https://oeis.org/A301593
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 0 if n cannot be expressed as a sum of distinct
 *            factorials, otherwise a(n) = the minimal number of distinct
 *            factorials that sum to n
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    /* Build a list of factorials <= n.  We use idx_t (uint64_t) for the
       computations to avoid overflow of seq_t which may be as small as
       uint8_t.  The list is stored in a fixed array because the number of
       factorials that fit in any reasonable n is very small. */
    idx_t facts[64];
    idx_t k = 0;                 /* number of factorials found */

    /* Compute factorials iteratively, guarding against overflow. */
    idx_t f = 1;                 /* (i-1)! before the iteration */
    idx_t i = 1;                 /* current factorial index (i!)  */
    while (1) {
        if (f > n) break;        /* current factorial already exceeds n */
        facts[k++] = f;          /* store i! */
        if (k == 64) break;      /* safety guard */

        /* Prepare next factorial: f = i! * (i+1) = (i+1)! */
        ++i;
        if (f > n / i) break;    /* would overflow or exceed n */
        f *= i;
    }

    /* n >= 1 guarantees at least the 1! factorial was stored. */
    if (k == 0) return (seq_t)0;

    /* Search all non‑empty subsets of the factorial list to find the
       one with minimal cardinality that sums exactly to n.  Since k is
       tiny (≤ 20 for 64‑bit n), an exhaustive search is trivial. */
    idx_t best = (idx_t)(-1);     /* sentinel for “no representation yet” */

    for (uint64_t mask = 1; mask < (1ULL << k); ++mask) {
        idx_t sum = 0;
        idx_t cnt = 0;
        for (idx_t j = 0; j < k; ++j) {
            if (mask & (1ULL << j)) {
                sum += facts[j];
                ++cnt;
                if (sum > n) break;      /* prune this subset */
            }
        }
        if (sum == n && (best == (idx_t)(-1) || cnt < best))
            best = cnt;
        if (best == 1) break;            /* cannot beat a single factorial */
    }

    return (seq_t)(best == (idx_t)(-1) ? 0 : best);
}
