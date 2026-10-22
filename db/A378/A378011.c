/*
 * A378011  a(n) = b(9*n+1), with the sequence {b(n)} having Dirichlet g.f.
 *          Product_{chi} L(chi,s), where chi runs through all Dirichlet
 *          characters modulo 9; 9th column of A378007.
 * https://oeis.org/A378011
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = b(9*n+1) where b is multiplicative with the rules
 *            described in the OEIS entry. This implementation uses a
 *            trial‑division factorisation of 9*n+1 and evaluates the
 *            multiplicative contributions directly. It does not rely on
 *            external utilities
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

/*--------------------------------------------------------------------*/
/* small binomial for k <= 5                                            */
static seq_t binomial_small(idx_t n, idx_t k)
{
    seq_t res = 1;
    for (idx_t i = 1; i <= k; ++i)
    {
        /* res = res * (n-k+i) / i; all operations are on seq_t.   */
        res = (res * (seq_t)(n - k + i)) / (seq_t)i;
    }
    return res;
}

/*--------------------------------------------------------------------*/
/* main sequence function                                               */
seq_t a(idx_t n)
{
    /* compute m = 9*n + 1, using an intermediate 128‑bit type to
       avoid overflow for modest sizes of n.                       */
    __uint128_t tmp128 = ( (__uint128_t)n ) * 9 + 1;
    if (tmp128 > UINT64_MAX)
        return (seq_t)0;            /* out of range – undefined behaviour */
    uint64_t m = (uint64_t)tmp128;

    /* if m == 1, the multiplicative identity gives 1. */
    if (m == 1)
        return (seq_t)1;

    seq_t result = 1;
    uint64_t d = 2;
    uint64_t rem = m;

    /* trial division up to sqrt(rem) */
    while (d * d <= rem)
    {
        if (rem % d == 0)
        {
            uint64_t exp = 0;
            while (rem % d == 0)
            {
                rem /= d;
                ++exp;
            }

            uint64_t mod = d % 9;
            seq_t term = 1;

            if (mod == 1)
            {
                term = binomial_small((idx_t)(exp + 5), 5);
            }
            else if (mod == 8)
            {
                if ((exp & 1) == 0)
                    term = binomial_small((idx_t)(exp / 2 + 2), 2);
                else
                    return (seq_t)0;
            }
            else if (mod == 4 || mod == 7)
            {
                if (exp % 3 == 0)
                    term = (seq_t)(exp / 3 + 1);
                else
                    return (seq_t)0;
            }
            else if (mod == 2 || mod == 5)
            {
                if (exp % 6 != 0)
                    return (seq_t)0;
                /* otherwise term stays 1 */
            }
            else
            {
                /* any other residue (0,3,6) should not occur for primes */
                return (seq_t)0;
            }

            result *= term;
        }
        ++d;
    }

    /* if anything remains, it is a prime factor > sqrt(m) */
    if (rem > 1)
    {
        uint64_t p = rem;
        uint64_t exp = 1;          /* exponent is 1 for the remaining prime */
        uint64_t mod = p % 9;
        seq_t term = 1;

        if (mod == 1)
        {
            term = binomial_small((idx_t)(exp + 5), 5);
        }
        else if (mod == 8)
        {
            if ((exp & 1) == 0)
                term = binomial_small((idx_t)(exp / 2 + 2), 2);
            else
                return (seq_t)0;
        }
        else if (mod == 4 || mod == 7)
        {
            if (exp % 3 == 0)
                term = (seq_t)(exp / 3 + 1);
            else
                return (seq_t)0;
        }
        else if (mod == 2 || mod == 5)
        {
            if (exp % 6 != 0)
                return (seq_t)0;
        }
        else
        {
            return (seq_t)0;
        }

        result *= term;
    }

    return result;
}
