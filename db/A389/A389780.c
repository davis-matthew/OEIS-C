/*
 * A389780  Number of divisors d of n such that gcd(d^2 - 1, n) = 1 and
 *          gcd((n/d)^2 - 1, n) != 1.
 * https://oeis.org/A389780
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of divisors d of n such that gcd(d^2-1, n) = 1 and
 *            gcd((n/d)^2-1, n) != 1
 * Range:     u8 n <= 23, u16 n <= 534, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/gcd.h"

/* Recursive helper to generate all divisors and count those that satisfy the
   given conditions. */
static void gen_divisors(const uint64_t primes[], const int exps[],
                         int idx, int num_primes, uint64_t curr,
                         uint64_t n, seq_t *count)
{
    if (idx == num_primes) {
        /* current divisor is 'curr' */
        seq_t d = curr;
        seq_t v1 = d * d - 1;
        seq_t g1 = gcd(v1, (seq_t)n);

        uint64_t m = n / curr;
        seq_t mm = m;
        seq_t v2 = mm * mm - 1;
        seq_t g2 = gcd(v2, (seq_t)n);

        if (g1 == 1 && g2 != 1) {
            (*count)++;
        }
        return;
    }

    uint64_t power = 1;
    for (int k = 0; k <= exps[idx]; ++k) {
        gen_divisors(primes, exps, idx + 1, num_primes,
                     curr * power, n, count);
        power *= primes[idx];
    }
}

seq_t a(idx_t n)
{
    if (n < 1) return 0;            /* offset starts at 1 */
    uint64_t nn = n;

    /* factor n */
    uint64_t primes[25];
    int exps[25];
    int num = 0;

    uint64_t m = nn;
    for (uint64_t p = 2; p * p <= m; ++p) {
        if (m % p == 0) {
            int e = 0;
            while (m % p == 0) {
                m /= p;
                ++e;
            }
            primes[num] = p;
            exps[num] = e;
            ++num;
        }
    }
    if (m > 1) {
        primes[num] = m;
        exps[num] = 1;
        ++num;
    }

    seq_t count = 0;
    gen_divisors(primes, exps, 0, num, 1, nn, &count);
    return count;
}
