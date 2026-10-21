/*
 * A289310  Let f be the multiplicative function satisfying f(p^k) = (1 +
 *          p*I)^k for any prime p and k > 0 (where I^2 = -1); a(n) = the real
 *          part of f(n).
 * https://oeis.org/A289310
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = Re(∏_{p^k||n} (1 + p·i)^k) (the real part of the product)
 * Range:     i8 n <= 95, i16 n <= 10000, i32 n <= 10000, i64 n <= 10000,
 *            i128 n <= 10000, big any n
 * Helpers:   factor_u64, mul_complex, power_complex
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include "../utils/factor_u64.h"
#include "../utils/mul_complex.h"
#include "../utils/power_complex.h"

seq_t a(idx_t n)
{
    if (n <= 1U) {
        return (seq_t)1;   /* a(1) = 1 */
    }

    /* factorise n into primes and exponents */
    size_t cnt = 0;
    uint64_t primes_arr[64];
    uint64_t exps_arr[64];
    factor_u64((uint64_t)n, primes_arr, exps_arr, &cnt);

    seq_t cur_r = (seq_t)1;
    seq_t cur_i = (seq_t)0;

    for (size_t idx = 0; idx < cnt; ++idx) {
        seq_t p      = (seq_t)primes_arr[idx];
        unsigned exp = (unsigned)exps_arr[idx];

        seq_t pk_r, pk_i;
        power_complex(p, exp, &pk_r, &pk_i);

        mul_complex(cur_r, cur_i, pk_r, pk_i, &cur_r, &cur_i);
    }

    return cur_r;   /* real part of f(n) */
}
