/*
 * A344457  a(n) = Sum_{d|n} d * Omega(d).
 * https://oeis.org/A344457
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{d|n} d * Omega(d)
 * Range:     u8 n <= 31, u16 n <= 64, u32 n <= 64, u64 n <= 64, u128 n <= 64,
 *            big any n
 * Helpers:   bigomega
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/bigomega.h"

seq_t a(idx_t n)
{
    seq_t sum = 0;
    for (idx_t i = 1; i <= n / i; ++i) {
        if (n % i == 0) {
            seq_t omega_i = bigomega(i);
            sum += (seq_t)i * omega_i;
            idx_t j = n / i;
            if (j != i) {
                seq_t omega_j = bigomega(j);
                sum += (seq_t)j * omega_j;
            }
        }
    }
    return sum;
}
