/*
 * A326398  a(n) is the smallest k > 0 such that the concatenation prime(n)k
 *          is composite.
 * https://oeis.org/A326398
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = 2 if 10*prime(n)+1 is prime else 1
 * Range:     u8 n <= 10, u16 n <= 3132, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   get_prime, is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/get_prime.h"
#include "../utils/is_prime_big.h"

seq_t a(idx_t n)
{
    /* n >= 1  (offset 1) */
    uint32_t p = get_prime(n);          /* nth prime */
    seq_t val = (seq_t)10 * p + 1;        /* concatenation prime(n)1 */
    return is_prime_big(val) ? 2 : 1;
}
