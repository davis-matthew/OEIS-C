/*
 * A338522  Number of cyclic Latin squares of order n.
 * https://oeis.org/A338522
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = phi(n) * n! (OEIS A338522)
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 12, u64 n <= 19, u128 n <= 21,
 *            big any n
 * Helpers:   fact_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>  /* needed for uint64_t used in phi computation */
#include "../utils/fact_seq.h"

static uint64_t phi_uint64(uint64_t n)
{
    uint64_t result = n;
    for (uint64_t p = 2; p * p <= n; ++p) {
        if (n % p == 0) {
            while (n % p == 0) n /= p;
            result -= result / p;
        }
    }
    if (n > 1) result -= result / n;
    return result;
}

seq_t a(idx_t n)
{
    /* n >= 1 is guaranteed; 0th index is ignored. */
    seq_t f = fact_seq(n);
    seq_t phi = (seq_t)phi_uint64((uint64_t)n);
    return f * phi;
}
