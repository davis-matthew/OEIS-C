/*
 * A247105  Variation of Flavius Josephus's sieve: Start with the natural
 *          numbers; at the k-th sieving step, make k passes removing every
 *          k-th term of the sequence remaining after the previous sieving
 *          step; iterate.
 * https://oeis.org/A247105
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is computed by applying the inverse of the sieving steps in
 *            reverse order. For step k we undo k passes of removing every
 *            k‑th element. The inverse of one pass is idx_new = idx_old +
 *            floor((idx_old-1)/(k-1)). By repeating this k times for k = n …
 *            2 the original number is obtained
 * Range:     u8 n <= 4, u16 n <= 9, u32 n <= 19, u64 n <= 40, u128 n <= 83,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

seq_t a(idx_t n)
{
    seq_t idx = n;                     /* current index after all future steps */

    for (idx_t k = n; k > 1; --k) {   /* iterate steps from n down to 2 */
        seq_t d = k - 1;               /* step size minus one */

        /* Undo the k passes of removing every k‑th element */
        for (idx_t t = 0; t < k; ++t) {
            seq_t delta = (idx - 1) / d;   /* floor division */
            idx += delta;
        }
    }

    return idx;
}
