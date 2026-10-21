/*
 * A297249  Numbers whose base-3 digits have greater down-variation than
 *          up-variation; see Comments.
 * https://oeis.org/A297249
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) is the nth integer whose base‑3 down‑variation exceeds its
 *            up‑variation. The function computes this by brute‑force testing
 *            of successive integers
 * Range:     u8 n <= 125, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   sat
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/sat.h"

/* Return the n­th term (1‑based) of the sequence. */
seq_t a(idx_t n)
{
    if (n == 0) return 0;                /* undefined for n = 0 */

    idx_t count = 0;
    idx_t cand = 1;                      /* candidate integer to test */

    while (1) {
        seq_t val = (seq_t)cand;
        if (sat(val)) {
            ++count;
            if (count == n)
                return val;
        }
        ++cand;
    }
}
