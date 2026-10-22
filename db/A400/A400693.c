/*
 * A400693  Sum of cubes of A069268: a(n) = Sum_{k=1..n} gcd(k!, k*(k+1)/2)^3.
 * https://oeis.org/A400693
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, new
 * Formula:   a(n) = Sum_{k=1..n} A069268(k)^3, where A069268(k) = k*(k+1)/2
 *            unless k+1 is an odd prime (p), in which case A069268(k) =
 *            (p-1)/2
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 30, u64 n <= 30, u128 n <= 30,
 *            big any n
 * Helpers:   is_odd_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/is_odd_prime.h"

seq_t a(idx_t n)
{
    seq_t sum = 0;
    for (idx_t k = 1; k <= n; ++k)
    {
        idx_t kp1 = k + 1;
        seq_t val;
        if (is_odd_prime(kp1))
            val = ((seq_t)(kp1 - 1)) / 2;
        else
            val = ((seq_t)k * (seq_t)kp1) / 2;
        seq_t v = val * val * val;    /* cube of val */
        sum += v;
    }
    return sum;
}
