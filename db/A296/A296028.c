/*
 * A296028  Characteristic function of primes in the nonmultiples of 3.
 * https://oeis.org/A296028
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 1 if the nth non‑multiple of 3 is prime, otherwise 0
 * Range:     u8 n <= 105, u16 n <= 105, u32 n <= 105, u64 n <= 105,
 *            u128 n <= 105, big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*          a(n) = is_prime( A001651(n) ) , where A001651(n) = n + ⌊(n−1)/2⌋. */

#include <stdbool.h>
#include "../utils/is_prime_big.h"

seq_t a(idx_t n)
{
    if (n == 0)          /* behaviour for offset not defined */
        return 0;

    seq_t sn = (seq_t)n;               /* convert index to seq_t safely   */
    seq_t val = sn + ((sn - 1) >> 1);   /* nth non‑multiple of 3            */

    return is_prime_big(val) ? 1 : 0;
}
