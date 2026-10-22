/*
 * A328668  Numbers whose binary expansion, without the most significant
 *          digit, is a necklace.
 * https://oeis.org/A328668
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n-th integer whose binary expansion, without the most
 *            significant digit, is a necklace. Here a(n) is indexed starting
 *            from 0
 * Range:     u8 n <= 58, u16 n <= 60, u32 n <= 60, u64 n <= 60, u128 n <= 60,
 *            big any n
 * Helpers:   bitlen_seq_t
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include "../utils/bitlen_seq_t.h"

static inline bool suffix_is_necklace(seq_t m)
{
    /* Determine whether the binary suffix of m (obtained by
       deleting the most significant bit) is lexicographically
       minimal among all its cyclic rotations. */
    unsigned int T = bitlen_seq_t(m);           /* total bits of m, 0 for m==0 */
    if (T <= 1)            /* 0 or 1 has empty suffix, considered a necklace */
        return true;

    unsigned int L = T - 1;                       /* length of suffix */
    seq_t msb = ((seq_t)1) << (T - 1);            /* integer with only the msb set */
    seq_t suffix = m - msb;                       /* remove the msb */

    seq_t mask = ((seq_t)1 << L) - 1;             /* keep only L bits during rotations */
    seq_t min_val = suffix;                       /* minimal rotation seen so far */

    for (unsigned int r = 1; r < L; ++r) {
        seq_t rot = ((suffix << r) | (suffix >> (L - r))) & mask;
        if (rot < min_val)
            min_val = rot;
    }
    return min_val == suffix;
}

seq_t a(idx_t n)
{
    /* Sequentially scan the non‑negative integers and count those
       for which suffix_is_necklace returns true.  When the count
       equals the requested index n, return that integer. */
    idx_t seen = 0;
    for (seq_t x = 0;; ++x) {
        if (suffix_is_necklace(x)) {
            if (seen == n)
                return x;
            ++seen;
        }
    }
}
