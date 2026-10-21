/*
 * A282881  Number of nX4 0..1 arrays with no 1 equal to more than one of its
 *          horizontal, vertical and antidiagonal neighbors, with the
 *          exception of exactly one element.
 * https://oeis.org/A282881
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of n×4 0..1 arrays with exactly one 1 that has
 *            more than one neighbour among its horizontal, vertical and
 *            anti‑diagonal neighbours, all other 1’s having at most one such
 *            neighbour
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 10, u64 n <= 21, u128 n <= 44,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The function uses a row‑by‑row dynamic programming that keeps, for each
   column of the previous row, whether the cell
   - is 0 (state 0)
   - is 1 with no neighbours (state 1)
   - is 1 with exactly one neighbour (state 2)
   - is 1 with two or more neighbours (state 3 – special)
   Only one cell may reach state 3 in the whole array. */

#include <stdint.h>

/* Pre‑computed transition tables */
static unsigned char trans_state[256][16];   /* new previous‑row state */
static unsigned char trans_special[256][16]; /* number of new specials added */

/* Build transition tables once */
static void init_trans(void) {
    static int initialized = 0;
    if (initialized) return;
    for (int s = 0; s < 256; ++s) {
        int p[4];
        p[0] =  s & 3;
        p[1] = (s >> 2) & 3;
        p[2] = (s >> 4) & 3;
        p[3] = (s >> 6) & 3;
        for (int m = 0; m < 16; ++m) {
            int bits[4];
            bits[0] = (m >> 0) & 1;
            bits[1] = (m >> 1) & 1;
            bits[2] = (m >> 2) & 1;
            bits[3] = (m >> 3) & 1;

            int specials = 0;

            /* Specials arising from the previous row cells */
            for (int c = 0; c < 4; ++c) {
                if (p[c] == 0) continue;          /* previous cell was 0 */
                int add = 0;
                if (bits[c] == 1)          add++;  /* vertical neighbour */
                if (c > 0 && bits[c - 1] == 1) add++; /* anti‑diagonal (down‑left) */
                if (p[c] == 1) {
                    if (add >= 2) specials++;     /* becomes state 3 */
                } else if (p[c] == 2) {
                    if (add >= 1) specials++;     /* becomes state 3 */
                }
                /* p[c] == 3 already special – nothing to add */
            }

            /* New state for the current row cells */
            int news = 0;
            for (int c = 0; c < 4; ++c) {
                int new_state = 0;
                if (bits[c] == 0) {
                    new_state = 0;
                } else {
                    int neigh = 0;
                    if (c > 0 && bits[c - 1] == 1) neigh++;          /* left */
                    if (c < 3 && bits[c + 1] == 1) neigh++;          /* right */
                    if (p[c] != 0) neigh++;                         /* vertical up */
                    if (c < 3 && p[c + 1] != 0) neigh++;             /* anti‑diagonal up‑right */
                    if (neigh == 0) new_state = 1;
                    else if (neigh == 1) new_state = 2;
                    else new_state = 3;
                    if (neigh >= 2) specials++;                     /* becomes special now */
                }
                news |= new_state << (c * 2);
            }

            trans_state[s][m] = (unsigned char)news;
            trans_special[s][m] = (unsigned char)specials;
        }
    }
    initialized = 1;
}

/* Main sequence function */
seq_t a(idx_t n)
{
    /* initialise transition tables once */
    init_trans();

    /* DP arrays: dp[state][flag]   flag = 0 (no special yet), 1 (special used) */
    static seq_t dp[256][2];
    static seq_t ndp[256][2];

    /* clear dp */
    for (int s = 0; s < 256; ++s) {
        dp[s][0] = 0;
        dp[s][1] = 0;
    }
    dp[0][0] = 1;   /* before any rows: all zero state, no special */

    for (idx_t row = 1; row <= n; ++row) {
        /* clear ndp */
        for (int s = 0; s < 256; ++s) {
            ndp[s][0] = 0;
            ndp[s][1] = 0;
        }
        for (int s = 0; s < 256; ++s) {
            for (int flag = 0; flag <= 1; ++flag) {
                seq_t val = dp[s][flag];
                if (val == 0) continue;
                for (int m = 0; m < 16; ++m) {
                    int added = trans_special[s][m];
                    int nf = flag + added;
                    if (nf > 1) continue;      /* would create more than one special */
                    int ns = trans_state[s][m];
                    ndp[ns][nf] = ndp[ns][nf] + val;
                }
            }
        }
        /* copy ndp -> dp for next iteration */
        for (int s = 0; s < 256; ++s) {
            dp[s][0] = ndp[s][0];
            dp[s][1] = ndp[s][1];
        }
    }

    /* sum over states with exactly one special (flag = 1) */
    seq_t ans = 0;
    for (int s = 0; s < 256; ++s) {
        ans = ans + dp[s][1];
    }
    return ans;
}
