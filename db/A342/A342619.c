/*
 * A342619  a(n) = Sum_{d|n} phi(n/d)^(n-d+1).
 * https://oeis.org/A342619
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{d|n} phi(n/d)^(n-d+1) (offset 1)
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 10, u64 n <= 16, u128 n <= 28,
 *            big any n
 * Helpers:   big_pow, phi
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include "../utils/phi.h"
#include "../utils/big_pow.h"

seq_t a(idx_t n) {
    seq_t sum = 0;
    if (n == 0) return 0; /* undefined for offset 1, but safe guard */

    for (idx_t d = 1; d <= n / d; ++d) {
        if (n % d != 0) continue;

        /* first divisor pair: d and n/d */
        idx_t d1 = d;
        idx_t d2 = n / d;

        /* term for divisor d1 */
        seq_t phi_val1 = phi(d2);          /* phi(n/d1) = phi(d2) */
        idx_t exp1 = n - d1 + 1;
        seq_t pow1 = big_pow(phi_val1, (seq_t)exp1);
        sum += pow1;

        /* if d1 != d2, add term for divisor d2 only once */
        if (d1 != d2) {
            seq_t phi_val2 = phi(d1);       /* phi(n/d2) = phi(d1) */
            idx_t exp2 = n - d2 + 1;
            seq_t pow2 = big_pow(phi_val2, (seq_t)exp2);
            sum += pow2;
        }
    }

    return sum;
}
