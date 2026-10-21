/*
 * A283393  a(n) = gcd(n^2-1, n^2+9).
 * https://oeis.org/A283393
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = gcd(n^2 - 1, n^2 + 9)
 * Range:     u8 n <= 89, u16 n <= 89, u32 n <= 89, u64 n <= 89, u128 n <= 89,
 *            big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/gcd.h"

seq_t a(idx_t n)
{
    /* We need only n^2 − 1 modulo 10.  Since gcd(x, y) = gcd(x, y–x),
       gcd(n^2-1, n^2+9) = gcd(n^2-1, 10).  The value n^2-1 modulo 10
       depends only on n modulo 10, so the implementation is O(1).  */
    seq_t t   = (seq_t)(n % 10);                 /* n mod 10 */
    seq_t sq  = t * t;                           /* (n mod 10)^2  */
    seq_t val = (sq + (seq_t)9) % (seq_t)10;     /* (n^2 - 1) mod 10 */
    return gcd(val, (seq_t)10);                  /* gcd with the constant 10 */
}
