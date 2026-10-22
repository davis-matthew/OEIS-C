/*
 * A340369  a(n) = 1 if n has at most 3 prime factors when counted with
 *          multiplicity, 0 otherwise.
 * https://oeis.org/A340369
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = [A001222(n) <= 3]
 * Range:     u8 n <= 9985, u16 n <= 9985, u32 n <= 9985, u64 n <= 9985,
 *            u128 n <= 9985, big any n
 * Helpers:   bigomega
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/bigomega.h"

seq_t a(idx_t n)
{
    return (bigomega(n) <= 3) ? 1 : 0;
}
