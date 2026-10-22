/*
 * A344223  a(n) = Sum_{k=1..n} tau(gcd(k,n)^n), where tau(n) is the number of
 *          divisors of n.
 * https://oeis.org/A344223
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=1..n} tau(gcd(k,n)^n)
 * Range:     u8 n <= 11, u16 n <= 41, u32 n <= 329, u64 n <= 6929,
 *            u128 n <= 10000, big any n
 * Helpers:   dfs_divisor
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/dfs_divisor.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0; /* not needed for OEIS offset 1, but harmless */

    /* factor n */
    idx_t temp = n;
    idx_t primes[16];
    int exp[16];
    int pfcount = 0;

    for (idx_t p = 2; p * p <= temp; ++p) {
        if (temp % p == 0) {
            int e = 0;
            while (temp % p == 0) {
                temp /= p;
                ++e;
            }
            primes[pfcount] = p;
            exp[pfcount] = e;
            ++pfcount;
        }
    }
    if (temp > 1) {
        primes[pfcount] = temp;
        exp[pfcount] = 1;
        ++pfcount;
    }

    int cur_exps[16] = {0};
    seq_t sum = 0;
    dfs_divisor(0, n, primes, exp, pfcount, cur_exps, &sum);
    return sum;
}
