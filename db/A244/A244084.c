/*
 * A244084  a(n) = denominator( n^n/(2*n)! ).
 * https://oeis.org/A244084
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac
 * Formula:   a(n) = denominator( n^n / (2*n)! )
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 8, u64 n <= 12, u128 n <= 18,
 *            big any n
 * Helpers:   build_primes_upto
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/build_primes_upto.h"

seq_t a(idx_t n)
{
    if (n == 0) {
        return 1;                      /* 0^0 / 0! = 1 */
    }

    idx_t two_n = 2 * n;

    /* Generate primes up to 2*n */
    uint64_t *primes = NULL;
    size_t primes_cnt = 0;
    build_primes_upto(two_n, &primes, &primes_cnt);

    seq_t result = 1;

    for (size_t i = 0; i < primes_cnt; ++i) {
        uint64_t p = primes[i];

        /* exponent of p in (2n)! : Legendre's formula */
        seq_t exp_d = 0;
        idx_t m = two_n;
        while (m) {
            m /= p;
            exp_d += m;
        }

        /* exponent of p in n : multiplicity of p in n itself */
        seq_t vp = 0;
        idx_t temp = n;
        while (temp % p == 0) {
            vp++;
            temp /= p;
        }

        /* exponent of p in n^n */
        seq_t exp_n = n * vp;

        /* remaining exponent for the denominator */
        if (exp_d > exp_n) {
            idx_t diff = (idx_t)(exp_d - exp_n);
            seq_t mult = 1;
            seq_t sp   = (seq_t)p;       /* prime as seq_t */
            for (idx_t j = 0; j < diff; ++j) {
                mult *= sp;
            }
            result *= mult;
        }
    }

    free(primes);
    return result;
}
