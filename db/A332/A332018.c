/*
 * A332018  a(n) = A038502(A000265(n)) if n is even or n == 0 (mod 3), a(n) =
 *          A038502(A000265(5*n + 1)) otherwise.
 * https://oeis.org/A332018
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 52, u16 n <= 74, u32 n <= 74, u64 n <= 74, u128 n <= 74,
 *            big any n
 * Helpers:   strip23
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
   Sequence A332018: greatest divisor of n coprime to 6.
   For n even or n ≡ 0 (mod 3): a(n) = n with all factors 2 and 3 removed.
   Otherwise: a(n) = (5 n + 1) with all factors 2 and 3 removed.
*/

#include <stdint.h>
#include "../utils/strip23.h"

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;                 /* undefined for n=0; return 0 as a safe value */

    seq_t x;
    if (((n & 1U) == 0) || (n % 3 == 0))   /* n is not coprime to 6 */
        x = (seq_t)n;
    else                                   /* n is coprime to 6 */
        x = (seq_t)5 * (seq_t)n + 1;

    return strip23(x);
}
