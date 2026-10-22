/*
 * A394841  A variant of Wallis sieve, read by antidiagonals.
 * https://oeis.org/A394841
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = 1 if the (i,j) cell read by antidiagonal enumeration is
 *            not a central square at any level of the recursive construction;
 *            a(n) = 0 otherwise. This implements OEIS A394841
 * Range:     u8 n <= 8368, u16 n <= 8368, u32 n <= 8368, u64 n <= 8368,
 *            u128 n <= 8368, big any n
 * Helpers:   _coords_from_antidiagonal, _value_at
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include "../utils/_coords_from_antidiagonal.h"
#include "../utils/_value_at.h"

/* Public function required by the build system. */
seq_t a(idx_t n)
{
    idx_t i, j;
    _coords_from_antidiagonal(n, &i, &j);
    return _value_at(i, j);
}
