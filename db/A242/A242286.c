/*
 * A242286  Positive integers k for which the k-th triangular number is
 *          greedy-summable.
 * https://oeis.org/A242286
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) gives the n‑th positive integer k such that the k‑th
 *            triangular number T(k)=k(k+1)/2 can be written as a sum of
 *            distinct earlier triangular numbers by the greedy algorithm
 * Range:     u8 n <= 15, u16 n <= 352, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   greedy_summable
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/greedy_summable.h"

/* Output sequence a(n) --------------------------------------------------- */
seq_t a(idx_t n)
{
    /* Cached list of indices k that are greedy‑summable. */
    static seq_t *Vals = NULL;
    static size_t ValsCap = 0;     /* allocated capacity of Vals */
    static size_t ValsLen = 0;     /* number of valid entries */
    static idx_t NextK = 1;        /* next k to test for greedy_summability */

    if (n == 0)    /* safety: offset starts at 1 */
        return 0;

    /* Generate terms until we have at least n of them. */
    while (ValsLen < n) {
        if (greedy_summable(NextK)) {
            if (ValsLen == ValsCap) {
                ValsCap = (ValsCap == 0) ? 4 : ValsCap * 2;
                Vals = (seq_t *)realloc(Vals, ValsCap * sizeof(seq_t));
            }
            Vals[ValsLen++] = (seq_t)NextK;
        }
        ++NextK;
    }
    return Vals[n - 1];
}
