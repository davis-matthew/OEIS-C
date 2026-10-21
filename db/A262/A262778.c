/*
 * A262778  a(n) = 10^n + prime(n).
 * https://oeis.org/A262778
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 10^n + prime(n)
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 9, u64 n <= 18, u128 n <= 18,
 *            big any n
 * Helpers:   get_prime, pow10
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/pow10.h"
#include "../utils/get_prime.h"

seq_t a(idx_t n)
{
    // Compute 10^n using the provided pow10 helper
    seq_t val = pow10(n);

    // Add the nth prime (prime(1)=2)
    val += (seq_t)get_prime(n);

    return val;
}
