/*
 * A343945  a(1) = 1; for n >= 2, a(n) = floor(log(sigma(n)) / log(tau(n))).
 * https://oeis.org/A343945
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = floor(log(sigma(n))/log(tau(n)))
 * Range:     u8 n <= 29, u16 n <= 87, u32 n <= 87, u64 n <= 87, u128 n <= 87,
 *            big any n
 * Helpers:   divisor_count_u64, seq_mul, sigma_of
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implementation uses integer comparison: a(n)=max{k | tau(n)^k <= sigma(n)} */

#include <stdint.h>
#include "../utils/sigma_of.h"
#include "../utils/divisor_count_u64.h"
#include "../utils/seq_mul.h"

/* External linkage as required */
seq_t a(idx_t n)
{
    /* Handle the special case n = 1: sigma(1)=1, tau(1)=1, defined as 1 */
    if (n == 1)
        return (seq_t)1;

    /* Compute sigma(n) as seq_t and tau(n) as uint64_t (fits in 64 bits) */
    seq_t sigma = sigma_of(n);
    uint64_t tau  = divisor_count_u64((uint64_t)n);

    /* Should never happen for n>1, but guard against tau==1 to avoid infinite loop */
    if (tau == 1)
        return (seq_t)1;

    seq_t power = (seq_t)1;  /* tau^0 = 1 */
    seq_t result = (seq_t)0;
    seq_t next;

    /* Increment power until it would exceed sigma */
    while (1) {
        next = seq_mul(power, (seq_t)tau);
        if (next <= sigma) {
            power = next;
            result++;
        } else {
            break;
        }
    }

    return result;
}
