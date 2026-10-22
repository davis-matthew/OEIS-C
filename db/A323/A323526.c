/*
 * A323526  One and prime numbers whose prime index is a perfect square.
 * https://oeis.org/A323526
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1) = 1; for n > 1, a(n) = the ((n-1)^2)-th prime
 * Range:     u8 n <= 8, u16 n <= 46, u32 n <= 46, u64 n <= 46, u128 n <= 46,
 *            big any n
 * Helpers:   get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/get_prime.h"

seq_t a(idx_t n)
{
    if (n == (idx_t)1) {
        return (seq_t)1;
    }

    idx_t m = n - (idx_t)1;
    idx_t pos = m * m;          /* position of the desired prime */
    uint32_t p = get_prime(pos);  /* get_prime returns the pos-th prime */

    return (seq_t)p;
}
