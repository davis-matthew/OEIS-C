/*
 * A333645  a(n) = Sum_{d|n} uphi(d).
 * https://oeis.org/A333645
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, mult
 * Formula:   a(n) = Sum_{d|n} uphi(d) where uphi is multiplicative with
 *            uphi(1) = 1 , uphi(p^e) = p^e - 1 (e >= 1)
 * Range:     u8 n <= 143, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   dfs_divisor_uphi
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/dfs_divisor_uphi.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0;                  /* not defined for n=0 */

    uint64_t temp = (uint64_t)n;
    uint64_t primes[64];
    int exps[64];
    int num = 0;

    for (uint64_t p = 2; p <= temp / p; ++p) {
        if (temp % p == 0) {
            int e = 0;
            while (temp % p == 0) {
                temp /= p;
                ++e;
            }
            primes[num] = p;
            exps[num]   = e;
            ++num;
        }
    }
    if (temp > 1) {
        primes[num] = temp;
        exps[num]   = 1;
        ++num;
    }

    seq_t sum = 0;
    dfs_divisor_uphi(primes, exps, num, 0, (seq_t)1, &sum);
    return sum;
}
