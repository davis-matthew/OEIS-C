/*
 * A361294  A variant of payphone permutations: given a circular booth with n
 *          payphones, one of which is already occupied, a(n) is the number
 *          ways for n-1 people to choose the payphones in order, where each
 *          person chooses an unoccupied payphone such that the closest
 *          occupied payphone is as distant as possibl...
 * https://oeis.org/A361294
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 9, u16 n <= 13, u32 n <= 19, u64 n <= 31, u128 n <= 51,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
   Sequence A361294: Variant payphone permutations.
   The process can be modeled by maintaining the multiset of
   “gaps” (number of consecutive empty payphones between
   two occupied ones).  At each step a person chooses a
   seat that lies in a gap of maximal size.  For
   a gap of size g:

      • if g is odd: there is a unique seat at distance (g-1)/2
        from the nearest occupied.  The gap splits into two
        gaps of size (g-1)/2 each.

      • if g is even: there are two symmetric seats, each at
        distance g/2-1 from the nearest occupied.  The gap
        splits into gaps of sizes (g/2-1) and (g/2) (the
        latter is always ≥1).

   When several gaps of the same maximal size exist we have
   an independent choice of which gap to fill.  The number of
   options for a step is therefore

         m   if the maximal gap size is odd or equal to 1,
         2·m otherwise   (m = number of gaps of that maximal size).

   This leads to an O(n) algorithm that iteratively updates
   the multiset of gap sizes and multiplies the corresponding
   options to obtain a(n).
*/

#include <stdlib.h>

seq_t a(idx_t n)
{
    if (n <= 1) return 1;                // a(1) = 1

    idx_t size = n;                      // frequency array size n (gaps 1..n-1)
    idx_t *freq = (idx_t*)calloc(size, sizeof(idx_t));
    if (!freq) return 0;                 // allocation failed

    idx_t max_sz = n - 1;                // initial single gap of size n-1
    freq[max_sz] = 1;
    idx_t cur_max = max_sz;

    seq_t result = 1;

    for (idx_t step = 1; step < n; ++step) {
        /* find current maximal gap size with non‑zero frequency */
        while (cur_max > 0 && freq[cur_max] == 0)
            --cur_max;

        idx_t m = freq[cur_max];         // number of maximal gaps
        if (m == 0) break;               // safety check

        /* compute number of choices for this step */
        seq_t choices = (seq_t)m;
        if ((cur_max & 1ULL) == 0)       // even maximal gap size
            choices *= 2;

        result *= choices;                // update product

        /* remove one maximal gap */
        --freq[cur_max];

        /* split the removed gap */
        if (cur_max == 1) {
            /* no new gaps are created */
        } else if ((cur_max & 1ULL) == 0) {
            /* even case: split into sizes a and b */
            idx_t a = cur_max / 2 - 1;   // may be zero
            idx_t b = cur_max / 2;
            if (a > 0) freq[a] += 1;     // only add if positive
            freq[b] += 1;
        } else {
            /* odd case: split into two equal gaps */
            idx_t k = (cur_max - 1) / 2;
            freq[k] += 2;
        }
    }

    free(freq);
    return result;
}
