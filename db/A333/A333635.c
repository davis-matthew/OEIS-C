/*
 * A333635  Numbers m such that m^2 + 1 has at most 2 prime factors.
 * https://oeis.org/A333635
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = nth m such that m^2 + 1 has at most 2 prime factors
 * Range:     u8 n <= 14, u16 n <= 203, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   count_prime_factors
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* (i.e. m^2 + 1 is prime or a product of two distinct primes) */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/count_prime_factors.h"

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;                 /* offset is 1; behavior for n < 1 is irrelevant */

    idx_t found = 0;               /* number of terms found so far */
    seq_t m = 0;                   /* current candidate m */

    while (true)
    {
        m++;                        /* try next integer */

        /* compute v = m^2 + 1 in type seq_t */
        seq_t v = (seq_t)m * (seq_t)m + (seq_t)1;

        /* count how many prime factors v has (with multiplicity) */
        if (count_prime_factors(v) <= 2U)
        {
            found++;
            if (found == n)
                return m;         /* return the nth term */
        }
    }
}
