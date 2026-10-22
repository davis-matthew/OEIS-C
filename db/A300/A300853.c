/*
 * A300853  L.g.f.: log(Product_{k>=1} (1 + x^(k^2))) = Sum_{n>=1} a(n)*x^n/n.
 * https://oeis.org/A300853
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, mult, easy
 * Formula:   not stated
 * Range:     i8 n <= 24, i16 n <= 1000, i32 n <= 1000, i64 n <= 1000,
 *            i128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A300853 – multiplicative sequence
 *
 *  a(n) = Π_{p^e‖n} a(p^e)
 *   a(2^e) = (1 - (-2)^(e+1)) / 3
 *   a(p^e) = (p^(2*⌊e/2+1⌋) - 1) / (p^2 - 1)   (p odd prime)
 *
 *  The function below factors n using trial division and applies the
 *  above formulas.  It works for any idx_t >= 1 and for any seq_t
 *  (int8_t, int16_t, …, __int128, or arbitrary‑precision).  No external
 *  helpers are required beyond the standard headers.
 */

#include <stdint.h>

/* Helper: exponentiation by squaring.  exp is small (≤ log₂(n)). */
static seq_t pow_seq(seq_t base, unsigned exp)
{
    seq_t result = 1;
    while (exp > 0)
    {
        if (exp & 1U)
            result *= base;
        base *= base;
        exp >>= 1;
    }
    return result;
}

seq_t a(idx_t n)
{
    if (n <= 1)
        return 1;   /* a(1) = 1 by definition */

    idx_t m = n;
    seq_t result = 1;

    /* factor 2 separately */
    if ((m & 1U) == 0U)  /* m even */
    {
        idx_t e = 0;
        while ((m & 1U) == 0U)
        {
            m >>= 1;     /* divide by 2 */
            ++e;
        }
        seq_t pow_neg2 = pow_seq((seq_t)-2, (unsigned)(e + 1));
        seq_t contrib   = (1 - pow_neg2) / 3;
        result *= contrib;
    }

    /* odd factors */
    for (idx_t p = 3; p <= m / p; p += 2)
    {
        if (m % p == 0)
        {
            idx_t e = 0;
            while (m % p == 0)
            {
                m /= p;
                ++e;
            }
            idx_t k = e / 2 + 1;                  /* floor(e/2)+1 */
            seq_t numer   = pow_seq((seq_t)p, (unsigned)(2 * k)) - 1;
            seq_t denom   = pow_seq((seq_t)p, 2) - 1;   /* p^2 - 1 */
            seq_t contrib = numer / denom;
            result *= contrib;
        }
    }

    /* if anything remains, it is a prime > sqrt(n) */
    if (m > 1)
    {
        idx_t p = m;     /* remaining prime factor with exponent 1 */
        idx_t e = 1;
        idx_t k = e / 2 + 1;                  /* floor(e/2)+1  = 1 */
        seq_t numer   = pow_seq((seq_t)p, (unsigned)(2 * k)) - 1;  /* p^2-1 */
        seq_t denom   = numer;                 /* same as numerator, so contrib = 1 */
        /* but keep formula for consistency */
        seq_t contrib = numer / denom;  /* equals 1 */
        result *= contrib;
    }

    return result;
}
