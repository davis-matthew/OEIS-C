/*
 * A297277  Numbers whose base-12 digits have equal down-variation and
 *          up-variation; see Comments.
 * https://oeis.org/A297277
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 32, u16 n <= 5472, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   _good_base12
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: Numbers whose base‑12 digits have equal down‑variation and up‑variation.
   Computed by scanning natural numbers and keeping those that satisfy
   dv = uv. */

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include "../utils/_good_base12.h"

/* Return the nth term of A297277 (offset 1). */
seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* undefined for offset 0 */
    idx_t count = 0;
    seq_t num = 0;
    while (true) {
        ++num;                     /* start at 1 */
        if (_good_base12(num)) {
            ++count;
            if (count == n) return num;
        }
    }
}
