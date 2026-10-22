/*
 * A390928  Array listed by rows: row n is the primitive Pythagorean triple a
 *          < b < c with least perimeter a+b+c such that its inradius
 *          (a+b-c)/2 = n.
 * https://oeis.org/A390928
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabf
 * Formula:   not stated
 * Range:     u8 n <= 24, u16 n <= 765, u32 n <= 9804, u64 n <= 9804,
 *            u128 n <= 9804, big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A390928
    Array listed by rows: row n is the primitive Pythagorean triple
    a < b < c with least perimeter a+b+c such that its inradius
    (a+b-c)/2 = n.  The sequence is the flattened row-wise array.
    This function returns the value at position n (n >= 1) by
    computing the triple for the inradius r = (n-1)/3 + 1
    and returning the appropriate leg or hypotenuse. */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/gcd.h"

seq_t a(idx_t n)
{
    /* Determine the inradius r (row) and the column (0→a,1→b,2→c) */
    idx_t r = (n - 1) / 3 + 1;
    idx_t col = (n - 1) % 3;

    /* Variables to hold the best triple found for this inradius */
    seq_t best_perim = 0;
    bool found = false;
    seq_t a_val = 0, b_val = 0, c_val = 0;

    /* Enumerate all factor pairs d*k = r, where d ≤ k (loop up to sqrt(r)) */
    for (idx_t d = 1; d * d <= r; ++d) {
        if (r % d != 0) continue;

        idx_t k = r / d;          /* k = (x - y)  */
        /* Candidate 1:   y = d ,  k = r/d */
        if ((k & 1U) && gcd((seq_t)d, (seq_t)k) == 1) {
            idx_t y = d;
            idx_t x = y + k;

            seq_t perim = (seq_t)2 * (seq_t)x * ((seq_t)x + (seq_t)y);
            if (!found || perim < best_perim) {
                found = true;
                best_perim = perim;
                seq_t x2 = (seq_t)x * (seq_t)x;
                seq_t y2 = (seq_t)y * (seq_t)y;
                seq_t a_candidate = x2 - y2;           /* a = x^2 - y^2 */
                seq_t b_candidate = (seq_t)2 * (seq_t)x * (seq_t)y; /* b = 2xy */
                seq_t c_candidate = x2 + y2;           /* c = x^2 + y^2 */

                /* Ensure a < b < c */
                if (a_candidate <= b_candidate) {
                    a_val = a_candidate;
                    b_val = b_candidate;
                } else {
                    a_val = b_candidate;
                    b_val = a_candidate;
                }
                c_val = c_candidate;
            }
        }

        /* Candidate 2:   y = k ,  d = r/k  (skip duplicate when d==k) */
        if (d == k) continue;
        if ((d & 1U) && gcd((seq_t)k, (seq_t)d) == 1) {
            idx_t y = k;
            idx_t x = y + d;

            seq_t perim = (seq_t)2 * (seq_t)x * ((seq_t)x + (seq_t)y);
            if (!found || perim < best_perim) {
                found = true;
                best_perim = perim;
                seq_t x2 = (seq_t)x * (seq_t)x;
                seq_t y2 = (seq_t)y * (seq_t)y;
                seq_t a_candidate = x2 - y2;
                seq_t b_candidate = (seq_t)2 * (seq_t)x * (seq_t)y;
                seq_t c_candidate = x2 + y2;

                if (a_candidate <= b_candidate) {
                    a_val = a_candidate;
                    b_val = b_candidate;
                } else {
                    a_val = b_candidate;
                    b_val = a_candidate;
                }
                c_val = c_candidate;
            }
        }
    }

    /* Return the required part of the triple */
    switch (col) {
        case 0: return a_val;   /* smallest leg   */
        case 1: return b_val;   /* middle leg     */
        case 2: return c_val;   /* hypotenuse     */
        default: return 0;      /* unreachable    */
    }
}
