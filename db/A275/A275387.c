/*
 * A275387  Numbers of ordered pairs of divisors d < e of n such that gcd(d,
 *          e) > 1.
 * https://oeis.org/A275387
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{d1|n, d2|n, d1<d2} (1-[gcd(d1,d2)=1])
 * Range:     u8 n <= 436, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Number of ordered divisor pairs d < e of n with gcd(d,e) > 1 */

#include <stdbool.h>
#include <stdlib.h>
#include "../utils/gcd.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* undefined for n=0, return 0 */

    /* Collect all divisors of n in a small local array. */
    seq_t divis[128];
    size_t cnt = 0;
    for (idx_t i = 1; i * i <= n; ++i) {
        if (n % i == 0) {
            divis[cnt++] = (seq_t)i;
            if (i != n / i) divis[cnt++] = (seq_t)(n / i);
        }
    }

    /* Total number of divisor pairs d < e. */
    seq_t total = (seq_t)cnt * ((seq_t)cnt - 1) / 2;

    /* Count how many of those pairs have gcd == 1. */
    seq_t coprime = 0;
    for (size_t a = 0; a < cnt; ++a) {
        for (size_t b = a + 1; b < cnt; ++b) {
            if (gcd(divis[a], divis[b]) == 1) {
                ++coprime;
            }
        }
    }

    /* The answer is total pairs minus coprime pairs. */
    return total - coprime;
}
