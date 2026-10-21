/*
 * A262148  Denominators of domain values whose images via the flowsnake
 *          Q-function follow a spiral trajectory.
 * https://oeis.org/A262148
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  frac, nonn
 * Formula:   a(n) = 49*a(n-6) for n>6, with initial terms a(1)=56, a(2)=56,
 *            a(3)=392, a(4)=392, a(5)=56, a(6)=392
 * Range:     u8 n <= 2, u16 n <= 12, u32 n <= 30, u64 n <= 30, u128 n <= 30,
 *            big any n
 * Helpers:   pow49
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>  /* for size_t */
#include "../utils/pow49.h"

seq_t a(idx_t n) {
    /* pattern of the first 6 terms */
    static const seq_t base[6] = {56, 56, 392, 392, 56, 392};
    /* convert to 0‑based index */
    idx_t n0 = n - 1;
    idx_t k   = n0 / 6;          /* how many times the recurrence has been applied */
    idx_t r   = n0 % 6;          /* position inside the 6‑term pattern */

    /* compute the term */
    seq_t pow_v = pow49(k);
    return base[r] * pow_v;
}
