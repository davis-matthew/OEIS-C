/*
 * A317623  Number of distinct values of X*(3*X-1) mod n.
 * https://oeis.org/A317623
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, mult
 * Formula:   a(n) = Number of distinct values of X*(3*X-1) mod n. The
 *            function implements the multiplicative formula given in the OEIS
 *            description
 * Range:     u8 n <= 16, u16 n <= 2648, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

/* Helper: fast exponentiation for seq_t */
static inline seq_t pow_seq(idx_t base, idx_t exp)
{
    seq_t result = 1;
    seq_t b = (seq_t)base;
    while (exp)
    {
        if (exp & 1) result *= b;
        b *= b;
        exp >>= 1;
    }
    return result;
}

/* Main sequence function */
seq_t a(idx_t n)
{
    if (n == 1) return (seq_t)1;

    seq_t res = 1;
    idx_t remaining = n;

    for (idx_t p = 2; p * p <= remaining; ++p)
    {
        if (remaining % p != 0) continue;
        idx_t exp = 0;
        while (remaining % p == 0)
        {
            remaining /= p;
            ++exp;
        }

        seq_t term;
        if (p == 2)
        {
            /* a(2^e) = 2^(e-1) */
            term = pow_seq(2, exp - 1);
        }
        else if (p == 3)
        {
            /* a(3^e) = 3^e */
            term = pow_seq(3, exp);
        }
        else
        {
            /* a(p^e) = 1 + floor( p^(e+1) / (2p+2) ) for p >= 5 */
            seq_t numerator   = pow_seq(p, exp + 1);
            seq_t denom       = (seq_t)(2 * p + 2);
            seq_t floor_value = numerator / denom;
            term = 1 + floor_value;
        }
        res *= term;
    }

    /* If remaining > 1, it's a prime factor > sqrt(n) with exponent 1 */
    if (remaining > 1)
    {
        seq_t term;
        if (remaining == 2)
            term = 1;                 /* 2^1 -> 2^(1-1) = 1 */
        else if (remaining == 3)
            term = 3;                 /* 3^1 = 3 */
        else
        {
            seq_t numerator   = pow_seq(remaining, 2);   /* e+1 = 2 */
            seq_t denom       = (seq_t)(2 * remaining + 2);
            seq_t floor_value = numerator / denom;
            term = 1 + floor_value;
        }
        res *= term;
    }

    return res;
}
