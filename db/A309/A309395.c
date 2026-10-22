/*
 * A309395  Number of integer-sided triangles with sides a,b,c, max(a,b) < c,
 *          a + c = n that are right triangles.
 * https://oeis.org/A309395
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of integer a in [1, floor((n-1)/2)] such that
 *            n(n-2a) is a perfect square
 * Range:     u8 n <= 9, u16 n <= 100, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/floor_sqrt.h"

seq_t a(idx_t n) {
    seq_t count = 0;
    if (n <= 2) return 0;                     /* no possible solutions for n = 1 or 2 */
    idx_t limit = (n - 1) / 2;                /* maximum admissible a */
    seq_t n_seq = (seq_t)n;
    for (idx_t a_val = 1; a_val <= limit; ++a_val) {
        idx_t k = n - 2 * a_val;              /* k = n - 2a */
        seq_t k_seq = (seq_t)k;
        seq_t prod = n_seq * k_seq;           /* n(n-2a) */
        seq_t s = floor_sqrt(prod);           /* integer square root */
        if (s * s == prod) {                  /* perfect square test */
            count += 1;
        }
    }
    return count;
}
