/*
 * A271936  Commutative powers: numbers of the form a^b = b^a.
 * https://oeis.org/A271936
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 4^(n-1) for 1 <= n <= 3 a(n) = (n-1)^(n-1) for n >= 4
 *            (same as A000302(n-1) for n<=3 and A000312(n-1) for n>=4)
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 10, u64 n <= 16, u128 n <= 21,
 *            big any n
 * Validated: partial, pass, 21 terms through n = 21 (2026-11-01)
 * Helpers:   ipow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/ipow.h"

seq_t a(idx_t n)
{
    if (n <= 3)
    {
        return ipow((seq_t)4, (seq_t)(n - 1));
    }
    else
    {
        return ipow((seq_t)(n - 1), (seq_t)(n - 1));
    }
}
