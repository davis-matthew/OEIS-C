/*
 * A342539  a(n) = Sum_{k=1..n} phi(gcd(k, n))^n.
 * https://oeis.org/A342539
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{d|n} phi(n/d) * phi(d)^n, n ≥ 1 (provides the OEIS
 *            sequence A342539)
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 10, u64 n <= 16, u128 n <= 28,
 *            big any n
 * Validated: standard, pass, 38 terms through n = 342 (2026-11-01)
 * Helpers:   ipow, totient_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/ipow.h"
#include "../utils/totient_seq.h"

seq_t a(idx_t n)
{
    if (n == 0)      /* offset starts at 1; guard against misuse   */
        return (seq_t)0;

    seq_t result = (seq_t)0;
    idx_t limit = n / 1;   /* used in the loop condition to avoid overflow */

    for (idx_t d = 1; d <= n / d; ++d) {
        if (n % d == 0) {
            idx_t d1 = d;
            idx_t d2 = n / d;

            seq_t phi_d1 = totient_seq((seq_t)d1);   /* φ(d1)   */
            seq_t phi_d2 = totient_seq((seq_t)d2);   /* φ(d2)   */

            seq_t pow_d1 = ipow(phi_d1, (seq_t)n);   /* φ(d1)^n */
            seq_t term1  = phi_d2 * pow_d1;            /* φ(d2)*φ(d1)^n */

            result += term1;

            if (d1 != d2) {
                seq_t pow_d2 = ipow(phi_d2, (seq_t)n);   /* φ(d2)^n */
                seq_t term2  = phi_d1 * pow_d2;            /* φ(d1)*φ(d2)^n */

                result += term2;
            }
        }
    }

    return result;
}
