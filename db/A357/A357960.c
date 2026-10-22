/*
 * A357960  a(n) = A005259(n-1)^5 * A005258(n)^6.
 * https://oeis.org/A357960
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = A005259(n-1)^5 * A005258(n)^6
 * Range:     u8 none, u16 n <= 1, u32 n <= 1, u64 n <= 2, u128 n <= 4,
 *            big any n
 * Helpers:   seq_pow, sum_A005258, sum_A005259
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/seq_pow.h"
#include "../utils/sum_A005258.h"
#include "../utils/sum_A005259.h"

seq_t a(idx_t n)
{
    seq_t a258 = sum_A005258(n);     /* A005258(n)   */
    seq_t a259 = sum_A005259(n - 1); /* A005259(n-1) */

    seq_t p5 = seq_pow(a259, 5);
    seq_t p6 = seq_pow(a258, 6);

    return p5 * p6;
}
