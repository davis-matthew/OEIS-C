/*
 * A359841  Integers Xd which are divisible by X, where d is the last decimal
 *          digit.
 * https://oeis.org/A359841
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 48, u16 n <= 65, u32 n <= 65, u64 n <= 65, u128 n <= 65,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
/* Sequence A359841: numbers k such that k is divisible by floor(k/10) */
seq_t a(idx_t n)
{
    static const seq_t table[32] = {
        10,11,12,13,14,15,16,17,18,19,
        20,22,24,26,28,30,33,36,39,40,
        44,48,50,55,60,66,70,77,80,88,
         90,99
    };

    if (n <= 32)
        return table[(size_t)(n - 1)];

    /* For n >= 33, all terms are multiples of 10 (starting at 100). */
    return ((seq_t)10) * ((seq_t)(n - 23));
}
