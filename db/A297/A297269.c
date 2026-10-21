/*
 * A297269  Numbers whose base-9 digits have greater up-variation than
 *          down-variation; see Comments.
 * https://oeis.org/A297269
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 150, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   desired
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A297269: numbers whose base‑9 up‑variation exceeds down‑variation
 * (computed by brute‑force enumeration). */

#include <stdbool.h>
#include "../utils/desired.h"

/* Return the n‑th term of A297269 (1‑based). */
seq_t a(idx_t n) {
    if (n == 0)
        return 0;   /* undefined for n = 0, but harmless */

    seq_t num = 1;          /* candidate number */
    idx_t count = 0;        /* how many valid numbers found */

    while (count < n) {
        if (desired(num))
            ++count;
        ++num;
    }

    /* num has been incremented after finding the nth valid number */
    return num - 1;
}
