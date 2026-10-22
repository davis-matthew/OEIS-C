/*
 * A373293  Number of edge covers of the fan graph F_{n,3}.
 * https://oeis.org/A373293
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 4*7^n - 5*3^n + 3
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 10, u64 n <= 22, u128 n <= 44,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    /* Compute 7^n and 3^n using the provided big_pow helper */
    seq_t pow7 = big_pow((seq_t)7, (seq_t)n);
    seq_t pow3 = big_pow((seq_t)3, (seq_t)n);

    /* a(n) = 4*7^n - 5*3^n + 3 */
    seq_t result = (seq_t)4 * pow7;           /* 4*7^n */
    result = result - (seq_t)5 * pow3;        /* -5*3^n */
    result = result + (seq_t)3;              /* +3 */

    return result;
}
