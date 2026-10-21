/*
 * A284711  Even bisection of A283848.
 * https://oeis.org/A284711
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (4*n)^(-1)*Sum_{d|2*n, d even} phi(d)*4^(2*n/d) +
 *            5*2^(2*n-2)
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 15, u64 n <= 31, u128 n <= 63,
 *            big any n
 * Helpers:   phi, pow2, pow4
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Offsets: a(2) is the first term. */

#include "../utils/phi.h"
#include "../utils/pow2.h"
#include "../utils/pow4.h"

seq_t a(idx_t n)
{
    if (n < 2) return 0;                /* undefined below offset */

    idx_t two_n = 2 * n;
    seq_t sum = 0;

    /* enumerate divisors of two_n */
    for (idx_t i = 1; i * i <= two_n; ++i) {
        if (two_n % i != 0) continue;

        idx_t d1 = i;
        idx_t d2 = two_n / i;

        if ((d1 & 1) == 0) {           /* d1 even */
            seq_t phi_val = phi(d1);
            idx_t exp = two_n / d1;     /* 2*n/d1 */
            seq_t pow_val = pow4(exp);  /* 4^exp */
            sum += phi_val * pow_val;
        }

        if (d1 == d2) continue;        /* square case handled above */

        if ((d2 & 1) == 0) {           /* d2 even */
            seq_t phi_val = phi(d2);
            idx_t exp = two_n / d2;     /* 2*n/d2 */
            seq_t pow_val = pow4(exp);
            sum += phi_val * pow_val;
        }
    }

    seq_t denom = (seq_t)4 * (seq_t)n;   /* 4*n */
    seq_t sum_div = sum / denom;        /* integer division */

    seq_t term2 = 5 * pow2(two_n - 2);   /* 5*2^(2*n-2) */

    return sum_div + term2;
}
