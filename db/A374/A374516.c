/*
 * A374516  Sum of leaders of maximal anti-runs in the n-th composition in
 *          standard order.
 * https://oeis.org/A374516
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 86, u16 n <= 86, u32 n <= 86, u64 n <= 86, u128 n <= 86,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A374516 – sum of leaders of maximal anti‑runs in the n‑th
    composition in standard order (graded reverse‑lexicographic, A066099).
    The composition for index n is obtained by:
        1. Write n in binary, then reverse that bit string.
        2. Record the (1‑based) positions of the 1’s in that reversed string.
        3. Prepend 0, take first differences, and finally reverse that list.
    The resulting list is the composition.  The required value is the
    sum of the leaders of its maximal anti‑runs – i.e. the first part of
    each block of consecutive unequal parts.  */

#include <stddef.h>

seq_t a(idx_t n) {
    /* Empty composition for n = 0 */
    if (n == 0) {
        return 0;
    }

    /* Maximum number of set bits in an unsigned 64‑bit integer is 64.
       We store the parts (differences) in a small fixed array. */
    unsigned int parts[65];
    size_t poscnt = 0;            /* number of parts */
    idx_t bits   = n;             /* copy of n for bit manipulation */
    idx_t pos    = 0;             /* current bit position (0‑based) */
    idx_t last_pos = 0;           /* 1‑based position of previous set bit */

    /* Scan bits of n from least significant to most significant.
       For each set bit, compute the difference with the previous
       set bit (or with 0 for the first).  This gives the composition
       parts in normal order. */
    while (bits) {
        if (bits & 1ULL) {
            unsigned int cur = (unsigned int)(pos + 1); /* 1‑based position */
            parts[poscnt++] = cur - (unsigned int)last_pos;
            last_pos = cur;
        }
        bits >>= 1;
        pos++;
    }

    /* Compute the sum of leaders of maximal anti‑runs.
       The composition parts must be considered in reverse order. */
    seq_t sum = (seq_t)parts[poscnt - 1];
    unsigned int prev_part = parts[poscnt - 1];

    for (int i = (int)poscnt - 2; i >= 0; --i) {
        unsigned int curr = parts[i];
        if (curr == prev_part) {
            sum += (seq_t)curr;
        }
        prev_part = curr;
    }

    return sum;
}
