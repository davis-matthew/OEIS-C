/*
 * A360734  The number of regions into which the plane is divided by a
 *          hypotrochoid with parameters R = d = prime(n+1) and r = prime(n).
 * https://oeis.org/A360734
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = prime(n+1)*(prime(n+1)-prime(n)-1)+2
 * Range:     u8 n <= 14, u16 n <= 51, u32 n <= 51, u64 n <= 51, u128 n <= 51,
 *            big any n
 * Helpers:   get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/get_prime.h"

seq_t a(idx_t n)
{
    if (n < 1)
        return 0; /* Undefined for n < 1 */

    seq_t p_prev = (seq_t)get_prime(n);
    seq_t p_next = (seq_t)get_prime(n + 1);

    seq_t diff = p_next - p_prev - 1;

    return p_next * diff + 2;
}
