/*
 * A380844  The number of divisors of n that have the same binary weight as n.
 * https://oeis.org/A380844
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = Sum_{d|n} [popcount(d) = popcount(n)]
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*   where popcount(x) = number of 1-bits in binary representation of x. */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* Recursive helper: counts divisors of the original number that have the
   same binary weight as the target. */
static void dfs(int idx, uint64_t val,
                uint64_t *primes, int *exps, int cnt,
                uint8_t target, seq_t *res)
{
    if (idx == cnt) {
        /* Base case: val is a divisor.  Compare popcounts. */
        if (__builtin_popcountll(val) == target)
            (*res)++;
        return;
    }
    uint64_t t = val;
    for (int k = 0; k <= exps[idx]; ++k) {
        dfs(idx + 1, t, primes, exps, cnt, target, res);
        t *= primes[idx];
    }
}

/* Main function required by the kernel */
seq_t a(idx_t n)
{
    uint64_t un = n;
    int w = __builtin_popcountll(un);                /* binary weight of n */

    /* Factorization: gather primes and their exponents. */
    uint64_t primes[20];
    int exps[20];
    int cnt = 0;
    uint64_t m = un;

    /* Handle factor 2 separately for efficiency. */
    if (m % 2 == 0) {
        int e = 0;
        while (m % 2 == 0) { m /= 2; e++; }
        primes[cnt] = 2;
        exps[cnt] = e;
        cnt++;
    }

    /* Odd factors */
    for (uint64_t p = 3; p <= m / p; p += 2) {
        if (m % p == 0) {
            int e = 0;
            while (m % p == 0) { m /= p; e++; }
            primes[cnt] = p;
            exps[cnt] = e;
            cnt++;
        }
    }

    if (m > 1) {               /* remaining prime factor */
        primes[cnt] = m;
        exps[cnt] = 1;
        cnt++;
    }

    /* Count divisors with matching popcount. */
    seq_t result = 0;
    dfs(0, 1, primes, exps, cnt, (uint8_t)w, &result);
    return result;
}
