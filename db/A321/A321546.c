/*
 * A321546  a(n) = Sum_{d|n} (-1)^(d-1)*d^7.
 * https://oeis.org/A321546
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, mult
 * Formula:   a(n) = Sum_{d|n} (-1)^(d-1) * d^7
 * Range:     i8 n <= 2, i16 n <= 4, i32 n <= 21, i64 n <= 511,
 *            i128 n <= 10000, big any n
 * Helpers:   pow_uint
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/pow_uint.h"

/* Integer square‑root: floor(sqrt(n)) using only integer arithmetic. */
static inline idx_t floor_sqrt_u64(idx_t n)
{
    idx_t r = 0;
    /* The loop fails only when (r+1) > n/(r+1), i.e. (r+1)^2 > n. */
    while (r + 1 <= n / (r + 1))
        ++r;
    return r;
}

seq_t a(idx_t n)
{
    /* The OEIS offset is 1, so a(1) is the first term. For n==0 we simply return 0. */
    if (n == 0)
        return 0;

    seq_t sum = 0;
    idx_t limit = floor_sqrt_u64(n);  /* largest divisor to check */

    for (idx_t d = 1; d <= limit; ++d) {
        if (n % d != 0)
            continue;

        idx_t e = n / d;  /* paired divisor */

        /* Process divisor d */
        seq_t d_pow   = pow_uint((uint64_t)d, (idx_t)7);
        seq_t sign_d  = (d & 1) ? 1 : -1;   /* (-1)^(d-1) */
        sum += sign_d * d_pow;

        /* If e is distinct, process it too */
        if (e != d) {
            seq_t e_pow   = pow_uint((uint64_t)e, (idx_t)7);
            seq_t sign_e  = (e & 1) ? 1 : -1;
            sum += sign_e * e_pow;
        }
    }

    return sum;
}
