/*
 * A250929  Number of (n+1)X(2+1) 0..3 arrays with no 2X2 subblock having a
 *          diagonal absolute difference less than its antidiagonal absolute
 *          difference.
 * https://oeis.org/A250929
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of (n+1)×3 arrays over {0,1,2,3} with no 2×2
 *            subblock having a diagonal absolute difference less than its
 *            anti-diagonal difference
 * Range:     u8 none, u16 n <= 2, u32 n <= 5, u64 n <= 12, u128 n <= 25,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The sequence is computed exactly: for n ≤ 16 we use a direct DP
   over all 64 possible rows; for n > 16 we apply the proven
   16‑term linear recurrence

   a(n) = 40*a(n-1) -475*a(n-2) +3392*a(n-3) -16134*a(n-4)
          +44165*a(n-5) -44214*a(n-6) -88218*a(n-7) +319624*a(n-8)
          -393886*a(n-9) +208973*a(n-10) +26874*a(n-11)
          -121797*a(n-12) +92658*a(n-13) -36825*a(n-14)
          +5454*a(n-15) +752*a(n-16)   */

/* The implementation uses only the integer operations allowed
   in the problem statement. */

#include <stdbool.h>

static bool adjacency[64][64];
static bool adjacency_done = false;

static bool valid_transition(unsigned u, unsigned v)
{
    int a0 =  u        & 3;
    int a1 = (u >> 2)  & 3;
    int a2 = (u >> 4)  & 3;
    int b0 =  v        & 3;
    int b1 = (v >> 2)  & 3;
    int b2 = (v >> 4)  & 3;

    int d01 = a0 - b1;  if (d01 < 0) d01 = -d01;
    int d10 = a1 - b0;  if (d10 < 0) d10 = -d10;
    if (d01 < d10) return false;          /* columns 0‑1 */

    int d12 = a1 - b2;  if (d12 < 0) d12 = -d12;
    int d21 = a2 - b1;  if (d21 < 0) d21 = -d21;
    if (d12 < d21) return false;          /* columns 1‑2 */

    return true;
}

static void init_adjacency(void)
{
    if (adjacency_done) return;
    for (unsigned u = 0; u < 64; ++u)
        for (unsigned v = 0; v < 64; ++v)
            adjacency[u][v] = valid_transition(u, v);
    adjacency_done = true;
}

static seq_t init_vals[17];   /* init_vals[1]..init_vals[16] */
static bool init_done = false;

static void init_values(void)
{
    if (init_done) return;
    init_adjacency();

    seq_t dp[64];
    seq_t next[64];

    for (unsigned i = 0; i < 64; ++i) dp[i] = 1; /* 1‑row arrays */

    for (idx_t step = 1; step <= 16; ++step) {
        for (unsigned i = 0; i < 64; ++i) next[i] = 0;
        for (unsigned u = 0; u < 64; ++u) {
            seq_t du = dp[u];
            if (!du) continue;
            for (unsigned v = 0; v < 64; ++v)
                if (adjacency[u][v])
                    next[v] += du;
        }
        for (unsigned i = 0; i < 64; ++i) dp[i] = next[i];
        seq_t sum = 0;
        for (unsigned i = 0; i < 64; ++i) sum += dp[i];
        init_vals[step] = sum;
    }
    init_done = true;
}

seq_t a(idx_t n)
{
    if (n < 1) return 0;            /* outside the defined offset */
    init_values();

    if (n <= 16) return init_vals[n];

    /* recurrence coefficients */
    const int coeffs[16] = {
        40, -475, 3392, -16134,
        44165, -44214, -88218, 319624,
        -393886, 208973, 26874, -121797,
        92658, -36825, 5454, 752
    };

    /* last[0] = a(cur-1), last[1] = a(cur-2), ..., last[15] = a(cur-16) */
    seq_t last[16];
    for (int i = 0; i < 16; ++i) last[i] = init_vals[16 - i];

    for (idx_t cur = 17; cur <= n; ++cur) {
        seq_t pos = 0, neg = 0;
        for (int k = 0; k < 16; ++k) {
            int c = coeffs[k];
            seq_t prod = last[k] * (seq_t)(c > 0 ? c : -c);
            if (c > 0) pos += prod;
            else       neg += prod;
        }
        /* safe because the recurrence guarantees pos >= neg */
        seq_t next = pos - neg;

        /* shift window */
        for (int i = 15; i > 0; --i) last[i] = last[i-1];
        last[0] = next;
    }
    return last[0];
}
