/*
 * A370201  a(n) = denominator((n!)^2/(2*(n-2)!*n^n)).
 * https://oeis.org/A370201
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, frac
 * Formula:   a(n) = denominator((n!)^2/(2*(n-2)!*n^n))
 * Range:     u8 n <= 6, u16 n <= 8, u32 n <= 12, u64 n <= 18, u128 n <= 22,
 *            big any n
 * Helpers:   build_primes_upto, seq_mul, seq_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/build_primes_upto.h"
#include "../utils/seq_pow.h"
#include "../utils/seq_mul.h"

static uint64_t *primes = NULL;
static size_t primes_cnt = 0;
static uint64_t primes_limit = 0;

seq_t a(idx_t n)
{
    if (n < 2) return 1; /* offset starts at 2 */

    /* build primes up to n if needed */
    if (n > primes_limit) {
        free(primes);
        build_primes_upto(n, &primes, &primes_cnt);
        primes_limit = n;
    }

    seq_t result = 1; /* identity for multiplication */

    for (size_t i = 0; i < primes_cnt && primes[i] <= n; ++i) {
        uint64_t p = primes[i];

        /* exponent of p in n */
        int exp_n = 0;
        idx_t temp = n;
        while (temp % p == 0) {
            ++exp_n;
            temp /= p;
        }

        /* exponent of p in n-1 */
        int exp_nm1 = 0;
        temp = n - 1;
        while (temp % p == 0) {
            ++exp_nm1;
            temp /= p;
        }

        /* exponent of p in (n-2)! (Legendre's formula) */
        int exp_fac = 0;
        idx_t m = n - 2;
        while (m > 0) {
            exp_fac += m / p;
            m /= p;
        }

        /* exponents in numerator and denominator */
        int e_D = (int)((idx_t)(n - 2) * exp_n) + (p == 2 ? 1 : 0);
        int e_N = exp_fac + 2 * exp_nm1;

        int e_res = e_D - e_N;
        if (e_res > 0) {
            seq_t pow_val = seq_pow((seq_t)p, (idx_t)e_res);
            result = seq_mul(result, pow_val);
        }
    }

    return result;
}
