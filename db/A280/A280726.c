/*
 * A280726  a(n) = A049501(phi(n)).
 * https://oeis.org/A280726
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = major index of φ(n) (sum of positions of 1→0 descents in
 *            binary representation of φ(n))
 * Range:     u8 n <= 756, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   floor_log2_uint64, phi
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/phi.h"
#include "../utils/floor_log2_uint64.h"

seq_t a(idx_t n) {
    if (n == 0) return 0;          /* safety for n<1, though not used */
    /* compute φ(n) */
    seq_t phi_val = phi(n);
    /* φ(n) fits in idx_t because φ(n) ≤ n ≤ 2^64‑1 */
    idx_t val = (idx_t)phi_val;
    if (val <= 1) return 0;        /* φ(1)=1 gives major index 0 */

    /* find most significant bit position (0‑based) */
    idx_t msb = floor_log2_uint64(val);

    seq_t sum = 0;
    int prev_bit = 1;              /* the msb is always 1 */

    for (int i = (int)msb; i > 0; --i) {
        int cur_bit = (int)((val >> (i-1)) & 1);
        if (prev_bit == 1 && cur_bit == 0) {
            /* position of the transition: msb − i + 1 (1‑based indexing) */
            sum += (seq_t)(msb - (idx_t)i + 1);
        }
        prev_bit = cur_bit;
    }

    return sum;
}
