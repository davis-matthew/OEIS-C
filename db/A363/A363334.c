/*
 * A363334  a(n) is the sum of divisors of n that are both coreful and
 *          bi-unitary.
 * https://oeis.org/A363334
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, mult
 * Formula:   a(n) = sum of divisors of n that are both coreful and
 *            bi-unitary. Multiplicative with a(p^e) = (p^(e+1)-1)/(p-1)-1 if
 *            e is odd, (p^(e+1)-1)/(p-1)-p^(e/2)-1 if e is even
 * Range:     u8 n <= 151, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include "../utils/factor_u64.h"

seq_t a(idx_t n)
{
    if (n == 1) return (seq_t)1;

    uint64_t nn = (uint64_t)n;
    uint64_t primes[64];
    uint64_t exps[64];
    size_t cnt;

    factor_u64(nn, primes, exps, &cnt);

    seq_t result = 1;

    for (size_t i = 0; i < cnt; ++i) {
        uint64_t p = primes[i];
        uint64_t e = exps[i];
        seq_t sum = 1;          /* start with p^0 */
        seq_t pow = 1;
        for (unsigned j = 1; j <= e; ++j) {
            pow *= (seq_t)p;
            sum += pow;
        }
        seq_t val = sum;
        if ((e & 1) == 0) { /* e even */
            seq_t mid = 1;
            unsigned half = (unsigned)(e / 2);
            for (unsigned j = 0; j < half; ++j) {
                mid *= (seq_t)p;
            }
            val -= mid;
        }
        val -= 1; /* remove the divisor 1 */
        result *= val;
    }

    return result;
}
