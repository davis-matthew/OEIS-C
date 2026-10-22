/*
 * A342540  a(n) = Sum_{k=1..n} phi(gcd(k, n))^(n-1).
 * https://oeis.org/A342540
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{d|n} phi(n/d) * phi(d)^(n-1)
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 10, u64 n <= 16, u128 n <= 28,
 *            big any n
 * Helpers:   big_pow, phi, seq_mul
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* (OEIS A342540) */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/phi.h"       /* seq_t phi(idx_t m); */
#include "../utils/big_pow.h"  /* seq_t big_pow(seq_t base, seq_t exp); */
#include "../utils/seq_mul.h"  /* seq_t seq_mul(seq_t a, seq_t b); */

seq_t a(idx_t n)
{
    /* n >= 1 by OEIS offset; undefined for n == 0, return 0 */
    if (n == 0)
        return 0;

    seq_t sum = 0;
    seq_t exp = (seq_t)(n - 1);

    for (idx_t i = 1; (idx_t)i * i <= n; ++i) {
        if (n % i == 0) {
            idx_t d  = i;
            idx_t d2 = n / i;

            seq_t phi_d  = phi(d);
            seq_t phi_nd = phi(d2);                 /* n/d */
            seq_t pow_d  = big_pow(phi_d, exp);
            sum += seq_mul(pow_d, phi_nd);

            if (d2 != d) {
                seq_t phi_d2 = phi(d2);
                seq_t phi_nd2 = phi(d);            /* n/d2 */
                seq_t pow_d2 = big_pow(phi_d2, exp);
                sum += seq_mul(pow_d2, phi_nd2);
            }
        }
    }
    return sum;
}
