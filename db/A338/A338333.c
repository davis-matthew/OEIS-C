/*
 * A338333  Number of relatively prime 3-part strict integer partitions of n
 *          with no 1's.
 * https://oeis.org/A338333
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 63, u16 n <= 63, u32 n <= 63, u64 n <= 63, u128 n <= 63,
 *            big any n
 * Helpers:   _gcd_idx
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Compute A338333: number of relatively prime strict 3-part partitions of n with no 1's,
   by brute‑force enumeration of all triples a<b<c, a+b+c=n, a>=2. */
#include <stdint.h>
#include "../utils/_gcd_idx.h"

/* helper: Euclidean algorithm for idx_t */

seq_t a(idx_t n) {
    if (n < 9) return 0;          /* the smallest n with a 3-part partition ≥2 is 9 */
    seq_t count = 0;
    idx_t max_a = (n - 3) / 3;     /* a + (a+1) + (a+2) <= n */
    for (idx_t a_val = 2; a_val <= max_a; ++a_val) {
        idx_t max_b = (n - a_val - 1) / 2;   /* ensure c = n-a-b > b */
        for (idx_t b_val = a_val + 1; b_val <= max_b; ++b_val) {
            idx_t c_val = n - a_val - b_val;
            /* c_val > b_val by construction of max_b, but guard anyway */
            if (c_val <= b_val) continue;
            idx_t g = _gcd_idx(a_val, b_val);
            g = _gcd_idx(g, c_val);
            if (g == 1) ++count;
        }
    }
    return count;
}
