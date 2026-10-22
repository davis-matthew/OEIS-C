/*
 * A327302  One of the two successive approximations up to 5^n for the 5-adic
 *          integer sqrt(-9). This is the 1 (mod 5) case (except for n = 0).
 * https://oeis.org/A327302
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   For n ≥ 2, a(n) is the unique k in {a(n-1)+m*5^(n-1) : m=0..4}
 *            such that k^2 + 9 is divisible by 5^n. The function computes
 *            a(n) by successively lifting the solution modulo increasing
 *            powers of 5
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 13, u64 n <= 24, u128 n <= 24,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Multiply a and b modulo mod without overflow.
   Works for any unsigned integral type or for arbitrary‑precision
   integer classes that support the usual operators. */
static inline seq_t mul_mod(seq_t a, seq_t b, seq_t mod)
{
    seq_t result = 0;
    a %= mod;
    b %= mod;
    while (b != 0)
    {
        if (b & 1)
            result = (result + a) % mod;
        a = (a + a) % mod;
        b = b / 2;
    }
    return result;
}

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;
    if (n == 1)
        return (seq_t)1;

    seq_t a_prev = (seq_t)1;   /* a(1) */
    seq_t pow5   = (seq_t)1;   /* 5^(k-1), starts as 5^0 for k=2 */

    for (idx_t k = 2; k <= n; ++k)
    {
        pow5 *= (seq_t)5;                     /* pow5 = 5^(k-1) */
        seq_t mod = pow5 * (seq_t)5;         /* mod = 5^k */

        seq_t found = a_prev;                /* default candidate */

        for (unsigned m = 0; m < 5; ++m)
        {
            seq_t cand = a_prev + (seq_t)m * pow5;
            seq_t cand2 = mul_mod(cand, cand, mod);
            if (((cand2 + (seq_t)9) % mod) == 0)
            {
                found = cand;
                break;
            }
        }
        a_prev = found;
    }

    return a_prev;
}
