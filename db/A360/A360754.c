/*
 * A360754  Expansion of Sum_{k>0} (k * x * (1 + (2 * x)^k))^k.
 * https://oeis.org/A360754
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{d|n} 2^(n-d) * d^d * binomial(d, n/d-1)
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 9, u64 n <= 15, u128 n <= 18,
 *            big any n
 * Helpers:   binom, pow2, pow_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binom.h"
#include "../utils/pow2.h"
#include "../utils/pow_seq.h"

seq_t a(idx_t n) {
    if (n == 0) return 0;  /* offset starts at 1, but guard for 0 */

    seq_t result = 0;
    idx_t i;

    for (i = 1; i * i <= n; ++i) {
        if (n % i == 0) {
            /* divisor d1 = i */
            idx_t d1 = i;
            idx_t m1 = n / d1;          /* equals n/d1 */
            idx_t k1 = m1 - 1;           /* n/d - 1 */

            if (k1 <= d1) {
                seq_t term = pow2((idx_t)(n - d1));
                term = term * pow_seq((seq_t)d1, (int)d1);
                term = term * binom((idx_t)d1, k1);
                result += term;
            }

            /* divisor d2 = n/i (if distinct) */
            idx_t d2 = n / i;
            if (d2 != d1) {
                idx_t m2 = n / d2;          /* equals i */
                idx_t k2 = m2 - 1;
                if (k2 <= d2) {
                    seq_t term = pow2((idx_t)(n - d2));
                    term = term * pow_seq((seq_t)d2, (int)d2);
                    term = term * binom((idx_t)d2, k2);
                    result += term;
                }
            }
        }
    }

    return result;
}
