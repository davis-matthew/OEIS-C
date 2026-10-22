/*
 * A363901  Expansion of Sum_{k>0} x^k / (1 - x^(3*k))^2.
 * https://oeis.org/A363901
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (1/3) * Σ_{d|n, d≡1 (mod 3)} (d + 2)
 * Range:     u8 n <= 519, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Compute the nth term of OEIS sequence A363901.
 * a(1) is the first term listed in the OEIS entry.
 */
seq_t a(idx_t n)
{
    if (n < 1) return 0;            /* offset starts at 1 */
    seq_t sum = 0;                  /* Σ of divisors ≡ 1 (mod 3)   */
    seq_t cnt = 0;                  /* number of such divisors     */

    for (idx_t k = 1; k <= n / k; ++k) {
        if (n % k == 0) {
            /* divisor k */
            if (k % 3 == 1) {
                sum += (seq_t)k;
                cnt += 1;
            }

            /* complementary divisor n/k */
            idx_t d = n / k;
            if (d != k && d % 3 == 1) {
                sum += (seq_t)d;
                cnt += 1;
            }
        }
    }

    /* a(n) = (sum + 2*cnt) / 3 */
    return (sum + 2 * cnt) / 3;
}
