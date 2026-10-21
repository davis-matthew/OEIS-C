/*
 * A249714  Record values in A249695.
 * https://oeis.org/A249714
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 0,1,3,6,7,12,21 for n = 1 … 7; a(n) = 3 * 2^(n-5) for n ≥
 *            8 (i.e. a(n) = 3 << (n-5))
 * Range:     u8 n <= 11, u16 n <= 19, u32 n <= 29, u64 n <= 29, u128 n <= 29,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"

seq_t a(idx_t n) {
    switch (n) {
        case 1: return (seq_t)0;
        case 2: return (seq_t)1;
        case 3: return (seq_t)3;
        case 4: return (seq_t)6;
        case 5: return (seq_t)7;
        case 6: return (seq_t)12;
        case 7: return (seq_t)21;
        default: {
            seq_t v = pow2(n - 5); /* v = 2^(n-5) */
            v *= (seq_t)3;         /* v = 3 * 2^(n-5) */
            return v;
        }
    }
}
