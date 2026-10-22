/*
 * A343213  Number of spanning trees of the graphs of the platonic solids
 *          sorted by the number of faces.
 * https://oeis.org/A343213
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full
 * Formula:   a(1)=2^4, a(2)=a(3)=2^7*3, a(4)=a(5)=2^9*3^4*5^3
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 5, u64 n <= 5, u128 n <= 5,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    seq_t result = 0;
    switch (n) {
        case 1:
            result = big_pow((seq_t)2U, (seq_t)4U);
            break;
        case 2:
        case 3:
            result = big_pow((seq_t)2U, (seq_t)7U) * (seq_t)3U;
            break;
        case 4:
        case 5:
            result = big_pow((seq_t)2U, (seq_t)9U) *
                     big_pow((seq_t)3U, (seq_t)4U) *
                     big_pow((seq_t)5U, (seq_t)3U);
            break;
        default:
            result = 0;
            break;
    }
    return result;
}
