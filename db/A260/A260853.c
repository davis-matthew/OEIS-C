/*
 * A260853  Base-3 representation of a(n) is the concatenation of the base-3
 *          representations of 1, 2, ..., n, n-1, ..., 1.
 * https://oeis.org/A260853
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = concatenate the base‑3 representations of 1,2,…,n,
 *            n‑1,…,2,1 (a(0)=0)
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 6, u64 n <= 10, u128 n <= 15,
 *            big any n
 * Helpers:   append_base3_digits
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/append_base3_digits.h"

seq_t a(idx_t n)
{
    if (n == 0ULL)
        return (seq_t)0;

    seq_t result = 0;

    /* forward sequence 1..n */
    for (uint64_t i = 1; i <= n; ++i)
        append_base3_digits(&result, i);

    /* backward sequence n‑1..1 */
    for (uint64_t i = n - 1; i >= 1; --i) {
        append_base3_digits(&result, i);
        if (i == 1) /* avoid underflow of unsigned loop */
            break;
    }

    return result;
}
