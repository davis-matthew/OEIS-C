/*
 * A276682  Number of divisors of the n-th 9-gonal number.
 * https://oeis.org/A276682
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = d((7n^2-5n)/2) where d(x) is the divisor function
 * Range:     u8 n <= 6, u16 n <= 123, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   divisor_count_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/divisor_count_u64.h"

seq_t a(idx_t n) {
    /* compute the n-th 9-gonal number P9(n) = n*(7n-5)/2 */
    seq_t nseq = (seq_t)n;
    seq_t prod = nseq * ((seq_t)7 * nseq - 5);
    prod /= 2;                    /* division is exact */

    /* number of divisors of prod */
    seq_t result = (seq_t)divisor_count_u64((uint64_t)prod);
    return result;
}
