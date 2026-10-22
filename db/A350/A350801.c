/*
 * A350801  a(n) = n*(tau(n) + 1) - 2*sigma(n) for n>=1, with a(0)=0.
 * https://oeis.org/A350801
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n*(tau(n)+1) - 2*sigma(n)
 * Range:     u8 n <= 47, u16 n <= 65, u32 n <= 65, u64 n <= 65, u128 n <= 65,
 *            big any n
 * Helpers:   divisor_count_u64, sigma_of
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/divisor_count_u64.h"
#include "../utils/sigma_of.h"

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;

    seq_t n_seq = (seq_t)n;
    seq_t tau = (seq_t)divisor_count_u64((uint64_t)n);
    seq_t sigma = sigma_of(n);
    return n_seq * (tau + 1) - 2 * sigma;
}
