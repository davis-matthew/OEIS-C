/*
 * A344210  a(n) = Sum_{d|n} mu(n/d) * d^n / phi(n).
 * https://oeis.org/A344210
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{d|n} mu(n/d) * d^n / phi(n) (OEIS A344210)
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 9, u64 n <= 16, u128 n <= 19,
 *            big any n
 * Helpers:   big_pow, build_primes_upto, calc_phi, floor_sqrt, mu_of
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

#include "../utils/build_primes_upto.h"
#include "../utils/calc_phi.h"
#include "../utils/big_pow.h"
#include "../utils/mu_of.h"
#include "../utils/floor_sqrt.h"

/* Recursive helper to generate all divisors of n and sum d^n weighted by mu. */
static void
recur_divisors(size_t pos, seq_t cur_d, idx_t n,
               const uint64_t *factors, const idx_t *exps,
               size_t pcnt, seq_t *pos_sum, seq_t *neg_sum)
{
    if (pos == pcnt) {
        /* Current divisor is cur_d. Compute mu of n / cur_d. */
        idx_t n_div = n / (idx_t)cur_d;
        int mu = mu_of(n_div);
        if (mu == 0) return;

        seq_t power = big_pow(cur_d, (seq_t)n);
        if (mu > 0) {
            *pos_sum += power;
        } else { /* mu == -1 */
            *neg_sum += power;
        }
        return;
    }

    /* Iterate over exponent of the current prime factor. */
    seq_t next = cur_d;
    for (idx_t e = 0; e <= exps[pos]; ++e) {
        recur_divisors(pos + 1, next, n, factors, exps, pcnt, pos_sum, neg_sum);
        /* Prepare next divisor: multiply by the prime for the next exponent. */
        next = next * (seq_t)factors[pos];
    }
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;      /* undefined for n=0, safe fallback */

    /* Generate a list of primes up to sqrt(n). */
    uint64_t *prime_list = NULL;
    size_t prime_cnt = 0;
    idx_t limit = (idx_t)floor_sqrt((seq_t)n);
    build_primes_upto(limit, &prime_list, &prime_cnt);

    /* Factor n into distinct primes and their exponents. */
    uint64_t factors[64];
    idx_t exps[64];
    size_t pcnt = 0;
    uint64_t m = (uint64_t)n;
    for (size_t i = 0; i < prime_cnt && m > 1; ++i) {
        uint64_t p = prime_list[i];
        if (p * p > m) break;
        if (m % p == 0) {
            idx_t e = 0;
            while (m % p == 0) {
                m /= p;
                ++e;
            }
            factors[pcnt] = p;
            exps[pcnt] = e;
            ++pcnt;
        }
    }
    if (m > 1 && pcnt < 64) {          /* remaining prime factor */
        factors[pcnt] = m;
        exps[pcnt] = 1;
        ++pcnt;
    }

    free(prime_list);   /* primes allocated by build_primes_upto */

    /* Sum over all divisors. */
    seq_t pos_sum = 0;
    seq_t neg_sum = 0;
    recur_divisors(0, (seq_t)1, n, factors, exps, pcnt, &pos_sum, &neg_sum);

    seq_t sum = pos_sum - neg_sum;     /* pos_sum >= neg_sum for this sequence */

    /* Divide by phi(n). */
    seq_t phi_n = calc_phi((seq_t)n);
    return sum / phi_n;
}
