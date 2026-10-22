/*
 * A378542  Sum of divisors d of n such that n/d has an even number of prime
 *          factors (counted with multiplicity).
 * https://oeis.org/A378542
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{d|n, Ω(n/d) even} d where Ω(m) is the total number
 *            of prime factors of m counted with multiplicity
 * Range:     u8 n <= 167, u16 n <= 9240, u32 n <= 9240, u64 n <= 9240,
 *            u128 n <= 9240, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Recursive helper to enumerate all divisors of n */
static void dfs_divisors(const uint64_t *primes, const int *exps, int cnt,
                         int idx, int exp_sum, seq_t cur, seq_t *res, int tot_exp)
{
    if (idx == cnt) {
        if (((tot_exp - exp_sum) & 1) == 0)   /* even number of prime factors in n/d */
            *res += cur;
        return;
    }

    seq_t prod = cur;
    int esum   = exp_sum;

    for (int k = 0; k <= exps[idx]; ++k) {
        dfs_divisors(primes, exps, cnt, idx + 1, esum, prod, res, tot_exp);
        prod *= (seq_t)primes[idx];
        esum += 1;
    }
}

seq_t a(idx_t n)
{
    if (n < 1) return 0;              /* undefined for n < 1 */

    /* Factor n by trial division (fast enough for n ≤ 10000 and acceptable for larger n as well) */
    uint64_t primes[64];
    int     exps[64];
    int     cnt = 0;
    uint64_t tmp = (uint64_t)n;

    for (uint64_t p = 2; p * p <= tmp; ++p) {
        if (tmp % p == 0) {
            int e = 0;
            while (tmp % p == 0) {
                tmp /= p;
                ++e;
            }
            primes[cnt] = p;
            exps[cnt]   = e;
            ++cnt;
        }
    }
    if (tmp > 1) {
        primes[cnt] = tmp;
        exps[cnt]   = 1;
        ++cnt;
    }

    /* Total number of prime factors of n (with multiplicity) */
    int tot_exp = 0;
    for (int i = 0; i < cnt; ++i) tot_exp += exps[i];

    seq_t result = 0;
    dfs_divisors(primes, exps, cnt, 0, 0, (seq_t)1, &result, tot_exp);
    return result;
}
