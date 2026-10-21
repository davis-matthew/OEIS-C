/*
 * A251214  Number of (n+1) X (3+1) 0..1 arrays with no 2 X 2 subblock having
 *          zero or two 1s.
 * https://oeis.org/A251214
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 6, u32 n <= 14, u64 n <= 29, u128 n <= 61,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
   OEIS A251214: Number of (n+1)×4 binary arrays with none of the 2×2 subblocks
   having 0 or 2 ones.
   Compute by dynamic programming on the 4-bit row states.
*/

#include <stdbool.h>

static unsigned char allowed[16][16];
static bool init = false;

static void compute_allowed(void)
{
    if (init) return;
    init = true;
    for (int s = 0; s < 16; ++s)
    {
        for (int t = 0; t < 16; ++t)
        {
            bool ok = true;
            for (int k = 0; k < 3; ++k)
            {
                int sum = ((s >> k) & 1) + ((s >> (k + 1)) & 1)
                        + ((t >> k) & 1) + ((t >> (k + 1)) & 1);
                if (sum == 0 || sum == 2)
                {
                    ok = false;
                    break;
                }
            }
            allowed[s][t] = ok ? 1 : 0;
        }
    }
}

seq_t a(idx_t n)
{
    compute_allowed();

    /* dp_prev[s] = number of sequences of current length ending in state s */
    seq_t dp_prev[16];
    seq_t dp_curr[16];

    for (int i = 0; i < 16; ++i) dp_prev[i] = 1;  /* first row arbitrary */

    for (idx_t step = 1; step <= n; ++step)
    {
        for (int i = 0; i < 16; ++i) dp_curr[i] = 0;

        for (int s = 0; s < 16; ++s)
        {
            if (dp_prev[s] == 0) continue;
            for (int t = 0; t < 16; ++t)
            {
                if (allowed[s][t])
                    dp_curr[t] += dp_prev[s];
            }
        }
        /* advance to next step */
        for (int i = 0; i < 16; ++i) dp_prev[i] = dp_curr[i];
    }

    /* sum over all ending states */
    seq_t result = 0;
    for (int i = 0; i < 16; ++i)
        result += dp_prev[i];
    return result;
}
