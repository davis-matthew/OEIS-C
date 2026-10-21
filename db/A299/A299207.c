/*
 * A299207  Length of longest run of primes in any of the four diagonals
 *          starting from the center of a square number spiral with prime(n)
 *          at the center.
 * https://oeis.org/A299207
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 7112, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   get_prime, is_prime_small
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * a(n) = length of longest run of primes in any of the four diagonals 
 * of the square number spiral whose center value is prime(n). 
 * The run counts primes strictly beyond the center (the center itself is not counted). 
 */

#include <stdbool.h>
#include <stdint.h>
#include "../utils/get_prime.h"
#include "../utils/is_prime_small.h"

seq_t a(idx_t n)
{
    /* Get the nth prime and compute the shift of the spiral */
    uint32_t p = get_prime(n);               /* p = prime(n) , p >= 2 for n >= 1 */
    seq_t shift = (seq_t)p - 1;               /* shift so that center = p */

    seq_t max_run = 0;                       /* best run among the four diagonals */

    /* Loop over the four diagonals */
    for (int dir = 0; dir < 4; ++dir)
    {
        seq_t run = 0;
        idx_t k = 1;                         /* layer index (starting at 1) */
        while (true)
        {
            /* Base value (2k+1)^2 */
            seq_t two_k_plus1 = (seq_t)(2 * (int)k + 1);
            seq_t base = two_k_plus1 * two_k_plus1;

            seq_t value;
            switch (dir)
            {
                /* Diagonal 0: top–right ( (2k+1)^2 ) */
                case 0:
                    value = base + shift;
                    break;
                /* Diagonal 1: top–left ( (2k+1)^2 - 2k ) */
                case 1:
                    value = base - (seq_t)(2 * k) + shift;
                    break;
                /* Diagonal 2: bottom–left ( (2k+1)^2 - 4k ) */
                case 2:
                    value = base - (seq_t)(4 * k) + shift;
                    break;
                /* Diagonal 3: bottom–right ( (2k+1)^2 - 6k ) */
                default: /* case 3 */
                    value = base - (seq_t)(6 * k) + shift;
                    break;
            }

            /* If the current value is prime, extend the run; otherwise stop */
            if (is_prime_small(value))
            {
                ++run;
                ++k;
            }
            else
            {
                break;
            }
        }

        if (run > max_run)
            max_run = run;
    }

    return max_run;
}
