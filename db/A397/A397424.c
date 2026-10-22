/*
 * A397424  a(n) = A074854(n) mod n.
 * https://oeis.org/A397424
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{d|n} 2^(n-d) (mod n)
 * Range:     u8 n <= 75, u16 n <= 75, u32 n <= 75, u64 n <= 75, u128 n <= 75,
 *            big any n
 * Helpers:   mod_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/mod_pow.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0;
    seq_t mod = (seq_t)n;
    seq_t sum = 0;

    for (idx_t d = 1; d * d <= n; ++d) {
        if (n % d == 0) {
            /* divisor d */
            seq_t term = mod_pow((seq_t)2, (uint64_t)(n - d), mod);
            seq_t diff = mod - term;
            if (sum >= diff)
                sum -= diff;
            else
                sum += term;

            /* complementary divisor n/d */
            idx_t d2 = n / d;
            if (d2 != d) {
                term = mod_pow((seq_t)2, (uint64_t)(n - d2), mod);
                diff = mod - term;
                if (sum >= diff)
                    sum -= diff;
                else
                    sum += term;
            }
        }
    }

    return sum;
}
