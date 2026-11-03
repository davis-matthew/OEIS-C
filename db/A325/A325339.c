/*
 * A325339  Number of divisors of n^3 that are <= n.
 * https://oeis.org/A325339
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = number of divisors of n^3 that are <= n
 * Range:     u8 n <= 789, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 97 terms through n = 9626 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Recursive helper that counts divisors d of n^3 with d <= nmax.
   primes[] and exps[] give n = ∏ primes[i]^exps[i]  */
static void dfs_div_count(int idx, seq_t cur, const uint64_t primes[], const int exps[],
                           int pc, unsigned long long *cnt, seq_t nmax)
{
    if (idx == pc) {
        (*cnt)++;
        return;
    }

    seq_t prod = cur;
    int maxexp = 3 * exps[idx];
    for (int e = 0; e <= maxexp; ++e) {
        if (prod > nmax) break;          /* no need to continue further */
        dfs_div_count(idx + 1, prod, primes, exps, pc, cnt, nmax);

        if (e == maxexp) break;          /* reached the maximum exponent for this prime */
        /* if next multiplication would exceed nmax, stop growing this branch */
        if (prod > nmax / primes[idx]) break;
        prod *= primes[idx];
    }
}

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
