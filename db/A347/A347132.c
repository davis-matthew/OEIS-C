/*
 * A347132  a(n) = Sum_{d|n} A001615(n/d) * A003415(d), where A003415 is the
 *          arithmetic derivative and A001615 is Dedekind psi function.
 * https://oeis.org/A347132
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{d|n} A001615(n/d) * A003415(d)
 * Range:     u8 n <= 31, u16 n <= 959, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   build_primes_upto
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*          Dedekind ψ(n/d) (multiplicative) multiplied by  */
/*          the arithmetic derivative of d.                 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/build_primes_upto.h"

/* --------- Helper: integer power for seq_t ---------------- */
static seq_t pow_seq_t(seq_t base, uint64_t exp)
{
    seq_t res = 1;
    while (exp) {
        if (exp & 1) res *= base;
        base *= base;
        exp >>= 1;
    }
    return res;
}

/* --------- Recursive enumeration of divisors of n ----------- */
static void rec_divisors(int idx, int cnt,
                         const uint64_t *primes,
                         const uint64_t *max_exp,
                         int *cur_exp,
                         seq_t *sum_res)
{
    if (idx == cnt) {
        /* Compute ψ(n/d) where d has exponents cur_exp   */
        seq_t psi = 1;
        for (int i = 0; i < cnt; ++i) {
            uint64_t e_m = max_exp[i] - cur_exp[i];
            if (e_m > 0) {
                seq_t part = pow_seq_t((seq_t)primes[i], e_m - 1);
                part *= (seq_t)(primes[i] + 1);
                psi *= part;
            }
        }

        /* Compute arithmetic derivative of d */
        seq_t deriv = 0;
        for (int i = 0; i < cnt; ++i) {
            if (cur_exp[i] == 0) continue;
            /* term = a_i * product_{j} p_j^{cur_exp[j] - δ_{ij}} */
            seq_t term = (seq_t)cur_exp[i];
            for (int j = 0; j < cnt; ++j) {
                int e = cur_exp[j] - (j == i ? 1 : 0);
                if (e > 0) term *= pow_seq_t((seq_t)primes[j], e);
            }
            deriv += term;
        }

        *sum_res += psi * deriv;
        return;
    }

    /* Iterate exponent for prime idx */
    for (int e = 0; e <= max_exp[idx]; ++e) {
        cur_exp[idx] = e;
        rec_divisors(idx + 1, cnt, primes, max_exp, cur_exp, sum_res);
    }
}

/* --------- Main sequence function -------------------------- */
seq_t a(idx_t n)
{
    if (n < 1) return 0;           /* undefined below offset */

    /* Cache primes up to the largest n seen so far */
    static uint64_t *prime_cache = NULL;
    static uint64_t prime_limit = 0;
    static size_t prime_cnt = 0;
    if (n > prime_limit) {
        if (prime_cache != NULL) free(prime_cache);
        build_primes_upto(n, &prime_cache, &prime_cnt);
        prime_limit = n;
    }

    /* Factorize n into primes[] and max_exp[] */
    uint64_t primes[64];
    uint64_t max_exp[64];
    int cnt = 0;
    uint64_t x = n;
    for (size_t i = 0; i < prime_cnt && prime_cache[i] * prime_cache[i] <= x; ++i) {
        uint64_t p = prime_cache[i];
        if (x % p == 0) {
            int e = 0;
            while (x % p == 0) {
                x /= p;
                ++e;
            }
            primes[cnt] = p;
            max_exp[cnt] = e;
            ++cnt;
        }
    }
    if (x > 1) {
        /* x is a prime factor > sqrt(n) */
        primes[cnt] = x;
        max_exp[cnt] = 1;
        ++cnt;
    }

    /* Recursively sum over all divisors */
    int cur_exp[64] = {0};
    seq_t result = 0;
    rec_divisors(0, cnt, primes, max_exp, cur_exp, &result);

    return result;
}
