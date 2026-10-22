/*
 * A360443  Smallest integer m > n such that the multiset of nonzero decimal
 *          digits of m is exactly the same as the multiset of nonzero decimal
 *          digits of n.
 * https://oeis.org/A360443
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, base
 * Formula:   a(n) = the smallest integer > n whose multiset of nonzero
 *            decimal * digits equals that of n (allowing any number of
 *            leading zeros)
 * Range:     u8 n <= 29, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   same_nonzero_counts
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/same_nonzero_counts.h"

seq_t a(idx_t n) {
    /* compute non‑zero digit counts of n */
    int ncnt[10] = {0};
    seq_t tn = (seq_t)n;
    seq_t tmp = tn;
    while (tmp > 0) {
        int d = (int)(tmp % 10);
        if (d != 0) ncnt[d]++;
        tmp /= 10;
    }

    seq_t m = tn + 1;           /* start searching from the next integer */
    while (true) {
        if (same_nonzero_counts(m, ncnt)) return m;
        m++;                      /* increment candidate */
    }
}
