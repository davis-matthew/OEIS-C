/*
 * A244083  a(n) = numerator( n^n/(2*n)! ).
 * https://oeis.org/A244083
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac
 * Formula:   a(n) = numerator( n^n/(2*n)! )
 * Range:     u8 n <= 6, u16 n <= 10, u32 n <= 12, u64 n <= 18, u128 n <= 28,
 *            big any n
 * Helpers:   build_primes_upto, seq_mul, seq_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdbool.h>
#include <stdint.h>
#include "../utils/build_primes_upto.h"
#include "../utils/seq_mul.h"
#include "../utils/seq_pow.h"

static uint64_t *primes = NULL;
static size_t primes_cnt = 0;
static idx_t primes_limit = 0;

static void ensure_primes(idx_t lim)
{
    if (lim > primes_limit) {
        if (primes) free(primes);
        build_primes_upto(lim, &primes, &primes_cnt);
        primes_limit = lim;
    }
}

seq_t a(idx_t n)
{
    seq_t result = 1;

    /* Special case n = 0: 0^0/(0)! = 1 */
    if (n == 0) return result;

    idx_t lim = 2 * n;
    ensure_primes(lim);

    for (size_t idx = 0; idx < primes_cnt && (idx_t)primes[idx] <= lim; ++idx) {
        uint64_t p = primes[idx];

        /* exponent of p in n^n */
        int exp_num = 0;
        if ((idx_t)p <= n) {
            idx_t tmp = n;
            while (tmp % p == 0) {
                ++exp_num;
                tmp /= p;
            }
            exp_num *= (int)n;
        }

        /* exponent of p in (2n)! */
        int exp_den = 0;
        idx_t pp = p;
        while (pp <= lim) {
            exp_den += lim / pp;
            if (pp > lim / p) break;   /* avoid overflow */
            pp *= p;
        }

        int exp = exp_num > exp_den ? exp_num - exp_den : 0;
        if (exp > 0) {
            seq_t factor = seq_pow((seq_t)p, (idx_t)exp);
            result = seq_mul(result, factor);
        }
    }

    return result;
}
