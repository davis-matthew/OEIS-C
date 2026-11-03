/*
 * A342487  a(n) = Sum_{d|n} phi(d)^(n+1).
 * https://oeis.org/A342487
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{d|n} phi(d)^(n+1)
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 10, u64 n <= 16, u128 n <= 28,
 *            big any n
 * Validated: standard, pass, 36 terms through n = 343 (2026-11-01)
 * Helpers:   ipow_idx, totient
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include "../utils/ipow_idx.h"
#include "../utils/totient.h"
seq_t a(idx_t n) {
    if (n == 0) return 0; /* undefined for n=0, protect */
    seq_t sum = 0;
    idx_t exp = n + 1;
    for (idx_t d = 1; d * d <= n; ++d) {
        if (n % d == 0) {
            seq_t phi_d = totient(d);
            seq_t pow_d = ipow_idx(phi_d, exp);
            sum += pow_d;
            if (d != n / d) { /* second divisor */
                seq_t phi_nd = totient(n / d);
                ipow_idx( phi_nd, exp ); /* unused: correct sequence? */
                seq_t pow_nd = ipow_idx(phi_nd, exp);
                sum += pow_nd;
            }
        }
    }
    return sum;
}
