/*
 * A327669  Sum of divisors of n that have an odd number of distinct prime
 *          factors.
 * https://oeis.org/A327669
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{d|n} d if d has an odd number of distinct prime *
 *            factors, else 0. * This implementation factorises n, then
 *            enumerates all its divisors, * adding those whose number of
 *            distinct prime factors is odd
 * Range:     u8 n <= 155, u16 n <= 9910, u32 n <= 9910, u64 n <= 9910,
 *            u128 n <= 9910, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

static void dfs(const uint64_t *primes, const unsigned int *exps, int idx,
                seq_t current, int parity, seq_t *sum, int num)
{
    if (idx == num) {
        if (parity & 1)
            *sum += current;
        return;
    }

    seq_t powp = (seq_t)1;
    unsigned int p = (unsigned int)primes[idx];
    unsigned int e = exps[idx];

    for (unsigned int k = 0; k <= e; ++k) {
        int new_parity = parity ^ ((k > 0) ? 1 : 0);
        dfs(primes, exps, idx + 1, current * powp, new_parity, sum, num);
        if (k < e)
            powp *= (seq_t)p;   /* multiply by prime for next exponent */
    }
}

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;   /* offset is 1, value for n==0 is irrelevant */

    /* Factorise n into primes */
    uint64_t primes[64];
    unsigned int exps[64];
    int num = 0;

    uint64_t m = (uint64_t)n;
    unsigned int e = 0;

    /* factor 2 */
    if (m % 2 == 0) {
        e = 0;
        while (m % 2 == 0) {
            m /= 2;
            ++e;
        }
        primes[num] = 2;
        exps[num]   = e;
        ++num;
    }

    /* odd factors */
    uint64_t p = 3;
    for (; p <= m / p; p += 2) {
        if (m % p == 0) {
            e = 0;
            while (m % p == 0) {
                m /= p;
                ++e;
            }
            primes[num] = p;
            exps[num]   = e;
            ++num;
        }
    }
    if (m > 1) {
        primes[num] = m;
        exps[num]   = 1;
        ++num;
    }

    /* Enumerate all divisors, summing those with odd distinct prime count */
    seq_t sum = 0;
    dfs(primes, exps, 0, (seq_t)1, 0, &sum, num);

    return sum;
}
