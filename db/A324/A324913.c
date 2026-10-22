/*
 * A324913  a(n) = Sum_{k=1..n} 2^k * phi(k), where phi is the Euler totient
 *          function A000010.
 * https://oeis.org/A324913
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=1..n} 2^k * phi(k)
 * Range:     u8 n <= 5, u16 n <= 12, u32 n <= 26, u64 n <= 58, u128 n <= 120,
 *            big any n
 * Helpers:   phi_simple
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/phi_simple.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0;                /* undefined below offset */
    seq_t sum   = 0;
    seq_t power = 2;                     /* 2^1 */

    for (idx_t k = 1; k <= n; ++k) {
        seq_t phi_k = phi_simple(k);     /* phi(k) */
        sum += power * phi_k;            /* add 2^k * phi(k) */
        power *= 2;                      /* next power of 2 */
    }
    return sum;
}
