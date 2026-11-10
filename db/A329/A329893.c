/*
 * A329893  a(n) = Product_{k=0..floor(log_2(n))} (1 +
 *          A004718(floor(n/(2^k)))), where A004718 is Per Nørgård's "infinity
 *          sequence".
 * https://oeis.org/A329893
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(0) = 1; a(n) = (1 + A004718(n)) * a(floor(n/2)) for n >= 1,
 *            i.e. a(n) = Product_{k=0..floor(log_2(n))} (1 +
 *            A004718(floor(n/2^k)))
 * Range:     i8 n <= 27, i16 n <= 126, i32 n <= 4082, i64 n <= 16383,
 *            i128 n <= 16383, big any n
 * Validated: standard, pass, 114 terms through n = 9193 (2026-11-08)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n) {
    seq_t prod = 1;
    seq_t v = 0;
    int msb = -1;
    idx_t t = n;
    while (t > 0) {
        msb++;
        t >>= 1;
    }
    for (int i = msb; i >= 0; i--) {
        if ((n >> i) & 1) {
            v += 1;
        } else {
            v = -v;
        }
        /* v is now A004718(n >> i) */
        prod *= (v + 1);
        if (prod == 0) return 0;
    }
    return prod;
}
