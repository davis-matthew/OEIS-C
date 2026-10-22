/*
 * A351414  Number of divisors of n that are either prime or have at least 1
 *          square divisor > 1 and at least two distinct prime factors.
 * https://oeis.org/A351414
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Sum_{d|n} [[omega(d) = 1] = mu(d)^2]
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Counts the number of divisors of n that are either prime or have at least one square factor >1
   and at least two distinct prime factors. */

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

/* Recursive helper to enumerate all divisors of n and count those that satisfy the condition. */
static void dfs_divisors(int pos,
                         const uint64_t *primes, const int *exps, int len,
                         uint64_t cur, int omega, int mu_sq,
                         seq_t *cnt)
{
    if (pos == len) {
        /* (omega==1) == mu_sq  */
        bool cond = ((omega == 1) == (mu_sq != 0));
        if (cond) (*cnt)++;
        return;
    }
    /* Case e = 0 (prime not used) */
    dfs_divisors(pos + 1, primes, exps, len, cur, omega, mu_sq, cnt);

    uint64_t cur_mul = cur;
    for (int e = 1; e <= exps[pos]; ++e) {
        cur_mul *= primes[pos];
        int new_omega = omega + 1;                     /* prime appears at least once */
        int new_mu_sq = mu_sq & (e <= 1 ? 1 : 0);      /* zero if any exponent >=2 */
        dfs_divisors(pos + 1, primes, exps, len, cur_mul,
                     new_omega, new_mu_sq, cnt);
    }
}

/* Main function required by the framework. */
seq_t a(idx_t n)
{
    if (n < 1) return 0;
    uint64_t val = n;

    /* Factorize n into primes and exponents (n ≤ ~10^4 => at most 10 distinct primes). */
    uint64_t primes[10];
    int exps[10];
    int len = 0;
    for (uint64_t p = 2; p * p <= val; ++p) {
        if (val % p == 0) {
            int e = 0;
            while (val % p == 0) {
                val /= p;
                ++e;
            }
            primes[len] = p;
            exps[len] = e;
            ++len;
        }
    }
    if (val > 1) {
        primes[len] = val;
        exps[len] = 1;
        ++len;
    }

    seq_t count = 0;
    dfs_divisors(0, primes, exps, len, 1, 0, 1, &count);
    return count;
}
