/*
 * A299150  Denominators of the positive solution to n = Sum_{d|n} a(d) *
 *          a(n/d).
 * https://oeis.org/A299150
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, frac, mult
 * Formula:   a(n) = 2^exp(n) where exp(n) is computed from n’s prime
 *            factorization via the exact recurrence a(n) = 2^exp(n). For each
 *            prime power p^e, exp(p^e) is found by the recurrence a_e = (p^e
 *            - sum_{k=1}^{e-1} a_k a_{e-k})/2. The function below implements
 *            this recurrence for all e≥1 and returns the expo...
 * Range:     u8 n <= 242, u16 n <= 19682, u32 n <= 65537, u64 n <= 65537,
 *            u128 n <= 65537, big any n
 * Validated: standard, pass, 99 terms through n = 7599 (2026-11-01)
 * Helpers:   factor_u64, pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/factor_u64.h"
#include "../utils/pow2.h"

/* compute the exponent of the denominator for a single prime power p^e
   where a(1)=1.  The result is the exponent e_fac such that
   a(p^e) = something / 2^e_fac. */
static idx_t denom_exp_prime_power(uint64_t p, idx_t e)
{
    /* storage for numerators and exponents of the a(p^k) values.  e is
       small (≤13 for n≤10000), so a fixed 20‑element buffer is sufficient. */
    unsigned __int128 num[20];
    idx_t      exp[20];
    num[0] = 1;
    exp[0] = 0;

    unsigned __int128 p_pow = 1;          /* p^i */
    for (idx_t i = 1; i <= e; ++i) {
        p_pow *= (unsigned __int128)p;    /* compute p^i */

        unsigned __int128 s_num = 0;      /* accumulated numerator */
        idx_t s_exp = 0;                 /* shared exponent */

        for (idx_t k = 1; k < i; ++k) {
            idx_t  term_exp = exp[k] + exp[i - k];
            unsigned __int128 term_num = num[k] * num[i - k];

            if (s_exp < term_exp) {
                s_num <<= (term_exp - s_exp);   /* bring existing sum to higher exp */
                s_exp = term_exp;
            } else if (s_exp > term_exp) {
                term_num <<= (s_exp - term_exp); /* bring new term to existing exp */
            }
            s_num += term_num;
        }

        /* a_i = (p^i * 2^s_exp - s_num) / 2^(s_exp+1) */
        unsigned __int128 numerator = (p_pow << s_exp) - s_num;
        idx_t den_exp = s_exp + 1;

        /* reduce by cancelling powers of two from numerator */
        while ((numerator & 1) == 0 && den_exp > 0) {
            numerator >>= 1;
            den_exp--;
        }

        num[i] = numerator;
        exp[i] = den_exp;
    }
    return exp[e];
}

seq_t a(idx_t n)
{
    if (n <= 1) return 1;          /* a(1) = 1 */

    /* factor n into prime powers */
    uint64_t primes[64];
    uint64_t exps[64];
    size_t   cnt = 0;
    factor_u64((uint64_t)n, primes, exps, &cnt);

    /* sum the denominator exponents for each prime power */
    idx_t total_exp = 0;
    for (size_t i = 0; i < cnt; ++i) {
        total_exp += denom_exp_prime_power(primes[i], exps[i]);
    }
    return pow2(total_exp);
}
