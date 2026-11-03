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
 * Validated: standard, pass, 79 terms through n = 9336 (2026-11-01)
 * Helpers:   factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/factor_u64.h"

/* multiply two complex numbers: (r1 + i1·I) * (r2 + i2·I)  →  (out_r + out_i·I) */
static void mul_complex(seq_t r1, seq_t i1, seq_t r2, seq_t i2,
                        seq_t *out_r, seq_t *out_i)
{
    seq_t tmp_r = r1 * r2 - i1 * i2;
    seq_t tmp_i = r1 * i2 + i1 * r2;
    *out_r = tmp_r;
    *out_i = tmp_i;
}

/* exponentiation by squaring for (1 + p·I)^k */
static void power_complex(seq_t p, unsigned k, seq_t *r_out, seq_t *i_out)
{
    seq_t base_r = (seq_t)1;
    seq_t base_i = p;
    seq_t res_r = (seq_t)1;
    seq_t res_i = (seq_t)0;

    while (k > 0) {
        if (k & 1U) {
            mul_complex(res_r, res_i, base_r, base_i, &res_r, &res_i);
        }
        mul_complex(base_r, base_i, base_r, base_i, &base_r, &base_i);
        k >>= 1U;
    }
    *r_out = res_r;
    *i_out = res_i;
}

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
