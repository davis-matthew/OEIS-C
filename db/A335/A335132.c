/*
 * A335132  Numbers whose binary expansion generates 3-fold rotationally
 *          symmetric EQ-triangles.
 * https://oeis.org/A335132
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   term is the nth number whose binary expansion generates a 3‑fold
 *            rotationally symmetric EQ‑triangle. Computation is done by a
 *            membership test and linear search
 * Range:     u8 n <= 17, u16 n <= 52, u32 n <= 52, u64 n <= 52, u128 n <= 52,
 *            big any n
 * Helpers:   is_member
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>
#include "../utils/is_member.h"

/* Forward declaration of the entry routine */
static bool is_member(seq_t m);

/* Return the n-th term of A335132 (1‑based index). */
seq_t a(idx_t n) {
    if (n == 1) {
        return 0;                /* a(1) = 0 */
    }
    idx_t found = 1;             /* count including 0 */
    seq_t candidate = 1;         /* first odd number */

    while (found < n) {
        if (is_member(candidate)) {
            ++found;
            if (found == n) {
                return candidate;
            }
        }
        candidate += 2;         /* skip even numbers */
    }
    /* Should never reach here */
    return 0;
}
