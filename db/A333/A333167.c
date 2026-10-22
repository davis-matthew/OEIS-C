/*
 * A333167  a(n) = r_2(n^2 + 1), where r_2(k) is the number of ways of writing
 *          k as a sum of 2 squares (A004018).
 * https://oeis.org/A333167
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = r_2(n^2 + 1)
 * Range:     u8 n <= 15, u16 n <= 310, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

/* Count representations of m as a sum of two squares.
   Uses the prime‑factorisation formula. */
static seq_t r2(seq_t m)
{
    /* work with a 64‑bit integer for intermediate arithmetic */
    unsigned long long val = (unsigned long long)m;
    unsigned long long result = 1;
    unsigned long long t = val;
    unsigned long long p = 2;

    for (; p * p <= t; ++p)
    {
        if (t % p == 0)
        {
            unsigned int exp = 0;
            while (t % p == 0)
            {
                t /= p;
                ++exp;
            }

            if (p % 4 == 3)
            {
                /* odd exponent of a prime 3 (mod 4) annihilates all
                   representations */
                if (exp & 1U)
                    return (seq_t)0;
            }
            else if (p % 4 == 1)
            {
                result *= (exp + 1);
            }
        }
    }

    /* remaining prime factor (if any) */
    if (t > 1)
    {
        if (t % 4 == 3)
            return (seq_t)0;
        if (t % 4 == 1)
            result *= 2;
    }

    result *= 4;   /* fundamental factor for 2‑square representations */
    return (seq_t)result;
}

seq_t a(idx_t n)
{
    seq_t m = (seq_t)n * (seq_t)n + 1;
    return r2(m);
}
