/*
 * A257015  Number of (n+2)X(2+2) 0..2 arrays with no 3x3 subblock diagonal
 *          sum equal to the antidiagonal sum.
 * https://oeis.org/A257015
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of (n+2)×4 arrays with entries 0..2 such that for
 *            every 3×3 subblock its main‑diagonal sum is not equal to its
 *            anti‑diagonal sum. The sequence satisfies a(n) =
 *            a(n-1)⋅E(n-1)+a(n-2)⋅E(n-2)+… where E(k) is the number of
 *            walk‑patterns of length k on a small graph. The implementation
 *            be...
 * Range:     u8 none, u16 none, u32 n <= 3, u64 n <= 9, u128 n <= 20,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
/* seq_t and idx_t are supplied by the build. */
/* Static helper structures – constructed on first call. */
static signed char diff0[81];
static signed char diff1[81];
static unsigned char adj[81][81];
static unsigned char deg[81];

/* Compute the number of sequences of rows of length m
   where consecutive rows at distance two form an allowed pair. */
static seq_t sequences_of_length(idx_t m)
{
    static seq_t v[81] = {0};
    static seq_t nv[81] = {0};
    /* initialize v to all 1’s (each single row sequence). */
    for (int i = 0; i < 81; ++i) {
        v[i] = 1;
        nv[i] = 0;
    }
    for (idx_t step = 1; step < m; ++step) {
        /* zero nv */
        for (int i = 0; i < 81; ++i) nv[i] = 0;
        for (int a = 0; a < 81; ++a) {
            seq_t val = v[a];
            if (val == 0) continue;
            unsigned char d = deg[a];
            for (unsigned int idx = 0; idx < d; ++idx) {
                nv[ adj[a][idx] ] += val;
            }
        }
        /* move nv to v for next iteration */
        for (int i = 0; i < 81; ++i) v[i] = nv[i];
    }
    /* sum all entries: total number of sequences of this length */
    seq_t total = 0;
    for (int i = 0; i < 81; ++i) total += v[i];
    return total;
}

seq_t a(idx_t n)
{
    /* one‑time initialization of differences and adjacency */
    static int initialized = 0;
    if (!initialized) {
        /* enumerate all 81 rows as 4‑digit base‑3 numbers */
        for (int idx = 0; idx < 81; ++idx) {
            int x = idx;
            int d[4];
            d[0] = x % 3; x /= 3;
            d[1] = x % 3; x /= 3;
            d[2] = x % 3; x /= 3;
            d[3] = x % 3;
            diff0[idx] = d[0] - d[2];
            diff1[idx] = d[1] - d[3];
            deg[idx] = 0;
        }
        /* build adjacency: a→c allowed iff diff0[a]!=diff0[c] && diff1[a]!=diff1[c] */
        for (int a = 0; a < 81; ++a) {
            for (int c = 0; c < 81; ++c) {
                if (diff0[a] != diff0[c] && diff1[a] != diff1[c]) {
                    adj[a][ deg[a]++ ] = (unsigned char)c;
                }
            }
        }
        initialized = 1;
    }

    idx_t L = n + 2;                   /* total number of rows */
    idx_t m_even = (L + 1) / 2;        /* ceil(L/2) – number of rows of even indices */
    idx_t m_odd  = L / 2;              /* floor(L/2) – number of rows of odd indices */

    seq_t cnt_even = sequences_of_length(m_even);
    seq_t cnt_odd  = sequences_of_length(m_odd);

    return cnt_even * cnt_odd;
}
