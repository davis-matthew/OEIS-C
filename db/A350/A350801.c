/*
 * A350801  a(n) = n*(tau(n) + 1) - 2*sigma(n) for n>=1, with a(0)=0.
 * https://oeis.org/A350801
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n*(tau(n)+1) - 2*sigma(n)
 * Range:     u8 n <= 47, u16 n <= 65, u32 n <= 65, u64 n <= 65, u128 n <= 65,
 *            big any n
 * Validated: partial, pass, 66 terms through n = 65 (2026-11-01)
 * Helpers:   num_divisors
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include "../utils/num_divisors.h"

/* Compute σ(i) by trial division up to √i. */
static seq_t sigma_of(idx_t n)
{
    seq_t sum = (seq_t)0;
    for (idx_t d = 1; d * d <= n; ++d) {
        if (n % d == 0) {
            sum += (seq_t)d;
            idx_t d2 = n / d;
            if (d2 != d)
                sum += (seq_t)d2;
        }
    }
    return sum;
}

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;

    seq_t n_seq = (seq_t)n;
    seq_t tau = (seq_t)num_divisors((uint64_t)n);
    seq_t sigma = sigma_of(n);
    return n_seq * (tau + 1) - 2 * sigma;
}
