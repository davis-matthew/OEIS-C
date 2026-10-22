/*
 * A333217  Numbers k such that the k-th composition in standard order covers
 *          an initial interval of positive integers.
 * https://oeis.org/A333217
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 134, u16 n <= 1008, u32 n <= 1008, u64 n <= 1008,
 *            u128 n <= 1008, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Search for the n-th k such that its composition in standard order
   (row k of A066099) covers an initial interval of positive integers. */

#include <stdint.h>
#include <stdbool.h>

/* Determine whether the k‑th composition covers an initial interval.
   Algorithm:
   * Enumerate the 1‑bit positions of k (least significant bit first).
   * Prepend a 0, take successive differences, and use that multiset of
     parts.  The composition covers an interval iff all integers from
     1 up to the maximum part appear at least once. */
static bool covers_initial_interval(uint64_t k)
{
    /* positions of 1‑bits, 1‑based, with a leading 0 at position 0 */
    int pos[65];          /* enough for any 64‑bit integer */
    int m = 0;            /* number of positions stored */
    pos[m++] = 0;         /* prepend 0 */

    int idx = 1;          /* bit position, 1‑based */
    uint64_t temp = k;
    while (temp != 0) {
        if (temp & 1ULL) pos[m++] = idx;
        temp >>= 1;
        ++idx;
    }

    /* Build a bitmask of the parts that appear and track the maximum part. */
    uint64_t mask = 0ULL;
    int max_part = 0;
    for (int i = 1; i < m; ++i) {
        int diff = pos[i] - pos[i - 1];     /* positive */
        if (diff > max_part) max_part = diff;
        mask |= 1ULL << (diff - 1);
    }

    /* k = 0 yields an empty multiset; it is considered to cover the
       initial interval (by definition of A333217). */
    if (max_part == 0) return true;

    /* Check that every part 1..max_part is present. */
    for (int p = 1; p <= max_part; ++p) {
        if ((mask & (1ULL << (p - 1))) == 0) return false;
    }
    return true;
}

seq_t a(idx_t n)
{
    uint64_t found = 0;
    uint64_t k = 0;

    for (;;) {
        if (covers_initial_interval(k)) {
            ++found;
            if (found == (uint64_t)n) {
                return (seq_t)k;
            }
        }
        ++k;
    }
}
