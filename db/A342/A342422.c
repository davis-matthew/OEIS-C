/*
 * A342422  a(n) = Sum_{k=1..n} (n/gcd(k,n))^gcd(k,n).
 * https://oeis.org/A342422
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=1..n} (n / gcd(k, n))^(gcd(k, n)) Equivalent: a(n)
 *            = Sum_{d|n} φ(d) * d^(n/d)
 * Range:     u8 n <= 11, u16 n <= 29, u32 n <= 59, u64 n <= 119,
 *            u128 n <= 242, big any n
 * Validated: standard, pass, 62 terms through n = 4734 (2026-11-01)
 * Helpers:   ipow, totient_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Required utility functions */
#include <stdint.h>
#include <stdbool.h>
#include "../utils/ipow.h"
#include "../utils/totient_seq.h"

seq_t a(idx_t n)
{
    seq_t sum = 0;
    for (idx_t i = 1; i * i <= n; ++i) {
        if (n % i == 0) {
            /* divisor d1 = i */
            idx_t d1 = i;
            seq_t d1_t = (seq_t)d1;
            seq_t term1 = totient_seq(d1_t) * ipow(d1_t, (seq_t)(n / d1));
            sum += term1;

            /* divisor d2 = n / i, if distinct from d1 */
            idx_t d2 = n / i;
            if (d2 != d1) {
                seq_t d2_t = (seq_t)d2;
                seq_t term2 = totient_seq(d2_t) * ipow(d2_t, (seq_t)(n / d2));
                sum += term2;
            }
        }
    }
    return sum;
}
