/*
 * A342540  a(n) = Sum_{k=1..n} phi(gcd(k, n))^(n-1).
 * https://oeis.org/A342540
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{d|n} phi(n/d) * phi(d)^(n-1)
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 10, u64 n <= 16, u128 n <= 28,
 *            big any n
 * Validated: standard, pass, 38 terms through n = 368 (2026-11-01)
 * Helpers:   ipow, totient
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* (OEIS A342540) */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include "../utils/ipow.h"
#include "../utils/totient.h"

/* Helper: safe multiplication for seq_t */
static inline seq_t seq_mul(seq_t a, seq_t b) {
    return a * b;
}

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

            seq_t phi_d  = totient(d);
            seq_t phi_nd = totient(d2);                 /* n/d */
            seq_t pow_d  = ipow(phi_d, exp);
            sum += seq_mul(pow_d, phi_nd);

            if (d2 != d) {
                seq_t phi_d2 = totient(d2);
                seq_t phi_nd2 = totient(d);            /* n/d2 */
                seq_t pow_d2 = ipow(phi_d2, exp);
                sum += seq_mul(pow_d2, phi_nd2);
            }
        }
    }
    return sum;
}
