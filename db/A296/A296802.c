/*
 * A296802  Number of n X 6 0..1 arrays with each 1 adjacent to 1, 3 or 4
 *          king-move neighboring 1's.
 * https://oeis.org/A296802
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   Count of n×6 binary arrays where every 1 has 1, 3 or 4 king‑move
 *            neighboring 1’s. The algorithm uses a transfer matrix over pairs
 *            of consecutive rows (state space 64² = 4096). For each pair
 *            (X,Y) it precomputes the list of rows Z that allow Y to be
 *            satisfied when X is above Y and Z is below Y. Th...
 * Range:     u8 n <= 1, u16 n <= 4, u32 n <= 8, u64 n <= 15, u128 n <= 31,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <string.h>

/* pre–computed tables ********************************************************/
static unsigned char lr[64][6];                 /* left/right neighbor count in row */
static unsigned char upc[64][64][6];            /* count of 1's in X at j-1,j,j+1  */
static unsigned char downc[64][64][6];          /* count of 1's in Z at j-1,j,j+1  */
static unsigned char adj_cnt[4096];             /* number of neighbours for each state */
static unsigned char adj[4096][64];            /* neighbour rows                               */

/* auxiliary: build all tables once ------------------------------------------------*/
static void precompute_tables(void) {
    /* lr: neighbour count within the same row */
    for (int y = 0; y < 64; ++y) {
        for (int j = 0; j < 6; ++j) {
            unsigned char cnt = 0;
            if ((y >> (j - 1) & 1) && j > 0) cnt++;      /* left */
            if ((y >> (j + 1) & 1) && j < 5) cnt++;      /* right */
            lr[y][j] = cnt;
        }
    }

    /* upc: neighbors in the row above (X) for each Y */
    for (int x = 0; x < 64; ++x) {
        for (int y = 0; y < 64; ++y) {
            for (int j = 0; j < 6; ++j) {
                unsigned char cnt = 0;
                if (j > 0)   cnt += ((x >> (j - 1)) & 1);
                cnt += ((x >> j) & 1);
                if (j < 5)   cnt += ((x >> (j + 1)) & 1);
                upc[x][y][j] = cnt;
            }
        }
    }

    /* downc: neighbors in the row below (Z) for each Y */
    for (int y = 0; y < 64; ++y) {
        for (int z = 0; z < 64; ++z) {
            for (int j = 0; j < 6; ++j) {
                unsigned char cnt = 0;
                if (j > 0)   cnt += ((z >> (j - 1)) & 1);
                cnt += ((z >> j) & 1);
                if (j < 5)   cnt += ((z >> (j + 1)) & 1);
                downc[y][z][j] = cnt;
            }
        }
    }

    /* adjacency lists for every pair (X,Y) */
    for (int x = 0; x < 64; ++x) {
        for (int y = 0; y < 64; ++y) {
            int idx = (x << 6) | y;
            unsigned char *list = adj[idx];
            unsigned char c = 0;
            for (int z = 0; z < 64; ++z) {
                bool ok = true;
                for (int j = 0; j < 6; ++j) {
                    if ((y >> j) & 1) {
                        int total = lr[y][j] + upc[x][y][j] + downc[y][z][j];
                        if (!(total == 1 || total == 3 || total == 4)) {
                            ok = false;
                            break;
                        }
                    }
                }
                if (ok) {
                    list[c++] = (unsigned char)z;
                }
            }
            adj_cnt[idx] = c;
        }
    }
}

/* main sequence function **********************************************************/
seq_t a(idx_t n)
{
    /* initialise once */
    static bool ready = false;
    if (!ready) {
        precompute_tables();
        ready = true;
    }
    if (n == 0) return (seq_t)0;   /* below offset – undefined *//*  */

    /* DP arrays */
    static seq_t ways[4096];
    static seq_t newways[4096];
    memset(ways, 0, sizeof(ways));

    /* initial states: X = 0 (virtual top row), Y = first row pattern */
    for (int y = 0; y < 64; ++y) {
        ways[y] = (seq_t)1;   /* idx = (0<<6)|y => index = y */
    }

    /* iterate for rows 2..n */
    for (idx_t step = 2; step <= n; ++step) {
        memset(newways, 0, sizeof(newways));
        for (int idx = 0; idx < 4096; ++idx) {
            seq_t val = ways[idx];
            if (val == (seq_t)0) continue;
            unsigned char outcnt = adj_cnt[idx];
            unsigned char *outlist = adj[idx];
            int y = idx & 63;          /* current Y */
            for (int k = 0; k < outcnt; ++k) {
                unsigned char z = outlist[k];
                int newidx = (y << 6) | z;
                newways[newidx] += val;
            }
        }
        /* swap arrays */
        memcpy(ways, newways, sizeof(ways));
    }

    /* final check – last row Y must be satisfied with above X and below 0 */
    seq_t result = (seq_t)0;
    for (int idx = 0; idx < 4096; ++idx) {
        seq_t val = ways[idx];
        if (val == (seq_t)0) continue;
        int x = idx >> 6;
        int y = idx & 63;
        bool ok = true;
        for (int j = 0; j < 6; ++j) {
            if ((y >> j) & 1) {
                int total = lr[y][j] + upc[x][y][j];   /* down neighbors are 0 */
                if (!(total == 1 || total == 3 || total == 4)) {
                    ok = false;
                    break;
                }
            }
        }
        if (ok) {
            result += val;
        }
    }
    return result;
}
