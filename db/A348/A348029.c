/*
 * A348029  a(n) = A003959(n) - sigma(n), where A003959 is multiplicative with
 *          a(p^e) = (p+1)^e and sigma is the sum of divisors.
 * https://oeis.org/A348029
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A003959(n) - sigma(n)
 * Range:     u8 n <= 48, u16 n <= 1535, u32 n <= 9579, u64 n <= 9579,
 *            u128 n <= 9579, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the multiplicative function A003959 and subtracts the sum of divisors σ(n). */

#include <stdint.h>
#include <stddef.h>

/* seq_t and idx_t are provided by the build system. */

/* Compute A003959(n) - σ(n).  Valid for n >= 1.  No external helpers are needed. */
seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;          /* undefined for n = 0, but return 0 */

    uint64_t rem = (uint64_t)n;   /* keep a 64‑bit copy for factorisation */
    seq_t f_val = (seq_t)1;       /* A003959(n) */
    seq_t sigma = (seq_t)1;       /* σ(n) */

    /* Trial division factorisation.  For n up to about 10 000 this is fast. */
    for (uint64_t p = 2; p * p <= rem; ++p)
    {
        if (rem % p == 0)
        {
            unsigned exp = 0;
            while (rem % p == 0)
            {
                rem /= p;
                ++exp;
            }

            /* Compute (p + 1)^exp for f_val. */
            seq_t base = (seq_t)p + (seq_t)1;
            seq_t power = (seq_t)1;
            for (unsigned j = 0; j < exp; ++j)
                power *= base;
            f_val *= power;

            /* Compute σ(p^exp) = (p^(exp+1) – 1)/(p – 1). */
            seq_t p_seq = (seq_t)p;
            seq_t pow_exp1 = (seq_t)1;
            for (unsigned j = 0; j < exp + 1; ++j)
                pow_exp1 *= p_seq;
            seq_t sigma_factor = (pow_exp1 - (seq_t)1) / (p_seq - (seq_t)1);
            sigma *= sigma_factor;
        }
    }

    /* If rem > 1, it is a prime with exponent 1. */
    if (rem > 1)
    {
        seq_t base = (seq_t)rem + (seq_t)1;
        f_val *= base;     /* (p + 1)^1 */
        sigma *= base;     /* σ(p) = p + 1 */
    }

    return f_val - sigma;
}
