/*
 * A281375  a(n) = floor(2^(n+1)/n).
 * https://oeis.org/A281375
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = floor(2^(n+1) / n)
 * Range:     u8 n <= 10, u16 n <= 19, u32 n <= 36, u64 n <= 69, u128 n <= 133,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    /* For small n we can use a 128‑bit intermediate to avoid
       overflow of the shift operator on 64‑bit seq_t types.  The
       condition (n+1) < 128 is safe because the left shift on
       unsigned __int128 values is defined for shift amounts
       less than the width of the type.  For larger exponents
       the seq_t type must support arbitrary‑precision integers,
       in which case the common pow2 helper implements the shift. */
    if (n + 1 < 128) {
        unsigned __int128 val = ((unsigned __int128)1 << (n + 1));
        unsigned __int128 q   = val / n;
        return (seq_t)q;
    } else {
        seq_t num = pow2(n + 1);
        return num / (seq_t)n;
    }
}
