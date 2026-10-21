/*
 * A254852  Number of (n+2)X(6+2) 0..1 arrays with no 3x3 subblock diagonal
 *          sum one and no antidiagonal sum two.
 * https://oeis.org/A254852
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 2, u32 n <= 11, u64 n <= 29, u128 n <= 66,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation: DP over rows using a transfer‑matrix of size 2^16; constraints
   are evaluated per 3×3 block in the binary matrix. */
#include <stdint.h>
#include <stddef.h>

/* Precomputed mask of allowed next rows for each pair (prev2, prev1).
   The pair is packed as (prev2 << 8) | prev1 and gives a 256‑bit mask
   stored in four 64‑bit words. */
static uint64_t allowed_mask[65536][4];

/* Flag to ensure precomputation happens only once. */
static int precomp_done = 0;

/* Precomputation of the allowed masks. */
static void precompute_allowed(void)
{
    for (int prev2 = 0; prev2 < 256; ++prev2) {
        for (int prev1 = 0; prev1 < 256; ++prev1) {
            uint64_t mask[4] = {0, 0, 0, 0};
            for (int newrow = 0; newrow < 256; ++newrow) {
                int ok = 1;
                for (int j = 0; j < 6; ++j) {
                    int a_j  = (prev2 >> j) & 1;
                    int a_j2 = (prev2 >> (j + 2)) & 1;
                    int b_j1 = (prev1 >> (j + 1)) & 1;
                    int c_j  = (newrow >> j) & 1;
                    int c_j2 = (newrow >> (j + 2)) & 1;

                    int main_sum = a_j + b_j1 + c_j2;
                    if (main_sum == 1) { ok = 0; break; }

                    int ant_sum = a_j2 + b_j1 + c_j;
                    if (ant_sum == 2) { ok = 0; break; }
                }
                if (ok) {
                    mask[newrow >> 6] |= ((uint64_t)1 << (newrow & 63));
                }
            }
            unsigned idx = (prev2 << 8) | prev1;
            allowed_mask[idx][0] = mask[0];
            allowed_mask[idx][1] = mask[1];
            allowed_mask[idx][2] = mask[2];
            allowed_mask[idx][3] = mask[3];
        }
    }
    precomp_done = 1;
}

/* Storage for DP arrays and active state lists. */
static seq_t dp0[65536], dp1[65536];
static uint32_t act[65536], next_act[65536];

/* Main function required by the problem. */
seq_t a(idx_t n)
{
    /* Ensure masks are ready. */
    if (!precomp_done)
        precompute_allowed();

    /* Total number of rows in the matrix. */
    const idx_t total_rows = n + 2;

    /* Pointers to current and next DP arrays. */
    seq_t *dp  = dp0, *nextdp = dp1;
    /* Active state lists. */
    uint32_t *active       = act;
    uint32_t *next_active  = next_act;
    size_t active_count    = 0, next_active_count = 0;

    /* Initialise DP with all 65536 pairs of initial rows. */
    for (size_t i = 0; i < 65536; ++i) dp[i] = 0;
    for (int r2 = 0; r2 < 256; ++r2)
        for (int r1 = 0; r1 < 256; ++r1) {
            unsigned idx = (r2 << 8) | r1;
            dp[idx] = 1;
            active[active_count++] = idx;
        }

    /* Add rows one by one. */
    for (idx_t step = 2; step < total_rows; ++step) {
        /* Zero next DP. */
        for (size_t i = 0; i < 65536; ++i) nextdp[i] = 0;
        next_active_count = 0;

        for (size_t ai = 0; ai < active_count; ++ai) {
            unsigned idx  = active[ai];
            seq_t val     = dp[idx];
            if (val == 0) continue;

            unsigned prev1 = idx & 0xFF;
            unsigned prev2 = idx >> 8;

            /* Iterate over allowed next rows. */
            for (int seg = 0; seg < 4; ++seg) {
                uint64_t words = allowed_mask[idx][seg];
                while (words) {
                    unsigned bit  = __builtin_ctzll(words);
                    unsigned newr  = (seg << 6) | bit;
                    unsigned newidx = (prev1 << 8) | newr;

                    if (nextdp[newidx] == 0)
                        next_active[next_active_count++] = newidx;

                    nextdp[newidx] += val;
                    words &= words - 1;          /* clear lowest bit */
                }
            }
        }

        /* Swap DP arrays and activity lists. */
        seq_t *tmpdp = dp;        dp = nextdp;        nextdp = tmpdp;
        uint32_t *tmpact = active; active = next_active; next_active = tmpact;
        active_count = next_active_count;
    }

    /* Sum all counts for the final layer. */
    seq_t result = 0;
    for (size_t ai = 0; ai < active_count; ++ai)
        result += dp[active[ai]];
    return result;
}
