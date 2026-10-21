/*
 * A296961  Number of nX6 0..1 arrays with each 1 adjacent to 2, 3 or 5
 *          king-move neighboring 1s.
 * https://oeis.org/A296961
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of n×6 binary arrays where each 1 has 2, 3, or 5
 *            king‑move neighboring 1s
 * Range:     u8 n <= 1, u16 n <= 4, u32 n <= 7, u64 n <= 15, u128 n <= 30,
 *            big any n
 * Helpers:   is_valid_triple
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include "../utils/is_valid_triple.h"

/* Precomputed transition masks: mask[prev2][prev1] has bit k set
   iff adding row k after prev2, prev1 is allowed. */
static uint64_t valid_mask[64][64];
static bool masks_ready = false;

/* Main sequence function */
seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* not defined below offset */

    /* --- 1. Build transition masks once -------------------------------- */
    if (!masks_ready) {
        for (unsigned int p2 = 0; p2 < 64; ++p2)
            for (unsigned int p1 = 0; p1 < 64; ++p1) {
                uint64_t m = 0;
                for (unsigned int cur = 0; cur < 64; ++cur)
                    if (is_valid_triple(p2, p1, cur))
                        m |= 1ULL << cur;
                valid_mask[p2][p1] = m;
            }
        masks_ready = true;
    }

    /* --- 2. DP arrays --------------------------------------------------- */
    static seq_t dp_prev[64 * 64];
    static seq_t dp_next[64 * 64];
    /* Initialise dp_prev to zero */
    for (int i = 0; i < 64 * 64; ++i) dp_prev[i] = 0;
    for (int arow = 0; arow < 64; ++arow)
        dp_prev[arow] = 1;     /* state (0, arow) with count 1 */

    /* --- 3. Process rows 2 .. n ---------------------------------------- */
    for (idx_t row = 2; row <= n; ++row) {
        for (int i = 0; i < 64 * 64; ++i) dp_next[i] = 0;
        for (unsigned int prev2 = 0; prev2 < 64; ++prev2) {
            unsigned int base = prev2 * 64;
            for (unsigned int prev1 = 0; prev1 < 64; ++prev1) {
                seq_t count = dp_prev[base + prev1];
                if (count == 0) continue;
                uint64_t m = valid_mask[prev2][prev1];
                while (m) {
                    unsigned int cur = __builtin_ctzll(m);
                    dp_next[prev1 * 64 + cur] += count;
                    m &= m - 1;
                }
            }
        }
        /* swap dp_prev and dp_next */
        for (int i = 0; i < 64 * 64; ++i) dp_prev[i] = dp_next[i];
    }

    /* --- 4. Final validation for last row (row n) ----------------------- */
    seq_t result = 0;
    for (unsigned int prev2 = 0; prev2 < 64; ++prev2)
        for (unsigned int prev1 = 0; prev1 < 64; ++prev1)
            if (is_valid_triple(prev2, prev1, 0))
                result += dp_prev[prev2 * 64 + prev1];

    return result;
}
