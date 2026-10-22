/*
 * A325339  Number of divisors of n^3 that are <= n.
 * https://oeis.org/A325339
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = number of divisors of n^3 that are <= n
 * Range:     u8 n <= 789, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   dfs_div_count
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/dfs_div_count.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0;                 /* value outside the offset */
    uint64_t nn = (uint64_t)n;
    if (nn == 1) return 1;                /* a(1) = 1 */

    /* Factorise nn using trial division */
    uint64_t primes[64];
    int exps[64];
    int pc = 0;
    uint64_t temp = nn;

    /* handle factor 2 */
    if ((temp & 1ULL) == 0) {
        int e = 0;
        while ((temp & 1ULL) == 0) {
            temp >>= 1;
            e++;
        }
        primes[pc] = 2ULL;
        exps[pc]    = e;
        pc++;
    }

    /* odd factors */
    for (uint64_t i = 3; i <= temp / i; i += 2) {
        if (temp % i == 0) {
            int e = 0;
            while (temp % i == 0) {
                temp /= i;
                e++;
            }
            primes[pc] = i;
            exps[pc]   = e;
            pc++;
        }
    }

    if (temp > 1) {                 /* remaining prime factor */
        primes[pc] = temp;
        exps[pc]   = 1;
        pc++;
    }

    unsigned long long count = 0;
    dfs_div_count(0, 1, primes, exps, pc, &count, (seq_t)nn);
    return (seq_t)count;
}
