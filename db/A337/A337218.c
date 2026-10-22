/*
 * A337218  The positive integers uniquely represented by the ternary form x^2
 *          + 2*y^2 + 2*z^2, with integers x <= 0, and 0 <= y <= z.
 * https://oeis.org/A337218
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 24, u16 n <= 29, u32 n <= 37, u64 n <= 38, u128 n <= 38,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A337218: merge of the 21‑element finite set
   (2*A337217 together with the 1(mod 4) members of A094739)
   and the infinite sequence 3·4^k  (k ≥ 0). */

#include <stddef.h>

seq_t a(idx_t n)
{
    /* Finite part (sorted ascending). */
    static const seq_t finite[21] = {
        1, 2, 5, 6, 10, 13, 14, 21, 22, 30,
        37, 42, 46, 58, 70, 78, 93, 133, 142, 190, 253
    };
    const size_t finite_size = 21;
    size_t f_idx = 0;          /* current index in the finite array */
    seq_t pow4 = 3;            /* current value of 3·4^k, starting with k = 0 */

    for (idx_t i = 1; i <= n; ++i) {
        seq_t next;

        if (f_idx < finite_size && finite[f_idx] <= pow4) {
            next = finite[f_idx++];
        } else {
            next = pow4;
            pow4 *= 4;   /* advance to the next power 3·4^{k+1} */
        }

        if (i == n)
            return next;
    }
    /* Should never be reached for n ≥ 1 */
    return 0;
}
