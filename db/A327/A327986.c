/*
 * A327986  Denominators of the coefficients in the expansion of
 *          (4*sinh(sqrt(x)/2)^2*cosh(sqrt(x)))/x.
 * https://oeis.org/A327986
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac
 * Formula:   a(n) = denominator of the coefficient of x^n in *
 *            (cosh(2*sqrt(x)) - 2*cosh(sqrt(x)) + 1)/x * = denominator of
 *            (4^(n+1)-2)/(2n+2)!
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 5, u64 n <= 10, u128 n <= 14,
 *            big any n
 * Helpers:   build_primes, mod_pow, seq_mul, seq_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/build_primes.h"
#include "../utils/mod_pow.h"
#include "../utils/seq_mul.h"
#include "../utils/seq_pow.h"

seq_t a(idx_t n)
{
    /* Handle offset n = 0 */
    if (n == 0) return (seq_t)1;

    /* M = 2n + 2 is the argument of the factorial  (2n+2)! */
    idx_t M = 2 * n + 2;

    /* Generate all primes ≤ M */
    idx_t *primes = NULL;
    size_t prime_cnt = 0;
    build_primes(M, &primes, &prime_cnt);

    /* Initialize result = 1 (as seq_t) */
    seq_t denom = (seq_t)1;

    /* Exponent of 2 in (4^(n+1)-2) is exactly 1 for n≥0 */
    for (size_t i = 0; i < prime_cnt; ++i) {
        idx_t p = primes[i];

        /* Exponent of p in (2n+2)! (Legendre's formula) */
        idx_t v_fact = 0;
        idx_t tmp = M;
        while (tmp) {
            tmp /= p;
            v_fact += tmp;
        }

        /* Exponent of p in numerator N = 4^(n+1) - 2 = 2 * (2^(2n+1)-1) */
        idx_t v_num = 0;
        if (p == 2) {
            v_num = 1;
        } else {
            /* Check if p divides 2^(2n+1)-1 */
            seq_t modp = (seq_t)p;
            seq_t rem = mod_pow((seq_t)2, (idx_t)(2 * n + 1), modp);
            if (rem == 1) {
                /* Find largest k such that p^k | (2^(2n+1)-1) */
                idx_t k = 1;
                seq_t pk = modp;            /* p^k */
                while (1) {
                    seq_t r = mod_pow((seq_t)2, (idx_t)(2 * n + 1), pk);
                    if (r != 1) break;
                    ++k;
                    pk = seq_mul(pk, modp); /* pk *= p   */
                }
                v_num = k - 1;              /* success count */
            }
        }

        /* Remaining exponent to keep in the denominator */
        if (v_fact > v_num) {
            idx_t exp = v_fact - v_num;
            seq_t part = seq_pow((seq_t)p, exp);
            denom = seq_mul(denom, part);
        }
    }

    free(primes);
    return denom;
}
