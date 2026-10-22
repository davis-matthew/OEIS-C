/*
 * A328641  Dirichlet g.f.: zeta(s)^2 / zeta(s-1)^2.
 * https://oeis.org/A328641
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, mult
 * Formula:   a(n) is multiplicative. For a prime power p^e a(p) = 2*(1 - p)
 *            a(p^e) = (p-1)*(e*p - p - e - 1) (e > 1)
 * Range:     i8 n <= 50, i16 n <= 4388, i32 n <= 10000, i64 n <= 10000,
 *            i128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>

seq_t a(idx_t n)
{
    if (n == 1u)
        return 1;                      /* a(1)=1 */

    idx_t temp = n;
    seq_t result = 1;

    /* handle factor 2 */
    idx_t e = 0;
    while ((temp & 1u) == 0u) {
        temp >>= 1;
        ++e;
    }
    if (e > 0) {
        seq_t p = 2;
        seq_t part;
        if (e == 1) {
            part = (seq_t)2 * ((seq_t)1 - p);
        } else {
            seq_t term1 = p - 1;
            seq_t term2 = (seq_t)e * p - p - e - 1;
            part = term1 * term2;
        }
        result *= part;
    }

    /* handle odd factors */
    for (idx_t d = 3; d <= temp / d; d += 2) {
        if (temp % d == 0) {
            e = 0;
            while (temp % d == 0) {
                temp /= d;
                ++e;
            }
            seq_t p = (seq_t)d;
            seq_t part;
            if (e == 1) {
                part = (seq_t)2 * ((seq_t)1 - p);
            } else {
                seq_t term1 = p - 1;
                seq_t term2 = (seq_t)e * p - p - e - 1;
                part = term1 * term2;
            }
            result *= part;
        }
    }

    /* any remaining prime factor > sqrt(original n) */
    if (temp > 1) {
        seq_t p = (seq_t)temp;
        seq_t part = (seq_t)2 * ((seq_t)1 - p);  /* e = 1 */
        result *= part;
    }

    return result;
}
