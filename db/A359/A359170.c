/*
 * A359170  a(n) = 1 if n is not a multiple of 3 and has an even number of
 *          prime factors (with multiplicity), otherwise a(n) = 0.
 * https://oeis.org/A359170
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 1 if n mod 3 != 0 and has an even number of prime factors
 *            (counted with multiplicity); otherwise 0
 * Range:     u8 n <= 100000, u16 n <= 100000, u32 n <= 100000,
 *            u64 n <= 100000, u128 n <= 100000, big any n
 * Validated: standard, pass, 137 terms through n = 8444 (2026-11-01)
 * Helpers:   bigomega
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/bigomega.h"

seq_t a(idx_t n)
{
    if (n % 3 == 0)
        return 0;
    seq_t omega = bigomega(n);
    return (omega % 2 == 0) ? (seq_t)1 : (seq_t)0;
}
