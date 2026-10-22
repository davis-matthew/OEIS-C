/*
 * A364011  Expansion of Sum_{k>0} x^k / (1 + x^(3*k)).
 * https://oeis.org/A364011
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = -∑_{d|n, d≡1 (mod 3)} (-1)^d
 * Range:     i8 n <= 3244, i16 n <= 10000, i32 n <= 10000, i64 n <= 10000,
 *            i128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implements the sequence by factoring n into primes and enumerating
   its divisors with a simple depth‑first recursion. */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

static void gen_divisors(int idx, int num, seq_t cur, const idx_t pf[],
                         const int exp[], seq_t *sum)
{
    if (idx == num)
    {
        if (cur % 3 == 1)
        {
            /* (-1)^cur: even divisor gives +1, odd gives -1 */
            seq_t term = ((cur & 1) == 0) ? 1 : -1;
            *sum += term;
        }
        return;
    }

    idx_t prime = pf[idx];
    seq_t val = cur;
    for (int e = 0; e <= exp[idx]; ++e)
    {
        gen_divisors(idx + 1, num, val, pf, exp, sum);
        if (e == exp[idx]) break;
        val *= (seq_t)prime;
    }
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* undefined, but safe guard */

    /* Factor n */
    idx_t pf[64];                  /* space for distinct primes */
    int exp[64];
    int cnt = 0;
    idx_t temp = n;

    if ((temp & 1) == 0)           /* handle factor 2 */
    {
        int e = 0;
        while ((temp & 1) == 0)
        {
            temp >>= 1;
            ++e;
        }
        pf[cnt] = 2;
        exp[cnt] = e;
        ++cnt;
    }

    for (idx_t p = 3; p * p <= temp; p += 2)
    {
        if (temp % p == 0)
        {
            int e = 0;
            while (temp % p == 0)
            {
                temp /= p;
                ++e;
            }
            pf[cnt] = p;
            exp[cnt] = e;
            ++cnt;
        }
    }

    if (temp > 1)                  /* remaining prime factor */
    {
        pf[cnt] = temp;
        exp[cnt] = 1;
        ++cnt;
    }

    seq_t sum = 0;
    gen_divisors(0, cnt, 1, pf, exp, &sum);

    return -sum;
}
