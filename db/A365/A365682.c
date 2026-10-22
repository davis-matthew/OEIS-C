/*
 * A365682  The sum of exponentially squarefree divisors of n.
 * https://oeis.org/A365682
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, mult
 * Formula:   a(n) = product_{p^e || n} (1 + sum_{k=1..e, k squarefree} p^k)
 * Range:     u8 n <= 107, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>

/* fast exponentiation for seq_t (seq_t is an unsigned integer type) */
static seq_t pow_uint(uint64_t base, idx_t exp)
{
    seq_t result = 1;
    seq_t cur = base;
    while (exp)
    {
        if (exp & 1) result *= cur;
        cur *= cur;
        exp >>= 1;
    }
    return result;
}

/* pre‑computed array telling whether a number <=64 is squarefree
   (1 true, 0 false). */
static bool squarefree[65];
static bool precomputed = false;

/* simple trial division factorisation and computation of a(n). */
seq_t a(idx_t n)
{
    if (!precomputed)
    {
        /* compute truth table for numbers 1..64 */
        for (int i = 1; i <= 64; ++i)
        {
            bool sf = true;
            for (int d = 2; d * d <= i; ++d)
                if (i % (d * d) == 0)
                {
                    sf = false;
                    break;
                }
            squarefree[i] = sf;
        }
        precomputed = true;
    }

    if (n == 0)  /* undefined, return 0 as a safe fallback */
        return 0;

    seq_t result = 1;
    idx_t temp = n;

    /* handle factor 2 */
    if (temp % 2 == 0)
    {
        int exp = 0;
        while ((temp & 1) == 0)
        {
            ++exp;
            temp >>= 1;
        }
        seq_t sum = 1;  /* accounts for k = 0 */
        for (int k = 1; k <= exp; ++k)
            if (squarefree[k]) sum += pow_uint(2, k);
        result *= sum;
    }

    /* odd factors */
    for (idx_t p = 3; p * p <= temp; p += 2)
    {
        if (temp % p == 0)
        {
            int exp = 0;
            while (temp % p == 0)
            {
                ++exp;
                temp /= p;
            }
            seq_t sum = 1;
            for (int k = 1; k <= exp; ++k)
                if (squarefree[k]) sum += pow_uint(p, k);
            result *= sum;
        }
    }

    /* remaining prime factor (if any) */
    if (temp > 1)
    {
        /* exponent is 1, always squarefree */
        seq_t sum = 1 + pow_uint(temp, 1);
        result *= sum;
    }

    return result;
}
