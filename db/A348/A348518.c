/*
 * A348518  Positive integers m with the property that there are 5 positive
 *          integers b_1 < b_2 < b_3 < b_4 < b_5 such that b_1 divides b_2,
 *          b_2 divides b_3, b_3 divides b_4, b_4 divides b_5, and m = b_1 +
 *          b_2 + b_3 + b_4 + b_5.
 * https://oeis.org/A348518
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 65, u16 n <= 65, u32 n <= 65, u64 n <= 65, u128 n <= 65,
 *            big any n
 * Validated: partial, pass, 65 terms through n = 65 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Pentaphile sequence
   a(n) = pentaphile[n-1]   for n <= P
   a(n) = 240 + (n - P)     for n >  P
   where P is the number of pentaphile numbers ≤ 240 (here P = 24+?).
   The sequence contains all integers ≥ 241.
   The first few terms match the OEIS listing:
   31, 39, 43, 45, 46, 47, 55, 57, 58, 59, 61, 62, 63, 64, 67, 69, 70,
   71, 73, 75, 76, 77, 78, 79, … , 241, 242, …
*/

#include <stdbool.h>
#include <stdlib.h>

/* Check whether m is a pentaphile number: there exist
   b1<b2<b3<b4<b5 with b1|b2|b3|b4|b5 and sum = m. */
static bool is_pentaphile(seq_t m)
{
    for (idx_t b1 = 1; b1 < m; ++b1) {
        for (idx_t b2 = 2*b1; b2 <= m; b2 += b1) {
            for (idx_t b3 = 2*b2; b3 <= m; b3 += b2) {
                for (idx_t b4 = 2*b3; b4 <= m; b4 += b3) {
                    /* b5 must be a multiple of b4 and greater than b4 */
                    for (idx_t b5 = 2*b4; b5 <= m; b5 += b4) {
                        seq_t s = b1 + b2 + b3 + b4 + b5;
                        if (s == m) return true;
                        if (s > m) break;   /* further b5 only increase sum */
                    }
                }
            }
        }
    }
    return false;
}

static bool pent_seq_computed = false;
static seq_t pent_seq[240];   /* maximum possible size */
static idx_t pent_count = 0;

/* Precompute all pentaphile numbers ≤ 240. */
static void compute_pent_seq(void)
{
    if (pent_seq_computed) return;
    for (idx_t m = 31; m <= 240; ++m) {
        if (is_pentaphile((seq_t)m)) {
            pent_seq[pent_count++] = (seq_t)m;
        }
    }
    pent_seq_computed = true;
}

seq_t a(idx_t n)
{
    compute_pent_seq();
    if (n <= pent_count) {
        return pent_seq[n-1];
    } else {
        /* All integers ≥ 241 are pentaphile numbers. */
        return (seq_t)(240 + (n - pent_count));
    }
}
