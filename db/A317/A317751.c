/*
 * A317751  Number of divisors d of n such that there exists a factorization
 *          of n into factors > 1 with GCD d.
 * https://oeis.org/A317751
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of divisors d of n for which there exists a
 *            factorization of n into numbers > 1 whose greatest common
 *            divisor is d. It can be computed by enumerating all divisors of
 *            n and applying the following criterion: - d = n is always valid.
 *            - d = 1 is valid iff n is not a prime power. - For d...
 * Range:     u8 n <= 303, u16 n <= 9205, u32 n <= 9205, u64 n <= 9205,
 *            u128 n <= 9205, big any n
 * Helpers:   dfs_r
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include "../utils/dfs_r.h"

/* Forward declaration of helper recursive function */
static void dfs_r(size_t idx, seq_t cur, size_t *f, uint64_t *primes, size_t *exps,
                  size_t rc, uint64_t n_val, bool n_prime_power, seq_t *cnt);

/* Main function computing the sequence */
seq_t a(idx_t n)
{
    if (n <= 1) return 0;                 /* a(1) = 0 */

    /* Factor n by trial division */
    uint64_t nn = (uint64_t)n;
    uint64_t primes[32];
    size_t   exps[32];
    size_t   rc = 0;
    uint64_t tmp = nn;

    for (uint64_t p = 2; p * p <= tmp; ++p) {
        if (tmp % p == 0) {
            size_t e = 0;
            while (tmp % p == 0) {
                tmp /= p;
                ++e;
            }
            primes[rc] = p;
            exps[rc]   = e;
            ++rc;
        }
    }
    if (tmp > 1) {
        primes[rc] = tmp;
        exps[rc]   = 1;
        ++rc;
    }

    bool n_is_prime_power = (rc == 1);

    seq_t count = 0;
    size_t f[32] = {0};

    dfs_r(0, (seq_t)1, f, primes, exps, rc, nn, n_is_prime_power, &count);
    return count;
}
