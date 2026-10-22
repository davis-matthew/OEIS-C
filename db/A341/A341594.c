/*
 * A341594  Number of strictly superior odd divisors of n.
 * https://oeis.org/A341594
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 9750, u16 n <= 9750, u32 n <= 9750, u64 n <= 9750,
 *            u128 n <= 9750, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Count strictly superior odd divisors of n by enumerating all odd divisors
   of the odd part of n and testing the condition d > n/d. */

#include <stddef.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    /* offset a(1)=0, a(2)=0, ... */
    if (n <= 1)
        return (seq_t)0;

    /* 1. Extract the odd part of n. */
    idx_t odd = n;
    while ((odd & 1ULL) == 0)
        odd >>= 1;

    /* 2. Factor the odd part by simple trial division. */
    idx_t primes[64];
    int   exps[64];
    int   pc = 0;

    idx_t m = odd;
    for (idx_t p = 3; p <= m / p; p += 2)   /* p*p <= m, avoid overflow */
    {
        if (m % p == 0)
        {
            int e = 0;
            while (m % p == 0)
            {
                m   /= p;
                ++e;
            }
            primes[pc] = p;
            exps[pc]   = e;
            ++pc;
        }
    }
    if (m > 1)          /* remaining prime factor */
    {
        primes[pc] = m;
        exps[pc]   = 1;
        ++pc;
    }

    /* 3. Generate all odd divisors of n. */
    idx_t divs[2048];
    size_t divcnt = 1;          /* start with divisor 1 */
    divs[0] = 1ULL;

    for (int i = 0; i < pc; ++i)
    {
        idx_t p = primes[i];
        int   e = exps[i];

        idx_t power = 1;
        size_t oldcnt = divcnt;   /* snapshot before adding new divisors */

        for (int k = 1; k <= e; ++k)
        {
            power *= p;         /* p^k   */
            for (size_t j = 0; j < oldcnt; ++j)
                divs[divcnt++] = divs[j] * power;
        }
    }

    /* 4. Count odd divisors that are strictly superior. */
    seq_t count = 0;
    for (size_t i = 0; i < divcnt; ++i)
    {
        idx_t d = divs[i];
        if (d > n / d)                 /* strictly superior divisor */
            ++count;
    }

    return count;
}
