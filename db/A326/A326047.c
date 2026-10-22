/*
 * A326047  a(n) = gcd(n-A050449(n), n-A050452(n)), where A050449 and A050452
 *          give the sum of divisors of the form 4k+1 and of the form 4k+3,
 *          respectively.
 * https://oeis.org/A326047
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = gcd(n - A050449(n), n - A050452(n))
 * Range:     u8 n <= 261, u16 n <= 9436, u32 n <= 9436, u64 n <= 9436,
 *            u128 n <= 9436, big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A050449(n): sum of divisors of n congruent to 1 mod 4 */
/* A050452(n): sum of divisors of n congruent to 3 mod 4 */

#include "../utils/gcd.h"

seq_t a(idx_t n)
{
    seq_t sum1 = 0;   /* sum of divisors ≡ 1 (mod 4) */
    seq_t sum3 = 0;   /* sum of divisors ≡ 3 (mod 4) */
    seq_t n_seq = (seq_t)n;

    for (idx_t d = 1; d * d <= n; ++d) {
        if (n % d == 0) {
            idx_t d1 = d;
            idx_t d2 = n / d;

            /* divisor d1 */
            if ((d1 & 3ULL) == 1) {
                sum1 += (seq_t)d1;
            } else if ((d1 & 3ULL) == 3) {
                sum3 += (seq_t)d1;
            }

            /* divisor d2 (if distinct) */
            if (d2 != d1) {
                if ((d2 & 3ULL) == 1) {
                    sum1 += (seq_t)d2;
                } else if ((d2 & 3ULL) == 3) {
                    sum3 += (seq_t)d2;
                }
            }
        }
    }

    /* compute absolute differences */
    seq_t term1 = sum1 > n_seq ? sum1 - n_seq : n_seq - sum1;
    seq_t term2 = sum3 > n_seq ? sum3 - n_seq : n_seq - sum3;

    return gcd(term1, term2);
}
