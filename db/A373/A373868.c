/*
 * A373868  Integers that are not perfect fifth powers.
 * https://oeis.org/A373868
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 68, u16 n <= 68, u32 n <= 68, u64 n <= 68, u128 n <= 68,
 *            big any n
 * Helpers:   fifth_root
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A373868: integers that are not perfect fifth powers.
   The function enumerates positive integers > 1, skipping all numbers
   that equal k^5 for some integer k ≥ 1. */

#include <stdbool.h>
#include "../utils/fifth_root.h"

/* Return the nth term of A373868 (1‑based index). */
seq_t a(idx_t n)
{
    idx_t count    = 0;     /* how many non‑fifth‑power numbers we have seen */
    idx_t candidate = 2;    /* start after 1, because 1 = 1^5 is excluded */

    for (;;) {
        idx_t r = fifth_root(candidate);
        unsigned __int128 r5 = (unsigned __int128)r * r * r * r * r;

        /* candidate is a member of the sequence only if it is NOT a
           perfect fifth power. */
        if ((unsigned __int128)candidate != r5) {
            ++count;
            if (count == n)
                return (seq_t)candidate;   /* fits in seq_t if test harness
                                               requires it */
        }
        ++candidate;    /* advance to the next integer */
    }
}
