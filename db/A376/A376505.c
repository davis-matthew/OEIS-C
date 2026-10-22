/*
 * A376505  Number of m <= n such that rad(m) | n that are neither squarefree
 *          nor prime powers, where rad = A007947.
 * https://oeis.org/A376505
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = number of m ≤ n such that rad(m) | n, m is not squarefree
 *            and not a prime power. Implementation enumerates all numbers m
 *            formed from primes dividing n, counts those with at least two
 *            distinct primes and at least one exponent ≥ 2
 * Range:     u8 n <= 3569, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>

/* Recursive helper that counts qualifying numbers.
   idx     : current index in primes array
   prod    : product of chosen prime powers so far
   distinct: number of distinct primes chosen
   hasGreater: true if any chosen exponent ≥ 2
   primes  : array of distinct prime factors of n
   pcnt    : number of primes in the array
   n       : upper bound for product
   ans     : pointer to result accumulator */
static void dfs(size_t idx, uint64_t prod, int distinct, int hasGreater,
                const uint64_t *primes, size_t pcnt, uint64_t n, seq_t *ans)
{
    if (prod > n)
        return;
    if (idx == pcnt) {
        if (prod > 1 && distinct > 1 && hasGreater) {
            seq_t one = 1;
            (*ans) += one;
        }
        return;
    }
    /* Option: skip the current prime */
    dfs(idx + 1, prod, distinct, hasGreater, primes, pcnt, n, ans);

    /* Option: include current prime with exponent >= 1 */
    uint64_t p = primes[idx];
    uint64_t powv = p;
    for (int exp = 1;; ++exp) {
        if (prod > n / powv)
            break;
        uint64_t newprod = prod * powv;
        int newHasGreater = hasGreater || (exp >= 2);
        dfs(idx + 1, newprod, distinct + 1, newHasGreater,
            primes, pcnt, n, ans);
        /* Prepare next exponent */
        if (powv > n / p)
            break;
        powv *= p;
    }
}

seq_t a(idx_t n)
{
    uint64_t nn = (uint64_t)n;
    /* Factor n to obtain its distinct primes */
    uint64_t temp = nn;
    uint64_t primes[64];   /* enough for any 64‑bit number */
    size_t pcnt = 0;

    if ((temp & 1) == 0) {
        primes[pcnt++] = 2;
        while ((temp & 1) == 0)
            temp >>= 1;
    }
    for (uint64_t p = 3; p * p <= temp; p += 2) {
        if (temp % p == 0) {
            primes[pcnt++] = p;
            while (temp % p == 0)
                temp /= p;
        }
    }
    if (temp > 1)
        primes[pcnt++] = temp;

    seq_t ans = 0;
    dfs(0, 1, 0, 0, primes, pcnt, nn, &ans);
    return ans;
}
