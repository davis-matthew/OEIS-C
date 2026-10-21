/*
 * A241931  Number of length n+4 0..3 arrays with no consecutive five elements
 *          summing to more than 2*3.
 * https://oeis.org/A241931
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 5, u32 n <= 16, u64 n <= 36, u128 n <= 78,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: Count of length (n+4) arrays over {0,1,2,3} such that
 * no consecutive five elements have a sum greater than 6.
 * The implementation uses a DP over the last four digits (state space of
 * size 4^4 = 256).  Transitions are built once and reused for all calls.
 */

#include <stddef.h>
#include <stdbool.h>

enum { TRANS_MAX = 4 };          /* maximum outgoing transitions per state */

/* Transition tables, built once. */
static unsigned char sum4[256];
static unsigned char next_cnt[256];
static unsigned char next_state[256][TRANS_MAX];
static bool trans_built = false;

static void build_trans(void)
{
    for (int s = 0; s < 256; ++s) {
        unsigned a = (s >> 6) & 3;
        unsigned b = (s >> 4) & 3;
        unsigned c = (s >> 2) & 3;
        unsigned d =  s        & 3;
        unsigned cur_sum = a + b + c + d;
        sum4[s] = cur_sum;

        unsigned cnt = 0;
        for (unsigned nd = 0; nd < 4; ++nd) {
            if (cur_sum + nd <= 6) {
                unsigned ns = ((s << 2) & 0xFF) | nd;
                next_state[s][cnt++] = (unsigned char)ns;
            }
        }
        next_cnt[s] = (unsigned char)cnt;
    }
    trans_built = true;
}

seq_t a(idx_t n)
{
    if (n < 1)
        return 0;

    idx_t L = n + 4;          /* desired length of the array */

    if (!trans_built)
        build_trans();

    seq_t cur[256];
    seq_t nxt[256];

    /* Initial state: all 4‑digit prefixes are allowed. */
    for (size_t i = 0; i < 256; ++i)
        cur[i] = (seq_t)1;

    /* Extend the array from length 4 up to length L. */
    for (idx_t len = 5; len <= L; ++len) {
        for (size_t i = 0; i < 256; ++i)
            nxt[i] = 0;

        for (size_t s = 0; s < 256; ++s) {
            seq_t val = cur[s];
            if (!val)
                continue;
            unsigned cnt = next_cnt[s];
            for (unsigned k = 0; k < cnt; ++k) {
                unsigned ns = next_state[s][k];
                nxt[ns] += val;
            }
        }

        for (size_t i = 0; i < 256; ++i)
            cur[i] = nxt[i];
    }

    /* Sum all valid sequences of length L. */
    seq_t total = 0;
    for (size_t i = 0; i < 256; ++i)
        total += cur[i];

    return total;
}
