/*
 * A318766  a(0) = 1; for n > 0, a(n) = (prime(n)^2 - 1) * a(n-1).
 * https://oeis.org/A318766
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0)=1; for n>0, a(n)=Π_{k=1..n}(prime(k)^2-1)
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 6, u64 n <= 9, u128 n <= 15,
 *            big any n
 * Helpers:   get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/get_prime.h"

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)1;          /* a(0) is defined as 1 */

    seq_t result = (seq_t)1;
    for (idx_t k = 1; k <= n; ++k)
    {
        uint32_t p = get_prime(k);   /* prime(k), 1‑based */
        seq_t sp = (seq_t)p;
        seq_t factor = sp * sp;      /* p^2 */
        factor -= (seq_t)1;          /* p^2 - 1 */
        result *= factor;            /* accumulate product */
    }
    return result;
}
