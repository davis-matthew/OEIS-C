/*
 * A250864  Number of (n+2)X(4+2) 0..3 arrays with nondecreasing sum of every
 *          three consecutive values in every row and column.
 * https://oeis.org/A250864
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = g(h0) * g(h1) * g(h2) * where h(m) = floor((n+2-1-m)/3)+1
 *            (m = 0,1,2) * and g(h) is the number of non‑decreasing chains of
 *            length h in the * poset of all 6‑tuples (x0…x5) of values 0…3
 *            that satisfy * * x0+x1+x2 ≤ x1+x2+x3 ≤ x2+x3+x4 ≤ x3+x4+x5 . * *
 *            The code pre‑computes g(h) for 0 ≤ h ≤ 3...
 * Range:     u8 none, u16 none, u32 n <= 1, u64 n <= 6, u128 n <= 26,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdbool.h>

/* maximum chain length needed for n ≤ 10000 (see comments) */
#define MAX_H 3334
#define T_SIZE 4096    /* number of 6‑tuples */

/* precalculated 4^d values for index arithmetic */
static const idx_t POW4[6] = {1, 4, 16, 64, 256, 1024};

/* static state ------------------------------------------ */
static bool initialized = false;

/* g[h] – number of chains of length h (g[0] unused) */
static seq_t g_array[MAX_H + 1];

/* DP arrays for the counting process */
static seq_t dp_current[T_SIZE];
static seq_t dp_next[T_SIZE];
static seq_t pref_dp[T_SIZE];

/* tuple validity and coordinates (0‑based index) */
static bool allowed[T_SIZE];
static unsigned char coords[T_SIZE][6];

/* ---------------------------------------------------------- */
static void init_sequence(void)
{
    /* 1. build allowed[] and coords[] */
    for (size_t v = 0; v < T_SIZE; ++v) {
        size_t tmp = v;
        for (int d = 0; d < 6; ++d) {
            coords[v][d] = (unsigned char)(tmp % 4);
            tmp /= 4;
        }
        int s0 = coords[v][0] + coords[v][1] + coords[v][2];
        int s1 = coords[v][1] + coords[v][2] + coords[v][3];
        int s2 = coords[v][2] + coords[v][3] + coords[v][4];
        int s3 = coords[v][3] + coords[v][4] + coords[v][5];
        allowed[v] = (s0 <= s1 && s1 <= s2 && s2 <= s3);
    }

    /* 2. initialise dp for h = 1 */
    seq_t total_h1 = 0;
    for (size_t i = 0; i < T_SIZE; ++i) {
        if (allowed[i]) {
            dp_current[i] = 1;
            ++total_h1;
        } else
            dp_current[i] = 0;
    }
    g_array[1] = total_h1;

    /* 3. incremental DP for h = 2 … MAX_H */
    for (idx_t h = 2; h <= MAX_H; ++h) {
        /* pref_dp = dp_current initially */
        for (size_t i = 0; i < T_SIZE; ++i)
            pref_dp[i] = dp_current[i];

        /* successive prefix sums along each of the 6 dimensions */
        for (int d = 0; d < 6; ++d) {
            idx_t stride = POW4[d];
            for (idx_t v = 0; v < T_SIZE; ++v) {
                if (coords[v][d] > 0) {
                    pref_dp[v] += pref_dp[v - stride];
                }
            }
        }

        /* compute next_dp and accumulate g[h] */
        seq_t total = 0;
        for (size_t i = 0; i < T_SIZE; ++i) {
            if (allowed[i]) {
                dp_next[i] = pref_dp[i];
                total += dp_next[i];
            } else
                dp_next[i] = 0;
        }
        g_array[h] = total;

        /* prepare for next iteration: dp_current = dp_next */
        for (size_t i = 0; i < T_SIZE; ++i)
            dp_current[i] = dp_next[i];
    }

    initialized = true;
}

/* ---------------------------------------------------------- */
seq_t a(idx_t n)
{
    if (!initialized)
        init_sequence();

    /* number of rows in the (n+2)×(4+2) array */
    idx_t N = n + 2;

    /* heights for the three residue classes modulo 3 */
    idx_t h[3];
    for (int m = 0; m < 3; ++m) {
        h[m] = ((N - 1 - (idx_t)m) / 3) + 1;
    }

    /* product of the three independent chain counts */
    seq_t result = 1;
    for (int m = 0; m < 3; ++m) {
        result *= g_array[h[m]];
    }
    return result;
}
