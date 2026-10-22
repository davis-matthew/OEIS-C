/*
 * A300826  a(n) = n/A125746(n), where A125746(n) gives the smallest divisor d
 *          of n such that the sum which includes d and all smaller divisors
 *          is >= n.
 * https://oeis.org/A300826
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 2365, u16 n <= 9376, u32 n <= 9376, u64 n <= 9376,
 *            u128 n <= 9376, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A300826: a(n) = n / A125746(n) */
/* A125746(n) is the smallest divisor d of n such that the sum of all divisors of n
   that are ≤ d is at least n.  a(n) is n divided by that divisor.  The routine
   factors n by trial division, generates all its divisors, sorts them, and
   accumulates the sum until the condition is satisfied.  It works for
   n up to at least 10 000 in well under a second. */

#include <stddef.h>

/* idx_t and seq_t are defined by the build system.  They must not be redeclared. */

seq_t a(idx_t n)
{
    if (n == 0) return 0;                 /* undefined for 0; return 0 safely */

    /* 1. Factor n into prime powers.  We store at most 16 distinct primes. */
    idx_t temp = n;
    idx_t primes[16];
    int   exps[16];
    int   pc = 0;

    for (idx_t p = 2; p * p <= temp; ++p)
    {
        if (temp % p == 0)
        {
            int e = 0;
            while (temp % p == 0)
            {
                temp /= p;
                ++e;
            }
            if (pc < 16)
            {
                primes[pc] = p;
                exps[pc]   = e;
                ++pc;
            }
        }
    }
    if (temp > 1)
    {
        if (pc < 16)
        {
            primes[pc] = temp;
            exps[pc]   = 1;
            ++pc;
        }
    }

    /* 2. Generate all divisors of n (by multiplicative construction). */
    idx_t divs[256];
    size_t div_cnt = 1;
    divs[0] = 1;

    for (int i = 0; i < pc; ++i)
    {
        idx_t p = primes[i];
        int   e = exps[i];
        size_t cur = div_cnt;
        idx_t mult = 1;

        for (int k = 1; k <= e; ++k)
        {
            mult *= p;
            for (size_t j = 0; j < cur; ++j)
            {
                divs[div_cnt++] = divs[j] * mult;
            }
        }
    }

    /* 3. Sort the divisors in ascending order (in‑place insertion sort). */
    for (size_t i = 1; i < div_cnt; ++i)
    {
        idx_t  val = divs[i];
        long   j = (long)i - 1;
        while (j >= 0 && divs[(size_t)j] > val)
        {
            divs[(size_t)j + 1] = divs[(size_t)j];
            --j;
        }
        divs[(size_t)j + 1] = val;
    }

    /* 4. Find the smallest divisor d such that the sum of all divisors ≤ d
       is at least n. */
    seq_t cum = 0;
    idx_t d   = 1;   /* default, will be overwritten when condition holds */

    for (size_t i = 0; i < div_cnt; ++i)
    {
        cum += (seq_t)divs[i];
        if (cum >= (seq_t)n)
        {
            d = divs[i];
            break;
        }
    }

    /* 5. Return a(n) = n / d. */
    return (seq_t)n / (seq_t)d;
}
