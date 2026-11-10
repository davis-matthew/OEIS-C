/*
 * A296082  a(1) = 0; for n > 1, a(n) = A032741(n) /
 *          gcd(A039653(n),A032741(n)).
 * https://oeis.org/A296082
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1) = 0; for n > 1, a(n) = (tau(n)-1) / gcd(sigma(n)-1,
 *            tau(n)-1), where tau = number of divisors, sigma = sum of
 *            divisors
 * Range:     u8 n <= 1267, u16 n <= 16384, u32 n <= 16384, u64 n <= 16384,
 *            u128 n <= 16384, big any n
 * Validated: standard, pass, 121 terms through n = 9831 (2026-11-08)
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/gcd.h"

seq_t a(idx_t n)
{
    if (n <= 1)
        return (seq_t)0;

    seq_t tau = 0;
    seq_t sum = 0;
    for (idx_t i = 1; i <= n / i; ++i) {
        if (n % i == 0) {
            idx_t j = n / i;
            if (i == j) {
                tau += 1;
                sum += (seq_t)i;
            } else {
                tau += 2;
                sum += (seq_t)i + (seq_t)j;
            }
        }
    }

    seq_t p = tau - 1;   /* A032741(n) */
    seq_t s = sum - 1;   /* A039653(n) */
    seq_t g = gcd(s, p);
    return p / g;
}
