/*
 * A344215  a(n) = n*(3^(n-1) - 2^(n-1)).
 * https://oeis.org/A344215
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n*(3^(n-1) - 2^(n-1))
 * Range:     u8 n <= 4, u16 n <= 9, u32 n <= 18, u64 n <= 28, u128 n <= 28,
 *            big any n
 * Helpers:   pow2, pow3
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"
#include "../utils/pow3.h"

seq_t a(idx_t n)
{
    if (n <= 1) {
        return 0;
    }

    seq_t p3 = pow3((idx_t)(n - 1));
    seq_t p2 = pow2((idx_t)(n - 1));
    seq_t diff = p3 - p2;
    return ((seq_t)n) * diff;
}
