/*
 * A352427  a(n) is the number of trailing 0's in the minimal representation
 *          of n in terms of the positive Pell numbers (A317204).
 * https://oeis.org/A352427
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = index of the smallest Pell number used in the minimal
 *            representation of n, minus 1
 * Range:     u8 n <= 994, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* minimal representation uses a greedy algorithm on Pell numbers:
   repeatedly subtract the largest Pell number ≤ remaining value.
   The last Pell number subtracted is the smallest used, so
   a(n) = smallest_index - 1. */

#include <stdlib.h>

/* static cache of Pell numbers: pell[0] is unused, pell[1] = 1, pell[2] = 2 */
static seq_t *pell = NULL;
static idx_t     pell_count = 0; /* number of entries in pell (highest index) */
static idx_t     pell_cap   = 0; /* allocated capacity of the pell array */

/* ensure that pell array contains all Pell numbers <= limit */
static void
ensure_pell(seq_t limit)
{
    /* initialise the array on first use */
    if (pell == NULL) {
        pell_cap   = 4;              /* initial capacity */
        pell       = (seq_t *)malloc(pell_cap * sizeof(seq_t));
        pell[0]    = 0;              /* dummy */
        pell[1]    = 1;
        pell[2]    = 2;
        pell_count = 2;
    }

    /* extend the array until pell[pell_count] > limit */
    while (pell[pell_count] <= limit) {
        /* grow capacity if needed */
        if (pell_count == pell_cap - 1) {
            pell_cap *= 2;
            pell = (seq_t *)realloc(pell, pell_cap * sizeof(seq_t));
        }
        /* next Pell number: P_n = 2*P_{n-1} + P_{n-2} */
        seq_t next = (seq_t)(2 * pell[pell_count] + pell[pell_count - 1]);
        ++pell_count;
        pell[pell_count] = next;
    }
}

/* main sequence function */
seq_t a(idx_t n)
{
    if (n < 1)
        return 0;
    seq_t rem = (seq_t)n;
    ensure_pell(rem);
    idx_t last_index = 0;

    while (rem > 0) {
        idx_t lo = 1, hi = pell_count;
        /* binary search for largest Pell <= rem */
        while (lo <= hi) {
            idx_t mid = (lo + hi) >> 1;
            if (pell[mid] <= rem)
                lo = mid + 1;
            else
                hi = mid - 1;
        }
        idx_t idx = hi;        /* idx is the largest index with pell[idx] <= rem */
        rem -= pell[idx];
        last_index = idx;      /* keep track of the smallest index used */
    }

    return (seq_t)(last_index - 1);
}
