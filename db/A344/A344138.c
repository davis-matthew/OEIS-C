/*
 * A344138  a(n) = Sum_{x_1|n, x_2|n, x_3|n, x_4|n} gcd(x_1,x_2,x_3,x_4).
 * https://oeis.org/A344138
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, mult
 * Formula:   a(n) = Sum_{d|n} phi(n/d) * tau(d)^4
 * Range:     u8 n <= 5, u16 n <= 119, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   rec_func
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/rec_func.h"

static void rec_func(int idx, idx_t d_acc, uint64_t tau_acc, seq_t phi_acc, seq_t *ans,
                     const uint64_t *primes, const int *exps, int num);

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;
    uint64_t temp = (uint64_t)n;
    uint64_t primes[16];
    int exps[16];
    int num = 0;
    for (uint64_t p = 2; p * p <= temp; ++p) {
        if (temp % p == 0) {
            int cnt = 0;
            while (temp % p == 0) {
                temp /= p;
                ++cnt;
            }
            primes[num] = p;
            exps[num] = cnt;
            ++num;
        }
    }
    if (temp > 1) {
        primes[num] = temp;
        exps[num] = 1;
        ++num;
    }

    seq_t ans = 0;
    rec_func(0, (idx_t)1, 1, (seq_t)1, &ans,
             primes, exps, num);
    return ans;
}
