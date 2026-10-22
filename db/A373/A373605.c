/*
 * A373605  Sum of the even-indexed digits minus the sum of the odd-indexed
 *          digits in the primorial base representation (A049345) of n.
 * https://oeis.org/A373605
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, base, easy
 * Formula:   a(n) = Σ_{k even} d_k – Σ_{k odd} d_k, where (d_0, d_1, …) are
 *            the digits in the primorial base representation of n
 * Range:     i8 n <= 9939, i16 n <= 9939, i32 n <= 9939, i64 n <= 9939,
 *            i128 n <= 9939, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

/* Simple primality test for values that fit in uint64_t.
   In the usage below the numbers are always small (primes up to a few
   hundred), so trial division up to sqrt(n) is more than fast enough. */
static bool is_prime_idx(idx_t n)
{
    if (n < 2) return false;
    if (n % 2 == 0) return n == 2;
    for (idx_t i = 3; i * i <= n; i += 2)
        if (n % i == 0) return false;
    return true;
}

seq_t a(idx_t n)
{
    seq_t total = 0;           /* alternating digit sum */
    idx_t m = n;               /* working copy of the number */
    idx_t prime = 2;           /* current prime for modulus/division */
    idx_t pos = 0;             /* position of the current digit (0 = LSB) */

    while (m > 0)
    {
        idx_t digit = m % prime;  /* next digit in the primorial base */
        m /= prime;              /* shift the remaining part */

        /* add or subtract depending on position parity */
        if (pos & 1)
            total -= (seq_t)digit;
        else
            total += (seq_t)digit;

        ++pos;  /* advance to the next digit */

        /* find the next prime for the next digit */
        ++prime;
        while (!is_prime_idx(prime))
            ++prime;
    }

    return total;
}
