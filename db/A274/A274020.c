/*
 * A274020  Number of n-bead 5-ary necklaces (no turning over allowed) that
 *          avoid the subsequence 110.
 * https://oeis.org/A274020
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 13, u64 n <= 27, u128 n <= 27,
 *            big any n
 * Helpers:   calc_c, phi
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = (1/n) * Σ_{d|n} φ(n/d) * c(d)
 * where c(1) = 5, c(2) = 25, c(3) = 122,
 * and for n ≥ 4:    c(n) = 5 * c(n-1) - c(n-3)
 * For n = 0 the value is defined to be 1.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/phi.h"   /* provides seq_t phi(idx_t m) */
#include "../utils/calc_c.h"

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;

    seq_t sum = (seq_t)0;
    for (idx_t i = 1; (idx_t)(i * i) <= n; ++i) {
        if (n % i == 0) {
            idx_t d1 = i;
            idx_t d2 = n / i;

            seq_t phi1 = phi(n / d1);
            seq_t c1   = calc_c(d1);
            sum = sum + phi1 * c1;

            if (d1 != d2) {
                seq_t phi2 = phi(n / d2);
                seq_t c2   = calc_c(d2);
                sum = sum + phi2 * c2;
            }
        }
    }
    return sum / (seq_t)n;
}
