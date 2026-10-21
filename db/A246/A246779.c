/*
 * A246779  Strictly increasing terms of the sequence A246776: a(1)=
 *          A246776(1) and for n>0 a(n+1) is the next term greater than a(n)
 *          after that a(n) appears in A246776 for the first time.
 * https://oeis.org/A246779
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n + 17 for n > 22; a(1..22) are defined explicitly
 * Range:     u8 n <= 68, u16 n <= 68, u32 n <= 68, u64 n <= 68, u128 n <= 68,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>  /* for uint64_t, may be unused but harmless */

seq_t a(idx_t n)
{
    /* first 22 terms of the sequence, a(1)=0, a(22)=38 */
    static const seq_t first22[22] = {
        0, 1, 4, 6, 9, 11, 13, 14, 15, 17,
        20, 22, 24, 25, 27, 28, 30, 32, 33, 34,
        37, 38
    };

    if (n <= 22)
        return first22[n - 1];

    /* For n > 22, the conjectured formula holds */
    return (seq_t)(n + 17);
}
