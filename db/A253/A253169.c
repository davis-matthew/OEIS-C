/*
 * A253169  Smallest m such that A256188(m) = n.
 * https://oeis.org/A253169
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n^2 if n triangular else n +
 *            T_{floor((sqrt(8*n+1)-1)/2)-1}
 * Range:     u8 n <= 7, u16 n <= 275, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/floor_sqrt.h"

seq_t a(idx_t n) {
    if (n == 0) return 0; /* offset = 1; 0 is unused */
    seq_t nn = (seq_t)n;
    seq_t S   = (seq_t)8 * nn + 1;
    seq_t sqrtS = floor_sqrt(S);

    /* n is triangular if S is a perfect square */
    if (sqrtS * sqrtS == S) {
        return nn * nn;
    } else {
        /* m = floor((sqrtS-1)/2)  -- integer division is fine on seq_t */
        seq_t m = (sqrtS - 1) / 2;
        /* T_{m-1} = (m-1)*m/2 */
        seq_t t = (m - 1) * m / 2;
        return nn + t;
    }
}
