/*
 * A290562  a(n) = n - cos(n*Pi/2).
 * https://oeis.org/A290562
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = n - cos(n*Pi/2) Equivalent integer form: if n%4 == 0: a =
 *            n - 1 if n%4 == 1: a = n if n%4 == 2: a = n + 1 if n%4 == 3: a =
 *            n
 * Range:     i8 n <= 128, i16 n <= 1000, i32 n <= 1000, i64 n <= 1000,
 *            i128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    seq_t nseq = (seq_t)n;
    switch (n & 3ULL) {
        case 0: return nseq - 1;
        case 1: return nseq;
        case 2: return nseq + 1;
        case 3: return nseq;
    }
    /* unreachable, but keep compiler happy */
    return nseq;
}
