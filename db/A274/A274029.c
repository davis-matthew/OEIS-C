/*
 * A274029  Product of infinitary divisors of n.
 * https://oeis.org/A274029
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Π_{d|n, d infinitary} d. For n > 1, a(n) = n^(2^(s-1)),
 *            where s = Σ popcount(exponent of each prime in n). a(1) = 1
 * Range:     u8 n <= 17, u16 n <= 23, u32 n <= 119, u64 n <= 263,
 *            u128 n <= 839, big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    if (n == 1) return (seq_t)1;

    /* factor n and sum popcount of each exponent */
    idx_t temp = n;
    unsigned long long sumPop = 0;

    /* handle factor 2 */
    if ((temp & 1ULL) == 0) {
        unsigned int e = 0;
        while ((temp & 1ULL) == 0) {
            temp >>= 1;
            ++e;
        }
        sumPop += __builtin_popcount(e);
    }

    /* odd factors */
    for (idx_t p = 3; p <= temp / p; p += 2) {
        if (temp % p == 0) {
            unsigned int e = 0;
            while (temp % p == 0) {
                temp /= p;
                ++e;
            }
            sumPop += __builtin_popcount(e);
        }
    }

    /* remaining prime factor > 1 */
    if (temp > 1) {                /* exponent of the remaining prime is 1 */
        sumPop += __builtin_popcount((unsigned int)1); /* =1 */
    }

    /* exponent = 2^(sumPop-1) */
    unsigned long long exp = 1ULL << (sumPop - 1);

    /* compute n^exp */
    seq_t base = (seq_t)n;
    return big_pow(base, (seq_t)exp);
}
