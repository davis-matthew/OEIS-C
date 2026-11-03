/*
 * A374767  Numbers k such that the leaders of strictly decreasing runs in the
 *          k-th composition in standard order are distinct.
 * https://oeis.org/A374767
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 64, u16 n <= 64, u32 n <= 64, u64 n <= 64, u128 n <= 64,
 *            big any n
 * Validated: partial, pass, 64 terms through n = 64 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation of A374767:
   a(n) is the n‑th nonnegative integer k such that the leaders
   of the strictly decreasing runs in the k‑th composition
   (in standard order) are pairwise distinct.  This is done by
   generating the composition for each k and checking the leaders
   linearly.  The algorithm runs in O(n·log k) time for k up to 2^64. */

#include <stdint.h>
#include <stdbool.h>

/* Return true iff the k‑th composition has distinct leaders. */
static bool distinct_leaders_of(uint64_t k)
{
    /* The empty composition (k == 0) is automatically valid. */
    if (k == 0) return true;

    /* Positions of 1 bits in the reversed binary of k (LSB first). */
    uint64_t bits = k;
    int pos = 1;
    int pos_arr[65];
    int pcnt = 0;
    while (bits) {
        if (bits & 1ULL) pos_arr[pcnt++] = pos;
        bits >>= 1;
        ++pos;
    }

    /* First differences of [0, pos_arr...] gives the parts before reversing. */
    int diff[65];
    for (int i = 0; i < pcnt; ++i) {
        if (i == 0) diff[i] = pos_arr[0];
        else diff[i] = pos_arr[i] - pos_arr[i - 1];
    }

    bool seen[65] = {0};
    int last_part = 0;

    /* Process parts in reverse order (which yields the actual composition). */
    for (int i = pcnt - 1; i >= 0; --i) {
        int part = diff[i];
        bool new_leader;
        if (i == pcnt - 1)            /* first part in the actual order */
            new_leader = true;
        else                          /* new run starts if current part >= previous part */
            new_leader = (part >= last_part);

        if (new_leader) {
            if (seen[part]) return false;
            seen[part] = true;
        }
        last_part = part;
    }
    return true;
}

seq_t a(idx_t n)
{
    idx_t count = 0;
    for (uint64_t k = 0; ; ++k) {
        if (distinct_leaders_of(k)) {
            ++count;
            if (count == n)
                return (seq_t)k;
        }
    }
}
