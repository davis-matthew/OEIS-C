/*
 * A295200  Number of nX3 0..1 arrays with each 1 horizontally or vertically
 *          adjacent to 2 or 4 1s.
 * https://oeis.org/A295200
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 8, u16 n <= 16, u32 n <= 32, u64 n <= 64, u128 n <= 128,
 *            big any n
 * Helpers:   cell_valid, compute_valid
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A295200: Number of n×3 binary arrays with each 1 horizontally or
   vertically adjacent to 2 or 4 1s.
   Recurrence: a(n) = 2*a(n‑1) – a(n‑2) + 2*a(n‑3) + a(n‑4) – 2*a(n‑5)
   with initial values a(1)=1, a(2)=3, a(3)=8, a(4)=14, a(5)=25.
   The algorithm below uses a transfer‑matrix DP with 64 states (pairs of
   consecutive rows) and works for any n≥1.  It is O(n) time and O(1) memory. */

#include <stdbool.h>
#include "../utils/cell_valid.h"
#include "../utils/compute_valid.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0;                /* offset starts at 1; unused case */

    /* Static validity table, computed on first call. */
    static bool valid[8][8][8];
    static bool valid_computed = false;
    if (!valid_computed)
    {
        compute_valid(valid);
        valid_computed = true;
    }

    /* DP arrays: key = prev*8 + cur   (64 states). */
    seq_t dp[64]  = {0};
    seq_t ndp[64] = {0};

    /* Initialise with a dummy top row of zeros. */
    for (int cur = 0; cur < 8; ++cur)
        dp[cur] = 1;          /* prev = 0 */

    /* Process rows 1 .. n-1 (i.e., we add a new row each step). */
    for (idx_t step = 1; step < n; ++step)
    {
        for (int i = 0; i < 64; ++i) ndp[i] = 0;
        for (int idx = 0; idx < 64; ++idx)
        {
            seq_t count = dp[idx];
            if (!count) continue;
            int prev = idx >> 3;      /* /8 */
            int cur  = idx & 7;        /* %8 */
            for (int next = 0; next < 8; ++next)
            {
                if (!valid[prev][cur][next]) continue;
                int nidx = (cur << 3) | next;   /* cur*8 + next */
                ndp[nidx] += count;
            }
        }
        /* swap */
        for (int i = 0; i < 64; ++i) dp[i] = ndp[i];
    }

    /* Final step: add dummy bottom row of zeros and sum valid endings. */
    seq_t result = 0;
    for (int idx = 0; idx < 64; ++idx)
    {
        seq_t count = dp[idx];
        if (!count) continue;
        int prev = idx >> 3;   /* /8 */
        int cur  = idx & 7;     /* %8 */
        if (valid[prev][cur][0]) result += count;
    }
    return result;
}
