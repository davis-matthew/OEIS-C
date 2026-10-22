/*
 * A331112  Sum of the digits of the n-th prime number in balanced ternary.
 * https://oeis.org/A331112
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, base, easy
 * Formula:   a(n) = sum of digits of the n‑th prime in balanced ternary
 * Range:     i8 n <= 100, i16 n <= 100, i32 n <= 100, i64 n <= 100,
 *            i128 n <= 100, big any n
 * Helpers:   get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/get_prime.h"

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;  /* sequence is defined for n ≥ 1 */

    uint32_t p = get_prime(n);  /* n‑th prime (1‑based) */
    seq_t sum = 0;
    idx_t m = p;                /* work variable, 64‑bit for safety */

    while (m != 0)
    {
        idx_t r = m % 3;
        if (r == 0)
        {
            m /= 3;
        }
        else if (r == 1)
        {
            sum += 1;
            m = (m - 1) / 3;
        }
        else   /* r == 2 → digit = –1 */
        {
            sum -= 1;
            m = (m + 1) / 3;
        }
    }

    return sum;
}
