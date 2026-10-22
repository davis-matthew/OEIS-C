/*
 * A345047  a(n) = A003958(n) / A345046(n), where A003958(n) is multiplicative
 *          with a(p^e) = (p-1)^e, and A345046(n) gives the least common
 *          multiple of the same factors.
 * https://oeis.org/A345047
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A003958(n) / A345046(n)
 * Range:     u8 n <= 9143, u16 n <= 9143, u32 n <= 9143, u64 n <= 9143,
 *            u128 n <= 9143, big any n
 * Helpers:   factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A003958(n) = product over p^e||n of (p-1)^e
   A345046(n) = lcm of the same factors */
/* The ratio equals the product of all primes q that divide any (p-1)
   raised to (Σ e·v_q(p-1)  – max e·v_q(p-1)). */

#include <stdint.h>
#include <stddef.h>
#include "../utils/factor_u64.h"

seq_t a(idx_t n)
{
    /* n is an idx_t (uint64_t). For factor_u64 we need uint64_t */
    uint64_t n_val = (uint64_t)n;
    if (n_val <= 1) {          /* a(1) = 1 */
        return (seq_t)1;
    }

    /* Factorise n: primes and exponents */
    uint64_t primes_n[64];
    uint64_t exps_n[64];
    size_t cnt_n = 0;
    factor_u64(n_val, primes_n, exps_n, &cnt_n);

    /* For each distinct prime q that arises from p-1 we store the sum
       of exponents and the maximum exponent among all (p^e). */
    uint64_t q_pr[64];
    uint64_t q_sum[64];
    uint64_t q_max[64];
    size_t cnt_q = 0;

    for (size_t i = 0; i < cnt_n; ++i) {
        uint64_t p = primes_n[i];
        uint64_t e = exps_n[i];

        /* t = p-1, possibly 1 (for p==2) */
        uint64_t t = p - 1;
        if (t <= 1) {
            continue;    /* nothing to contribute */
        }

        /* factor t */
        uint64_t primes_t[64];
        uint64_t exps_t[64];
        size_t cnt_t = 0;
        factor_u64(t, primes_t, exps_t, &cnt_t);

        for (size_t j = 0; j < cnt_t; ++j) {
            uint64_t q = primes_t[j];
            uint64_t pe = exps_t[j];
            uint64_t exp = pe * e;   /* exponent in (p-1)^e */

            /* search for q in q_pr */
            size_t pos = 0;
            while (pos < cnt_q && q_pr[pos] != q) {
                ++pos;
            }
            if (pos == cnt_q) {          /* new prime */
                q_pr[cnt_q] = q;
                q_sum[cnt_q] = exp;
                q_max[cnt_q] = exp;
                ++cnt_q;
            } else {                     /* existing */
                q_sum[pos] += exp;
                if (exp > q_max[pos]) {
                    q_max[pos] = exp;
                }
            }
        }
    }

    /* Compute product of q^(sum - max) */
    seq_t result = (seq_t)1;
    for (size_t i = 0; i < cnt_q; ++i) {
        uint64_t surplus = q_sum[i] - q_max[i];
        for (uint64_t j = 0; j < surplus; ++j) {
            result *= (seq_t)q_pr[i];
        }
    }

    return result;
}
