/*
 * A253629  Multiplicative function defined for prime powers by a(p^e) =
 *          p^(e-1)(p+1) if p > 2 and a(2^e) = 2^(e-1).
 * https://oeis.org/A253629
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, mult
 * Formula:   a(n) = ∏_{p^e‖n} f(p^e) where f(2^e) = 2^(e-1) (prime 2) f(p^e)
 *            = p^(e-1)(p+1) (prime p > 2)
 * Range:     u8 n <= 164, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>

/* --- Small power function --------------------------------------------- */
static inline seq_t seq_pow_seq(seq_t base, idx_t exp)
{
    seq_t result = (seq_t)1;
    while (exp > 0)
    {
        if (exp & (idx_t)1)
            result *= base;
        base *= base;
        exp >>= (idx_t)1;
    }
    return result;
}

/* --- Main sequence function -------------------------------------------- */
seq_t a(idx_t n)
{
    /* a(1) = 1 by definition */
    if (n <= (idx_t)1)
        return (seq_t)1;

    seq_t result = (seq_t)1;
    idx_t m = n;

    /* Trial division for all prime factors up to sqrt(m) */
    for (idx_t p = (idx_t)2; p <= m / p; ++p)
    {
        if (m % p != (idx_t)0)
            continue;

        idx_t exp = (idx_t)0;
        while (m % p == (idx_t)0)
        {
            m /= p;
            ++exp;
        }

        seq_t pow_base = seq_pow_seq((seq_t)p, exp - (idx_t)1);
        seq_t term;

        if (p == (idx_t)2)
            term = pow_base;                     /* f(2^e) = 2^(e-1)          */
        else
            term = pow_base * ((seq_t)(p + (idx_t)1));  /* f(p^e) = p^(e-1)(p+1) */

        result *= term;
    }

    /* Remaining prime factor larger than sqrt(original n) */
    if (m > (idx_t)1)
    {
        /* exponent is 1 for a remaining prime factor */
        seq_t pow_base = seq_pow_seq((seq_t)m, (idx_t)0); /* p^0 = 1 */
        seq_t term;

        if (m == (idx_t)2)
            term = pow_base;                     /* f(2) = 2^0 = 1           */
        else
            term = pow_base * ((seq_t)(m + (idx_t)1));

        result *= term;
    }

    return result;
}
