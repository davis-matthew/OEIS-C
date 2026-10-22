/*
 * A361596  Expansion of e.g.f. exp( x^2/(2 * (1-x)^2) ) / (1-x).
 * https://oeis.org/A361596
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (3*n-2)*a(n-1) - (n-1)*(3*n-5)*a(n-2) +
 *            (n-2)^2*(n-1)*a(n-3); Initial values: a(0)=1, a(1)=1, a(2)=3
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 11, u64 n <= 18, u128 n <= 20,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0) return 1;
    if (n == 1) return 1;
    if (n == 2) return 3;

    seq_t a0 = 1; /* a(0) */
    seq_t a1 = 1; /* a(1) */
    seq_t a2 = 3; /* a(2) */

    for (idx_t i = 3; i <= n; ++i) {
        seq_t coeff1 = (seq_t)(3 * i - 2);
        seq_t coeff2 = (seq_t)((i - 1) * (3 * i - 5));
        seq_t coeff3 = (seq_t)((i - 2) * (i - 2) * (i - 1));

        seq_t next = a2 * coeff1;          /* (3*n-2)*a(n-1) */
        next += a0 * coeff3;               /* + (n-2)^2*(n-1)*a(n-3) */
        next -= a1 * coeff2;               /* - (n-1)*(3*n-5)*a(n-2) */

        a0 = a1;
        a1 = a2;
        a2 = next;
    }

    return a2;
}
