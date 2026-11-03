/*
 * A328958  a(n) = d(n) - (omega(n) * bigomega(n)), where d (number of
 *          divisors) = A000005, omega = A001221, bigomega = A001222.
 * https://oeis.org/A328958
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = d(n) - omega(n) * bigomega(n) where d(n) = number of
 *            divisors (A000005), omega(n) = number of distinct prime factors
 *            (A001221), bigomega(n) = total number of prime factors counted
 *            with multiplicity (A001222)
 * Range:     i8 n <= 20000, i16 n <= 20000, i32 n <= 20000, i64 n <= 20000,
 *            i128 n <= 20000, big any n
 * Validated: standard, pass, 121 terms through n = 9203 (2026-11-01)
 * Helpers:   bigomega, num_divisors, omega
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/bigomega.h"
#include "../utils/num_divisors.h"
#include "../utils/omega.h"

seq_t a(idx_t n)
{
    uint64_t nn = (uint64_t)n;            /* safe cast for small indices */
    seq_t divisors   = num_divisors(nn);
    seq_t distinct   = (seq_t)omega(nn);
    seq_t total_factors = bigomega(nn);
    return divisors - distinct * total_factors;
}
