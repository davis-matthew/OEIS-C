/*
 * A358751  a(n) = 1 if bigomega(n) == 1 (mod 4), otherwise 0.
 * https://oeis.org/A358751
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 1 if bigomega(n) == 1 (mod 4), otherwise 0
 * Range:     u8 n <= 100000, u16 n <= 100000, u32 n <= 100000,
 *            u64 n <= 100000, u128 n <= 100000, big any n
 * Validated: standard, pass, 121 terms through n = 9497 (2026-11-01)
 * Helpers:   bigomega
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/bigomega.h"

seq_t a(idx_t n)
{
    /* bigomega counts prime factors with multiplicity */
    seq_t omega = bigomega(n);
    return ((omega % (seq_t)4) == (seq_t)1) ? 1 : 0;
}
