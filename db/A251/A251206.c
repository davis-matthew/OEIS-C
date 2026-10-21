/*
 * A251206  Number of (n+1)X(4+1) 0..2 arrays with every 2X2 subblock summing
 *          to a nonzero multiple of 2.
 * https://oeis.org/A251206
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of (n+1)×5 arrays over {0,1,2} such that every 2×2
 *            subblock sums to a nonzero even number. Computed by a
 *            dynamic‑programming over all 5‑digit rows; each row (243
 *            possibilities) is a state
 * Range:     u8 none, u16 n <= 2, u32 n <= 5, u64 n <= 12, u128 n <= 25,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>

/* There are 3^5 = 243 possible rows of length 5 with entries in {0,1,2}. */
#define ROWS 243

/* Encoded base‑3 digits of each row (most significant digit first). */
static unsigned char digits[ROWS][5];
/* Out‑degree of each row (number of admissible next rows). */
static int deg[ROWS];
/* start[i] is the index in neigh where row i's adjacency list begins;
   start[ROWS] is the total number of edges. */
static int start[ROWS + 1];
/* Flat adjacency list: neigh[start[i] .. start[i+1]-1] contains the indices of rows that can follow row i. */
static int *neigh = NULL;
/* Flag to guard one‑time initialization. */
static bool initialized = false;

/* One‑time construction of full adjacency graph. */
static void init(void)
{
    if (initialized)
        return;

    /* Encode all 5‑digit rows in base‑3. */
    for (int r = 0; r < ROWS; ++r) {
        int v = r;
        for (int c = 4; c >= 0; --c) {
            digits[r][c] = (unsigned char)(v % 3);
            v /= 3;
        }
    }

    /* First pass: determine admissible pairs and out‑degrees.
       As an upper bound we allocate ROWS*ROWS slots. */
    int maxPairs = ROWS * ROWS;
    int *from = (int *)malloc(maxPairs * sizeof(int));
    int *to   = (int *)malloc(maxPairs * sizeof(int));
    int pairCnt = 0;

    for (int i = 0; i < ROWS; ++i) {
        deg[i] = 0;
        for (int j = 0; j < ROWS; ++j) {
            bool ok = true;
            for (int k = 0; k < 4; ++k) {
                int s = digits[i][k] + digits[i][k+1]
                        + digits[j][k] + digits[j][k+1];
                /* Accept only even, non‑zero sums. */
                if (s == 0 || (s & 1)) { ok = false; break; }
            }
            if (ok) {
                from[pairCnt] = i;
                to[pairCnt]   = j;
                ++deg[i];
                ++pairCnt;
            }
        }
    }

    /* Build start offsets. */
    start[0] = 0;
    for (int i = 0; i < ROWS; ++i)
        start[i+1] = start[i] + deg[i];

    /* Allocate and fill the flat neighbour list. */
    neigh = (int *)malloc(pairCnt * sizeof(int));
    int *ptr = (int *)malloc(ROWS * sizeof(int));
    for (int i = 0; i < ROWS; ++i)
        ptr[i] = start[i];

    for (int k = 0; k < pairCnt; ++k) {
        int i = from[k];
        int j = to[k];
        neigh[ptr[i]++] = j;
    }

    free(ptr);
    free(from);
    free(to);

    initialized = true;
}

/* Main sequence function */
seq_t a(idx_t n)
{
    init();                       /* ensure adjacency is ready */

    /* Working arrays for the DP.  They are static so that they are
       allocated only once; each call reinitialises them. */
    static seq_t cnt[ROWS];
    static seq_t nxt[ROWS];

    /* Initialise counts for height 1 (single row). */
    for (int i = 0; i < ROWS; ++i)
        cnt[i] = 1;

    /* Apply the transition n times to obtain arrays of height n+1. */
    for (idx_t step = 1; step <= n; ++step) {
        /* Zero the next‑state buffer. */
        for (int i = 0; i < ROWS; ++i)
            nxt[i] = 0;

        /* Transition: for each current row, add its count to all
           admissible following rows. */
        for (int i = 0; i < ROWS; ++i) {
            seq_t val = cnt[i];
            if (val != 0) {
                for (int p = start[i]; p < start[i+1]; ++p) {
                    int j = neigh[p];
                    nxt[j] += val;
                }
            }
        }

        /* Copy nxt -> cnt for the next iteration. */
        for (int i = 0; i < ROWS; ++i)
            cnt[i] = nxt[i];
    }

    /* Sum all counts to obtain the total number of arrays. */
    seq_t result = 0;
    for (int i = 0; i < ROWS; ++i)
        result += cnt[i];
    return result;
}
