/*
 * A283801  Concatenation of the first n odd composite numbers (A071904).
 * https://oeis.org/A283801
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn, easy
 * Formula:   a(n) = concatenation of the first n odd composite numbers
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 5, u64 n <= 10, u128 n <= 15,
 *            big any n
 * Helpers:   digit_len, is_prime, pow10
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/is_prime.h"
#include "../utils/pow10.h"
#include "../utils/digit_len.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* offset starts at 1 */

    seq_t result = 0;
    idx_t count   = 0;
    idx_t num     = 9;              /* first odd composite */

    while (count < n)
    {
        if (!is_prime(num))        /* composite odd number */
        {
            seq_t part = (seq_t)num;
            int     d = digit_len(part);
            seq_t mult = pow10((idx_t)d);
            result = result * mult + part;
            count++;
        }
        num += 2;                  /* skip even numbers */
    }
    return result;
}
