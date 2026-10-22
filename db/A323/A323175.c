/*
 * A323175  A Syracuse-like routine producing loops (see the Comments
 *          section).
 * https://oeis.org/A323175
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn, fini, easy
 * Formula:   not stated
 * Range:     u8 n <= 31, u16 n <= 143, u32 n <= 143, u64 n <= 143,
 *            u128 n <= 143, big any n
 * Helpers:   is_prime_big, reverse_decimal
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence:
   a(1) = 1
   For n > 1:
       if a(n‑1) is prime:
           a(n) = reverse_decimal(a(n‑1)) + 1
       else:
           a(n) = a(n‑1) + 1
*/
#include <stdbool.h>
#include "../utils/is_prime_big.h"
#include "../utils/reverse_decimal.h"

seq_t a(idx_t n)
{
    seq_t curr = 0;
    for (idx_t i = 1; i <= n; ++i)
    {
        if (is_prime_big(curr))
            curr = reverse_decimal(curr) + 1;
        else
            curr = curr + 1;
    }
    return curr;
}
