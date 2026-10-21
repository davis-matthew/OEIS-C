/*
 * A246702  The number of positive k < (2n-1)^2 such that (2^k - 1)/(2n - 1)^2
 *          is an integer.
 * https://oeis.org/A246702
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = ⌊ (4·n·(n-1)) / A002326(2·n·(n-1)) ⌋ Equivalent: a(n) = ⌊
 *            ((2·n−1)²−1) / ord_m ⌋, where ord_m is the multiplicative order
 *            of 2 modulo m = (2·n−1)²
 * Range:     u8 n <= 8, u16 n <= 236, u32 n <= 1560, u64 n <= 1560,
 *            u128 n <= 1560, big any n
 * Helpers:   factor_u64, gcd, mod_pow, pow_uint
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/factor_u64.h"
#include "../utils/mod_pow.h"
#include "../utils/pow_uint.h"
#include "../utils/gcd.h"

seq_t a(idx_t n)
{
    if (n <= 1) return 0;                /* a(1) = 0 */

    seq_t base = (seq_t)(2 * n - 1);      /* 2·n−1 */
    seq_t m = base * base;                /* m = (2·n−1)² */
    seq_t m_minus1 = m - 1;               /* numerator = (2·n−1)²−1 */

    /* Factor m into primes and exponents */
    uint64_t primes_m[64];
    uint64_t exps_m[64];
    size_t cnt_m;
    factor_u64((uint64_t)m, primes_m, exps_m, &cnt_m);

    seq_t lcm_val = 1;                    /* will accumulate lcm of orders */

    for (size_t i = 0; i < cnt_m; ++i) {
        uint64_t p  = primes_m[i];
        uint64_t e  = exps_m[i];
        seq_t mod_pe = pow_uint(p, e);     /* p^e  */

        /* Euler's totient of p^e: (p−1)·p^(e−1) */
        seq_t totient = (seq_t)(p - 1) * pow_uint(p, e - 1);

        /* Start with the full totient as the candidate order */
        seq_t order = totient;

        /* factor the totient so we can try to reduce */
        uint64_t primes_ord[64];
        uint64_t exps_ord[64];
        size_t cnt_ord;
        factor_u64((uint64_t)order, primes_ord, exps_ord, &cnt_ord);

        for (size_t j = 0; j < cnt_ord; ++j) {
            uint64_t q = primes_ord[j];
            while ((order % q) == 0) {
                seq_t cand = order / q;
                if (mod_pow((seq_t)2, (uint64_t)cand, mod_pe) == (seq_t)1) {
                    order = cand;          /* we can reduce */
                } else {
                    break;                 /* can't reduce by q any more */
                }
            }
        }

        /* lcm of all per‑prime‑power orders */
        seq_t g = gcd(order, lcm_val);
        lcm_val = (lcm_val / g) * order;
    }

    /* Result = floor( (m−1) / ord_m ) */
    seq_t res = m_minus1 / lcm_val;
    return res;
}
