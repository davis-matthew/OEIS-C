/*
 * A294370  Lexicographically earliest sequence of distinct positive numbers
 *          such that, for any n > 0, a(2*n) = 3*a(n).
 * https://oeis.org/A294370
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 63, u16 n <= 66, u32 n <= 66, u64 n <= 66, u128 n <= 66,
 *            big any n
 * Helpers:   pow3
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = ( i + (i-1)/2 ) * 3^v₂(n)
 *   where n = 2^v · m with m odd,
 *   i = (m+1)/2 (the position of m among the odd numbers),
 *   i + (i-1)/2 is the i‑th positive integer not divisible by 3.
 *   v₂(n) is the exponent of 2 in n.
 *
 * This follows the description:
 *   a(A005408(i) · 2^j) = A001651(i) · 3^j
 * where A005408 lists odd numbers and A001651 lists numbers not
 * divisible by 3.
 */
#include <stdbool.h>
#include "../utils/pow3.h"

seq_t a(idx_t n) {
    idx_t temp = n;
    idx_t v = 0;           /* exponent of 2 in n */

    /* strip powers of 2 from n */
    while ((temp & 1) == 0) {
        temp >>= 1;
        ++v;
    }

    idx_t m   = temp;                  /* odd part of n   */
    idx_t i   = (m + 1) / 2;           /* index of m among odd numbers */
    idx_t nondiv3 = i + (i - 1) / 2;   /* i-th number not divisible by 3 */

    seq_t base = (seq_t)nondiv3;
    seq_t pw   = pow3(v);

    return base * pw;          /* multiplication works for all seq_t types */
}
