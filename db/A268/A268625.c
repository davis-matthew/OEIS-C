/*
 * A268625  Number of n X 5 0..2 arrays with some element plus some
 *          horizontally or vertically adjacent neighbor totaling two no more
 *          than once.
 * https://oeis.org/A268625
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 32*a(n-1) - 386*a(n-2) + 2264*a(n-3) - 7265*a(n-4) +
 *            13512*a(n-5) - 14960*a(n-6) + 9872*a(n-7) - 3776*a(n-8) +
 *            768*a(n-9) - 64*a(n-10) (empirical)
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 7, u64 n <= 16, u128 n <= 35,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <string.h>

/* Pre‑computed data for the transfer matrix (243 patterns). */
static int pat_val[243][5];
static unsigned char hcount[243];                 /* horizontal bad edges in a row */
static unsigned char vert[243][243];             /* vertical bad edges between rows */

/* Cache of the first ten terms. The array is 1‑based: a_cache[i] is a(i). */
static seq_t a_cache[11];
static int a_cache_ready = 0;

/* Recurrence coefficients */
static const int coeff[10] = {
    32,   -386,   2264,   -7265,   13512,
    -14960, 9872, -3776,  768,   -64
};

/* ------------------------------------------------------------------ */
/* Helper function: initialise pattern tables and first ten terms. */
static void compute_base_terms(void)
{
    /* Build pattern values and horizontal counts. */
    for (int p = 0; p < 243; ++p) {
        int x = p;
        int hc = 0;
        for (int c = 0; c < 5; ++c) {
            pat_val[p][c] = x % 3;
            x /= 3;
        }
        for (int c = 0; c < 4; ++c)
            if (pat_val[p][c] + pat_val[p][c + 1] == 2)
                ++hc;
        hcount[p] = (unsigned char)hc;
    }

    /* Build vertical adjacency table. */
    for (int u = 0; u < 243; ++u) {
        for (int v = 0; v < 243; ++v) {
            int vcnt = 0;
            for (int c = 0; c < 5; ++c)
                if (pat_val[u][c] + pat_val[v][c] == 2)
                    ++vcnt;
            vert[u][v] = (unsigned char)vcnt;
        }
    }

    /* DP arrays: dp[pattern][k] – number of ways with k bad edges (k=0,1). */
    static seq_t dp[243][2];
    static seq_t newdp[243][2];
    memset(dp, 0, sizeof(dp));

    /* Row 1 initialisation. */
    for (int p = 0; p < 243; ++p) {
        unsigned char h = hcount[p];
        if (h <= 1)
            dp[p][h] = (seq_t)1;
    }
    /* Store a(1). */
    a_cache[1] = (seq_t)0;
    for (int p = 0; p < 243; ++p) {
        a_cache[1] += dp[p][0];
        a_cache[1] += dp[p][1];
    }

    /* Subsequent rows up to row 10. */
    for (int row = 2; row <= 10; ++row) {
        memset(newdp, 0, sizeof(newdp));
        for (int prev = 0; prev < 243; ++prev) {
            for (int curr = 0; curr < 243; ++curr) {
                int add = hcount[curr] + vert[curr][prev];
                if (add > 1) continue;                /* too many bad edges */
                for (int kprev = 0; kprev <= 1; ++kprev) {
                    if (!dp[prev][kprev]) continue;
                    int nk = kprev + add;
                    if (nk <= 1) {
                        newdp[curr][nk] += dp[prev][kprev];
                    }
                }
            }
        }
        memcpy(dp, newdp, sizeof(dp));
        /* Store a(row). */
        a_cache[row] = (seq_t)0;
        for (int p = 0; p < 243; ++p) {
            a_cache[row] += dp[p][0];
            a_cache[row] += dp[p][1];
        }
    }

    a_cache_ready = 1;
}

/* ------------------------------------------------------------------ */
/* Main sequence function. */
seq_t a(idx_t n)
{
    if (!a_cache_ready)
        compute_base_terms();

    if (n <= 10)
        return a_cache[n];

    /* Buffer holding the last ten values: buf[0] = a(k-10), ..., buf[9] = a(k-1). */
    static seq_t buf[10];
    for (int i = 0; i < 10; ++i)
        buf[i] = a_cache[i + 1];

    seq_t val = 0;
    for (idx_t k = 11; k <= n; ++k) {
        seq_t pos = 0, neg = 0;
        for (int j = 0; j < 10; ++j) {
            int c = coeff[j];
            seq_t prod = buf[9 - j] * (seq_t)(c >= 0 ? c : -c);
            if (c >= 0)
                pos += prod;
            else
                neg += prod;
        }
        val = pos - neg;

        /* Shift buffer for next iteration. */
        for (int i = 0; i < 9; ++i)
            buf[i] = buf[i + 1];
        buf[9] = val;
    }
    return val;
}
