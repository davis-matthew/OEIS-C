/*
 * A362292  a(n) = (n+1/3)^n * (3*n)!/n!.
 * https://oeis.org/A362292
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = ((3*n+1)/3)^n * (3*n)! / n!
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 3, u64 n <= 6, u128 n <= 10,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    /* Product (n+1)*(n+2)*...*(3n) divided by 3^cnt3, where
       cnt3 = v3((3n)!)-v3(n!), the number of 3‑factors in (3n)!/n! */
    idx_t start = n + 1;
    idx_t limit = 3 * n;
    seq_t result = 1;

    /* Count 3‑factors in (3n)!/n! */
    idx_t cnt3 = 0;
    {
        idx_t t = limit;
        while (t) { t /= 3; cnt3 += t; }
        t = n;
        while (t) { t /= 3; cnt3 -= t; }
    }

    /* Multiply and cancel 3‑factors on the fly. */
    for (idx_t k = start; k <= limit; ++k)
    {
        result *= (seq_t)k;
        while (cnt3 && (result % 3) == 0)
        {
            result /= 3;
            --cnt3;
        }
    }

    /* Multiply by (3*n+1)^n. */
    seq_t base  = (seq_t)(3 * n + 1);
    seq_t power = big_pow(base, (seq_t)n);
    result *= power;

    return result;
}
