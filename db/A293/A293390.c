/*
 * A293390  Least m such that the exponents in expression for n as a sum of
 *          distinct powers of 2 are pairwise distinct mod m; a(0) = 0 by
 *          convention.
 * https://oeis.org/A293390
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A293390 – Least m such that the exponents of the binary
 *  representation of n are pairwise distinct modulo m.
 *
 *  a(0) = 0
 *  a(n) = 1            if n is a power of 2 (or n == 1)
 *  a(n) = least positive m for which no difference of the
 *         exponents is divisible by m.
 *
 *  The implementation gathers the set of exponents of 1‑bits,
 *  computes all pairwise differences, and then searches m
 *  from 1 upward until it finds one that divides none of those
 *  differences.  For 64‑bit input the search stops at
 *  maxDiff+1 <= 65, so the routine is O(1) and very fast.
 */

#include <stddef.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    /* Special case for the convention a(0)=0. */
    if (n == 0)
        return (seq_t)0;

    /* Collect the exponents of the set bits of n.
     * At most 64 bits for an idx_t. */
    idx_t exps[64];
    idx_t k = 0;
    idx_t pos = 0;
    idx_t tmp = n;
    while (tmp != 0) {
        if (tmp & (idx_t)1)
            exps[k++] = pos;
        pos++;
        tmp >>= 1;
    }

    /* For n that is a power of 2 (k==1) the answer is 1. */
    if (k <= 1)
        return (seq_t)1;

    /* Compute all pairwise differences of the exponents. */
    idx_t diffs[2016];           /* 64*63/2 = 2016 */
    idx_t dcnt = 0;
    idx_t maxDiff = 0;

    for (idx_t i = 0; i < k; ++i) {
        for (idx_t j = i + 1; j < k; ++j) {
            idx_t d = exps[j] - exps[i];
            diffs[dcnt++] = d;
            if (d > maxDiff)
                maxDiff = d;
        }
    }

    /* Search for the smallest m that divides none of the differences. */
    for (idx_t m = 1; m <= maxDiff + 1; ++m) {
        bool ok = true;
        for (idx_t i = 0; i < dcnt; ++i) {
            if (diffs[i] % m == 0) {
                ok = false;
                break;
            }
        }
        if (ok)
            return (seq_t)m;
    }

    /* The search must always succeed; this line is unreachable. */
    return (seq_t)(maxDiff + 1);
}
