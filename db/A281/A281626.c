/*
 * A281626  a(n) = (sum of trivial divisors of n) - (sum of nontrivial
 *          divisors of n).
 * https://oeis.org/A281626
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, easy, changed
 * Formula:   a(n) = 2*(n+1) - sigma(n), with a(1) = 1
 * Range:     i8 n <= 126, i16 n <= 10000, i32 n <= 10000, i64 n <= 10000,
 *            i128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* where sigma(n) is the sum of all positive divisors of n. */

#include <stdint.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    /* Special‑case n = 1, because the general formula gives 3. */
    if (n == (idx_t)1)
        return (seq_t)1;

    uint64_t temp = (uint64_t)n;      /* Working copy of n for factorisation */
    seq_t sigma = (seq_t)1;           /* Product of (p^{e+1}-1)/(p-1) for all prime powers */

    uint64_t i = 2;
    /* Trial division up to sqrt(temp) */
    while ((unsigned __int128)i * i <= temp)
    {
        if (temp % i == 0)
        {
            seq_t part = (seq_t)1;    /* Sum of powers of this prime: 1 + p + … + p^e */
            seq_t power = (seq_t)1;   /* Current power p^k */
            while (temp % i == 0)
            {
                temp /= i;
                power = power * (seq_t)i;   /* Advance p^k */
                part = part + power;        /* Add to the sum */
            }
            sigma = sigma * part;            /* Include this prime factor */
        }
        i++;
    }

    /* If a prime factor larger than sqrt(n) remains */
    if (temp > 1)
        sigma = sigma * ((seq_t)temp + (seq_t)1);

    /* Final formula: 2*(n+1) - sigma(n) */
    seq_t result = (seq_t)2 * ((seq_t)n + (seq_t)1) - sigma;
    return result;
}
