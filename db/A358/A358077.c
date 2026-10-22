/*
 * A358077  Sum of the nonprime divisors of n whose divisor complement is
 *          squarefree.
 * https://oeis.org/A358077
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{d|n, nonprime d, n/d squarefree} d
 * Range:     u8 n <= 119, u16 n <= 9533, u32 n <= 9533, u64 n <= 9533,
 *            u128 n <= 9533, big any n
 * Helpers:   is_prime, is_squarefree
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>

/* Helpers from the utility library */
#include "../utils/is_prime.h"      /* bool is_prime(idx_t x); */
#include "../utils/is_squarefree.h"/* bool is_squarefree(seq_t x); */

seq_t a(idx_t n)
{
    seq_t sum = 0;

    for (idx_t i = 1; i * i <= n; ++i)
    {
        if (n % i == 0)
        {
            /* divisor i */
            idx_t d1 = i;
            if (!is_prime(d1))
            {
                idx_t comp = n / d1;
                if (is_squarefree((seq_t)comp))
                    sum += (seq_t)d1;
            }

            /* paired divisor n/i, avoid double counting when i == n/i */
            idx_t d2 = n / i;
            if (d2 != d1)
            {
                if (!is_prime(d2))
                {
                    idx_t comp = n / d2; /* this is i */
                    if (is_squarefree((seq_t)comp))
                        sum += (seq_t)d2;
                }
            }
        }
    }

    return sum;
}
