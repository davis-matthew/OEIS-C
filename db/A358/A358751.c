/*
 * A358751  a(n) = 1 if bigomega(n) == 1 (mod 4), otherwise 0.
 * https://oeis.org/A358751
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 1 if bigomega(n) == 1 (mod 4), otherwise 0
 * Range:     u8 n <= 9497, u16 n <= 9497, u32 n <= 9497, u64 n <= 9497,
 *            u128 n <= 9497, big any n
 * Helpers:   bigomega
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/bigomega.h"

seq_t a(idx_t n)
{
    /* bigomega counts prime factors with multiplicity */
    seq_t omega = bigomega(n);
    return ((omega % (seq_t)4) == (seq_t)1) ? 1 : 0;
}
