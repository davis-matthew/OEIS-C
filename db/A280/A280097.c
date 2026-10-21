/*
 * A280097  Sum of the divisors of 24*n - 1.
 * https://oeis.org/A280097
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 8, u16 n <= 55, u32 n <= 55, u64 n <= 55, u128 n <= 55,
 *            big any n
 * Helpers:   sum_of_divisors_24n_minus_1
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes a(n) = σ(24*n – 1), the sum of all positive divisors
   of 24*n – 1.  The implementation factorises the number by
   trial division.  All arithmetic on the running sum is done
   in uint64_t to avoid overflow, then the result is cast to
   seq_t at the end. */
#include <stdint.h>
#include <stdbool.h>
#include "../utils/sum_of_divisors_24n_minus_1.h"

seq_t a(idx_t n) {
    /* compute m = 24*n – 1 using seq_t arithmetic to avoid overflow */
    seq_t m = (seq_t)24 * n - 1;
    /* m is small enough to fit in uint64_t for all practical n. */
    uint64_t m64 = (uint64_t)m;
    uint64_t s64  = sum_of_divisors_24n_minus_1(m64);
    return (seq_t)s64;
}
