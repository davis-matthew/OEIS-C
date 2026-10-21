/*
 * A255005  a(n) = the digit sum of prime(n) + the digit product of prime(n).
 * https://oeis.org/A255005
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = A007605(n) + A053666(n) = digit_sum(prime(n)) +
 *            digit_product(prime(n))
 * Range:     u8 n <= 54, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   digit_product, digit_sum, get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/get_prime.h"
#include "../utils/digit_sum.h"
#include "../utils/digit_product.h"

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;                 // offset 1; below offset is undefined, return 0 here
    seq_t prime_val = (seq_t)get_prime(n);
    return digit_sum(prime_val) + digit_product(prime_val);
}
