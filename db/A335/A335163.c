/*
 * A335163  Nim n-th power of 4.
 * https://oeis.org/A335163
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n+15) = a(n)
 * Range:     u8 n <= 89, u16 n <= 89, u32 n <= 89, u64 n <= 89, u128 n <= 89,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

seq_t a(idx_t n)
{
    static const seq_t period[15] = {
        (seq_t)1, (seq_t)4, (seq_t)6, (seq_t)14, (seq_t)5,
        (seq_t)2, (seq_t)8, (seq_t)11, (seq_t)7, (seq_t)10,
        (seq_t)3, (seq_t)12, (seq_t)13, (seq_t)9, (seq_t)15
    };
    idx_t r = n % (idx_t)15;
    return period[(size_t)r];
}
