/*
 * A372715  a(n) = tau(3*n-2) = A000005(3*n-2).
 * https://oeis.org/A372715
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = tau(3*n - 2)
 * Range:     u8 n <= 86, u16 n <= 86, u32 n <= 86, u64 n <= 86, u128 n <= 86,
 *            big any n
 * Helpers:   divisor_count_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the number of positive divisors of 3*n-2. */
#include <stdint.h>
#include "../utils/divisor_count_u64.h"

seq_t a(idx_t n)
{
    /* 3*n - 2 fits easily in 64 bits for all n of interest. */
    uint64_t v = (uint64_t)3 * n - 2ULL;
    return divisor_count_u64(v);
}
