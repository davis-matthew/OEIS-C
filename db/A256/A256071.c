/*
 * A256071  Number of ordered ways to write n = p + x*(3*x-1)/2, where p is
 *          prime or zero, and x is an integer.
 * https://oeis.org/A256071
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) counts the number of ordered pairs (p , x) such that n = p
 *            + x*(3*x-1)/2, where p is a prime or zero and x is any integer
 * Range:     u8 n <= 100, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include "../utils/is_prime.h"

seq_t a(idx_t n)
{
    seq_t count = 0;

    /* x = 0 gives the generalized pentagonal number 0. */
    if (n == 0 || is_prime(n))
        count++;

    for (idx_t i = 1; ; ++i) {
        /* Positive and negative indices produce distinct pentagonal numbers. */
        seq_t g_pos = (seq_t)i * (((seq_t)3 * i) - 1) / 2;   /* x =  i */
        seq_t g_neg = (seq_t)i * (((seq_t)3 * i) + 1) / 2;   /* x = -i */

        bool continued = false;

        if (g_pos <= n) {
            idx_t p = n - (idx_t)g_pos;
            if (p == 0 || is_prime(p))
                count++;
            continued = true;
        }

        if (g_neg <= n) {
            idx_t p = n - (idx_t)g_neg;
            if (p == 0 || is_prime(p))
                count++;
            continued = true;
        }

        if (!continued) /* both g_pos and g_neg exceeded n */
            break;
    }

    return count;
}
