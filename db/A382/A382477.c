/*
 * A382477  If n = Product (p_j^k_j) then a(n) = -Sum ((-1)^k_j * k_j * p_j).
 * https://oeis.org/A382477
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = - Sum ((-1)^k_j * k_j * p_j) over prime powers p_j^k_j in
 *            n
 * Range:     i8 n <= 75, i16 n <= 75, i32 n <= 75, i64 n <= 75, i128 n <= 75,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Compute a(n) for A382477.  Uses trial division to factor n. */
seq_t a(idx_t n)
{
    /* Use a 64‑bit working variable for factorisation */
    uint64_t m = n;
    /* Accumulate the sum in a wider signed type to avoid overflow for
       very small seq_t implementations (int8_t, int16_t).  The final
       cast to seq_t is implementation‑defined but fine for values that
       fit in seq_t. */
    int64_t sum = 0;

    for (uint64_t p = 2; p * p <= m; ++p)
    {
        if (m % p == 0)
        {
            int32_t e = 0;
            while (m % p == 0)
            {
                m /= p;
                ++e;
            }
            int64_t term = (int64_t)e * (int64_t)p;          /* k * p   */
            if (e & 1)  sum -= term;   /* odd exponent: (-1)^e = -1 */
            else        sum += term;   /* even exponent: (-1)^e =  1 */
        }
    }

    /* Remaining prime factor, if any, has exponent 1 (odd) */
    if (m > 1)
    {
        sum -= (int64_t)m;  /* e = 1 ⇒ sign = -1 */
    }

    return (seq_t)(-sum);   /* a(n) = -sum(( -1)^k * k * p) */
}
