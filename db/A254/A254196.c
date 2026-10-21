/*
 * A254196  a(n) is the numerator of Product_{i=1..n} (1/(1-1/prime(i))) - 1.
 * https://oeis.org/A254196
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = numerator of ∏_{i=1..n} 1/(1-1/prime(i)) - 1 (the
 *            numerator of the reduced fraction of the sum of reciprocal
 *            numbers whose prime factors are among the first n primes)
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 12, u64 n <= 19, u128 n <= 31,
 *            big any n
 * Helpers:   gcd, get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/gcd.h"
#include "../utils/get_prime.h"

seq_t
a(idx_t n)
{
    /* OEIS starts at n = 1; return 0 for n = 0 as a safe default. */
    if (n == 0)
        return 0;

    seq_t num = 1;   /* numerator of the product P = ∏ p/(p-1) in lowest terms */
    seq_t den = 1;   /* denominator of the product P in lowest terms */

    for (idx_t i = 1; i <= n; ++i) {
        uint32_t p = get_prime(i);          /* i‑th prime (1‑based). */
        seq_t pk   = (seq_t)p;
        seq_t new_num = num * pk;
        seq_t new_den = den * (pk - (seq_t)1);

        seq_t g = gcd(new_num, new_den);
        num = new_num / g;
        den = new_den / g;
    }

    /* Now P = num/den. Compute (P - 1) = (num - den) / den in lowest terms. */
    seq_t diff_num = num - den;
    seq_t g2 = gcd(diff_num, den);
    seq_t result = diff_num / g2;

    return result;
}
