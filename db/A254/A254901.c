/*
 * A254901  Number of (n+2)X(2+2) 0..1 arrays with every 3X3 subblock sum of
 *          the two sums of the diagonal and antidiagonal minus the two
 *          minimums of the central column and central row nondecreasing
 *          horizontally, vertically and ne-to-sw antidiagonally.
 * https://oeis.org/A254901
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of (n+2)×4 binary arrays satisfying * the
 *            monotone‐window condition described in A254901. * It is computed
 *            by a linear‑time dynamic program over * triples of consecutive
 *            rows
 * Range:     u8 none, u16 n <= 4, u32 n <= 26, u64 n <= 72, u128 n <= 166,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <string.h>

/* ---------- Precomputation tables ---------- */

static uint8_t  S0[4096];
static uint8_t  S1[4096];
static uint8_t  horiz[4096];

static uint8_t  next_len[4096];
static uint16_t next_idx[4096][16];   /* at most 16 outgoing edges per state */

static int tables_ready = 0;

static void init_tables(void)
{
    if (tables_ready) return;

    for (int r0 = 0; r0 < 16; ++r0)
        for (int r1 = 0; r1 < 16; ++r1)
            for (int r2 = 0; r2 < 16; ++r2)
            {
                int idx = (r0 << 8) | (r1 << 4) | r2;

                /* window starting at column 0 */
                int d0 = ((r0 >> 0) & 1) + ((r1 >> 1) & 1) + ((r2 >> 2) & 1);
                int a0 = ((r0 >> 2) & 1) + ((r1 >> 1) & 1) + ((r2 >> 0) & 1);
                int minC0 = ((r0 >> 1) & 1) & ((r1 >> 1) & 1) & ((r2 >> 1) & 1);
                int minR0 = ((r1 >> 0) & 1) & ((r1 >> 1) & 1) & ((r1 >> 2) & 1);
                int s0 = d0 + a0 - minC0 - minR0;

                /* window starting at column 1 */
                int d1 = ((r0 >> 1) & 1) + ((r1 >> 2) & 1) + ((r2 >> 3) & 1);
                int a1 = ((r0 >> 3) & 1) + ((r1 >> 2) & 1) + ((r2 >> 1) & 1);
                int minC1 = ((r0 >> 2) & 1) & ((r1 >> 2) & 1) & ((r2 >> 2) & 1);
                int minR1 = ((r1 >> 1) & 1) & ((r1 >> 2) & 1) & ((r1 >> 3) & 1);
                int s1 = d1 + a1 - minC1 - minR1;

                S0[idx]   = (uint8_t)s0;
                S1[idx]   = (uint8_t)s1;
                horiz[idx] = (s0 <= s1) ? 1 : 0;
            }

    for (int t = 0; t < 4096; ++t)
    {
        int r0 = t >> 8;
        int r1 = (t >> 4) & 0xF;
        int r2 = t & 0xF;
        uint8_t len = 0;
        for (int nr3 = 0; nr3 < 16; ++nr3)
        {
            int nt = (r1 << 8) | (r2 << 4) | nr3;
            if (!horiz[nt]) continue;
            if (S0[nt] < S0[t]) continue;
            if (S1[nt] < S1[t]) continue;
            if (S0[nt] < S1[t]) continue;   /* anti‑diagonal */
            next_idx[t][len++] = (uint16_t)nt;
        }
        next_len[t] = len;
    }

    tables_ready = 1;
}

/* ---------- Main sequence function ---------- */

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* offset is 1, safety guard */

    init_tables();

    /* DP buffers */
    seq_t cur[4096];
    seq_t nxt[4096];
    seq_t *curp = cur;
    seq_t *nxtp = nxt;

    /* initial states: all horizontal‑valid triples */
    for (int i = 0; i < 4096; ++i)
        curp[i] = horiz[i] ? (seq_t)1 : (seq_t)0;

    /* number of transitions = (n+2) - 3 = n - 1 */
    idx_t steps = n > 0 ? n - 1 : 0;
    for (idx_t step = 0; step < steps; ++step)
    {
        memset(nxtp, 0, sizeof(nxt));

        for (int t = 0; t < 4096; ++t)
        {
            seq_t val = curp[t];
            if (!val) continue;
            uint8_t len = next_len[t];
            const uint16_t *adj = next_idx[t];
            for (uint8_t k = 0; k < len; ++k)
                nxtp[adj[k]] += val;
        }

        seq_t *tmp = curp;
        curp = nxtp;
        nxtp = tmp;
    }

    /* sum all final states */
    seq_t result = 0;
    for (int i = 0; i < 4096; ++i)
        result += curp[i];

    return result;
}
