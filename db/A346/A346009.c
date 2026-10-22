/*
 * A346009  a(n) is the numerator of the average number of distinct prime
 *          factors of the divisors of n.
 * https://oeis.org/A346009
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, frac
 * Formula:   a(n) = numerator of reduced fraction sum_{p^e||n} e/(e+1)
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   factor_u64, gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include "../utils/factor_u64.h"
#include "../utils/gcd.h"

static const size_t MAX_FACTS = 128;   /* enough for any n < 2^64 */

seq_t a(idx_t n)
{
    if (n == 1) return 0;             /* a(1) = 0 */

    uint64_t primes[MAX_FACTS];
    uint64_t exps[MAX_FACTS];
    size_t cnt = 0;

    factor_u64((uint64_t)n, primes, exps, &cnt);

    /* lcm of all (e_i + 1) */
    seq_t lcm = 1;
    for (size_t i = 0; i < cnt; ++i) {
        seq_t denom = (seq_t)(exps[i] + 1);
        seq_t g = gcd(lcm, denom);
        lcm = (lcm / g) * denom;
    }

    /* numerator of the unreduced sum */
    seq_t num = 0;
    for (size_t i = 0; i < cnt; ++i) {
        seq_t term = (seq_t)exps[i];
        seq_t mult = lcm / ((seq_t)(exps[i] + 1));
        num += term * mult;
    }

    /* reduce: num / lcm */
    seq_t g = gcd(num, lcm);
    num /= g;

    return num;
}
