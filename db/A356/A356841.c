/*
 * A356841  Numbers k such that the k-th composition in standard order covers
 *          an interval of positive integers (gapless).
 * https://oeis.org/A356841
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 67, u16 n <= 67, u32 n <= 67, u64 n <= 67, u128 n <= 67,
 *            big any n
 * Validated: partial, pass, 67 terms through n = 67 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation: a(n) is the n-th non‑negative integer k such that the
   k-th composition in standard order (graded reverse‑lexicographic)
   is gapless, i.e. the multiset of its part sizes forms a set of
   consecutive integers. The algorithm enumerates k, extracts the
   composition from the binary representation of k and tests the
   gapless property. */

#include <stdint.h>
#include <stdbool.h>

/* Helper: test whether the composition corresponding to k is gapless. */
static bool is_gapless(idx_t k)
{
    /* empty composition k == 0 is considered gapless */
    if (k == 0) return true;

    /* gather parts from set bits of k */
    int parts[128];
    int parts_cnt = 0;
    int prev_pos = 0;          /* position of previous '1', start with 0 */
    int bit_pos = 1;           /* positions in the reversed binary string
                                 (1‑based) */
    idx_t t = k;
    while (t != 0) {
        if (t & (idx_t)1) {
            int cur_pos = bit_pos;
            parts[parts_cnt++] = cur_pos - prev_pos;
            prev_pos = cur_pos;
        }
        t >>= 1;
        ++bit_pos;
    }

    /* analyze distinct part sizes */
    int min_part = parts[0];
    int max_part = parts[0];
    bool seen[128] = {0};
    int distinct = 0;

    for (int i = 0; i < parts_cnt; ++i) {
        int p = parts[i];
        if (p < min_part) min_part = p;
        if (p > max_part) max_part = p;
        if (!seen[p]) {
            seen[p] = true;
            ++distinct;
        }
    }

    return distinct == (max_part - min_part + 1);
}

/* Main sequence function. */
seq_t a(idx_t n)
{
    idx_t count = 0;
    idx_t k = 0;

    for (;;) {
        if (is_gapless(k)) {
            ++count;
            if (count == n) {
                return (seq_t)k;
            }
        }
        ++k;
    }
}
