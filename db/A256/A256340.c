/*
 * A256340  Numbers which have only digits 7 and 8 in base 10.
 * https://oeis.org/A256340
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) is the n‑th positive integer composed only of decimal
 *            digits 7 and 8, in ordinary increasing order. Equivalently, take
 *            n+1, write it in binary, drop the leading 1 bit, then replace
 *            0->7 and 1->8 to form the decimal number
 * Range:     u8 n <= 6, u16 n <= 30, u32 n <= 1022, u64 n <= 8190,
 *            u128 n <= 8190, big any n
 * Helpers:   ilog2_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/ilog2_u64.h"

seq_t a(idx_t n) {
    /* n is 1‑based.  Compute the length L of the decimal representation. */
    unsigned int L = ilog2_u64((uint64_t)n + 1);          /* floor_log2(n+1)  */
    uint64_t pow2L = ((uint64_t)1) << L;                 /* 2^L             */
    uint64_t offset = (uint64_t)n - (pow2L - 2);         /* 1‑based offset in this length block */
    uint64_t index  = offset - 1;                       /* 0‑based binary index */

    seq_t result = 0;
    for (int i = (int)L - 1; i >= 0; --i) {
        result = result * 10 + ((index >> i) & 1 ? 8 : 7);
    }
    return result;
}
