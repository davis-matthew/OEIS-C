/*
 * A373868  Integers that are not perfect fifth powers.
 * https://oeis.org/A373868
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 68, u16 n <= 68, u32 n <= 68, u64 n <= 68, u128 n <= 68,
 *            big any n
 * Validated: partial, pass, 68 terms through n = 68 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A373868: integers that are not perfect fifth powers.
   The function enumerates positive integers > 1, skipping all numbers
   that equal k^5 for some integer k ≥ 1. */

#include <stdbool.h>

/* Return the integer floor(c^(1/5)) for a 64‑bit unsigned integer c.
   Uses binary search with 128‑bit arithmetic to avoid overflow. */
static idx_t fifth_root(idx_t c)
{
    if (c < 32)          /* 2^5 = 32, so all smaller numbers give root 1 */
        return 1;

    idx_t low  = 0;
    idx_t high = 1;
    while ((unsigned __int128)high * high * high * high * high <= (unsigned __int128)c)
        high <<= 1;                /* find an upper bound */

    while (low + 1 < high) {        /* binary search between low and high */
        idx_t mid = low + (high - low) / 2;
        unsigned __int128 midp = (unsigned __int128)mid * mid * mid * mid * mid;
        if (midp <= (unsigned __int128)c)
            low = mid;
        else
            high = mid;
    }
    return low;
}

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
