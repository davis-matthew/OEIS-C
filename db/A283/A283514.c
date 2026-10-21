/*
 * A283514  Concatenation of the numbers from n down to 3.
 * https://oeis.org/A283514
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  base, nonn, easy
 * Formula:   a(n) = concat_{k=n,…,3} k (i.e. a(n) = n(n-1)…3 written in
 *            decimal, each term appended to the right)
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 10, u64 n <= 15, u128 n <= 19,
 *            big any n
 * Helpers:   digit_count, pow10
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/digit_count.h"
#include "../utils/pow10.h"

seq_t a(idx_t n)
{
    if (n < 3)
        return 0;                       /* defined only for n ≥ 3 */

    seq_t result = 0;

    for (idx_t k = n; k >= 3; --k)
    {
        idx_t len   = digit_count((uint64_t)k);   /* number of decimal digits of k */
        result      = result * pow10(len) + (seq_t)k;
    }

    return result;
}
