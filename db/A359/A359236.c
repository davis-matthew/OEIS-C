/*
 * A359236  Number of divisors of 5*n-2 of form 5*k+1.
 * https://oeis.org/A359236
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = number of divisors of 5*n-2 that are congruent to 1
 *            modulo 5
 * Range:     u8 n <= 51, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static void count_divisors(const uint64_t *primes, const int *exps, int pcnt,
                           int idx, uint64_t val, seq_t *cnt)
{
    if (idx == pcnt) {
        if ((val % 5ULL) == 1ULL)
            (*cnt)++;
        return;
    }
    uint64_t p = primes[idx];
    uint64_t v = val;
    for (int e = 0; e <= exps[idx]; ++e) {
        count_divisors(primes, exps, pcnt, idx + 1, v, cnt);
        if (e < exps[idx])
            v *= p;
    }
}

seq_t a(idx_t n)
{
    seq_t m = (seq_t)n * 5 - 2;               /* 5*n - 2 */
    uint64_t m64 = (uint64_t)m;               /* assume m fits into uint64_t */

    /* factor m64 into primes */
    uint64_t primes[64];
    int exps[64];
    int pcnt = 0;
    uint64_t tmp = m64;

    if (tmp % 2ULL == 0) {
        int e = 0;
        while (tmp % 2ULL == 0) {
            tmp /= 2ULL;
            ++e;
        }
        primes[pcnt] = 2ULL;
        exps[pcnt] = e;
        ++pcnt;
    }
    for (uint64_t p = 3; p * p <= tmp; p += 2) {
        if (tmp % p == 0) {
            int e = 0;
            while (tmp % p == 0) {
                tmp /= p;
                ++e;
            }
            primes[pcnt] = p;
            exps[pcnt] = e;
            ++pcnt;
        }
    }
    if (tmp > 1) {
        primes[pcnt] = tmp;
        exps[pcnt] = 1;
        ++pcnt;
    }

    seq_t count = 0;
    count_divisors(primes, exps, pcnt, 0, 1ULL, &count);
    return count;
}
