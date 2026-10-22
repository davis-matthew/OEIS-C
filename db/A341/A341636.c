/*
 * A341636  a(n) = Sum_{d|n} phi(d) * tau(d) * tau(n/d).
 * https://oeis.org/A341636
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, mult
 * Formula:   a(n) = Sum_{d|n} φ(d) τ(d) τ(n/d)
 * Range:     u8 n <= 31, u16 n <= 1439, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   divisor_count_u64, phi
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/phi.h"
#include "../utils/divisor_count_u64.h"

seq_t a(idx_t n) {
    if (n == 0) return 0; /* safety, not used in the sequence */
    seq_t sum = 0;
    for (idx_t d = 1; d <= n; ++d) {
        if (n % d != 0) continue;
        seq_t phi_d = phi((idx_t)d);
        seq_t tau_d = (seq_t)divisor_count_u64(d);
        idx_t nd = n / d;
        seq_t tau_nd = (seq_t)divisor_count_u64(nd);
        seq_t prod = phi_d * tau_d;
        prod *= tau_nd;
        sum += prod;
    }
    return sum;
}
