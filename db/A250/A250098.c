/*
 * A250098  Number of triangles in minimal triangulation of the orientable
 *          closed surface of genus n (S_n).
 * https://oeis.org/A250098
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2*ceil((7 + sqrt(1+48*n))/2) for all n ≠ 2, a(2) = 24
 * Range:     u8 n <= 2, u16 n <= 60, u32 n <= 60, u64 n <= 60, u128 n <= 60,
 *            big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/floor_sqrt.h"

seq_t a(idx_t n) {
    if (n == 2) {
        return (seq_t)24;
    }
    seq_t val   = (seq_t)n * (seq_t)48 + (seq_t)1;
    seq_t s     = floor_sqrt(val);
    if (s * s == val) {            /* sqrt is integer */
        seq_t numerator = (seq_t)7 + s;
        if ((numerator & (seq_t)1) == (seq_t)0) {
            return numerator;      /* even */
        } else {
            return numerator + (seq_t)1; /* odd -> add 1 */
        }
    } else {                       /* sqrt not integer */
        seq_t t = ((seq_t)7 + s) / (seq_t)2;
        return (t + (seq_t)1) * (seq_t)2;
    }
}
