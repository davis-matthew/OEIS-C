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
 * Validated: standard, pass, 86 terms through n = 9875 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Recursive helper to enumerate all exponent combinations
   and accumulate the sum of uphi over all divisors. */
static void dfs_divisor_uphi(const uint64_t *primes,
                             const int *exps,
                             int num,
                             int idx,
                             seq_t current_uphi,
                             seq_t *sum)
{
    if (idx == num) {
        *sum += current_uphi;
        return;
    }
    seq_t p = (seq_t)primes[idx];
    seq_t pow = 1;                         /* p^k */
    for (int k = 0; k <= exps[idx]; ++k) {
        seq_t factor = (k == 0) ? (seq_t)1 : (pow - 1);
        dfs_divisor_uphi(primes, exps, num, idx + 1,
                         current_uphi * factor, sum);
        pow *= p;                           /* next power */
    }
}

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
