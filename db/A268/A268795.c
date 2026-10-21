/*
 * A268795  Number of nX5 0..2 arrays with some element plus some
 *          horizontally, vertically or antidiagonally adjacent neighbor
 *          totalling two exactly once.
 * https://oeis.org/A268795
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of n×5 arrays over {0,1,2} having exactly one
 *            horizontally, vertically, or anti‑diagonally adjacent pair whose
 *            values sum to 2. The function implements a dynamic program over
 *            row patterns
 * Range:     u8 n <= 1, u16 n <= 4, u32 n <= 10, u64 n <= 22, u128 n <= 46,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Dependencies:
   The type seq_t and idx_t are supplied by the build system.
   No external headers beyond those required for standard types. */
#include <stdbool.h>

#define PAT 243               /* 3^5 possible row patterns */
#define MAX_Q PAT

/* Precomputed tables */
static uint8_t digits[PAT][5];      /* base‑3 digits of a pattern  */
static uint8_t hsum[PAT];           /* horizontal sum‑2 pairs in a row  */
static uint8_t vcount[PAT][PAT];    /* vertical sum‑2 pairs between two rows */
static uint8_t adcount[PAT][PAT];   /* anti‑diagonal sum‑2 pairs between two rows */
static int zeroIdx[PAT][MAX_Q];
static int oneIdx[PAT][MAX_Q];
static int zeroCnt[PAT];
static int oneCnt[PAT];

static bool tables_ready = false;

/* Helper function to compute all tables once */
static void build_tables(void) {
    /* Fill digits and hsum */
    for (int p = 0; p < PAT; ++p) {
        int v = p;
        for (int col = 0; col < 5; ++col) {
            digits[p][col] = v % 3;
            v /= 3;
        }
        uint8_t h = 0;
        for (int i = 0; i < 4; ++i)
            if (digits[p][i] + digits[p][i + 1] == 2)
                ++h;
        hsum[p] = h;
    }

    /* Fill vcount, adcount and transition lists */
    for (int p = 0; p < PAT; ++p) {
        zeroCnt[p] = 0;
        oneCnt[p]   = 0;
        for (int q = 0; q < PAT; ++q) {
            /* Vertical */
            uint8_t v = 0;
            for (int i = 0; i < 5; ++i)
                if (digits[p][i] + digits[q][i] == 2)
                    ++v;
            vcount[p][q] = v;

            /* Anti‑diagonal */
            uint8_t a = 0;
            for (int i = 0; i < 4; ++i)
                if (digits[p][i + 1] + digits[q][i] == 2)
                    ++a;
            adcount[p][q] = a;

            uint8_t newPairs = hsum[q] + v + a;
            if (newPairs == 0) {
                zeroIdx[p][zeroCnt[p]++] = q;
            } else if (newPairs == 1) {
                oneIdx[p][oneCnt[p]++] = q;
            }
            /* newPairs > 1 are useless for the DP and ignored */
        }
    }
    tables_ready = true;
}

/* Main sequence function */
seq_t a(idx_t n) {
    if (n < 1) return 0;

    if (!tables_ready) build_tables();

    /* DP arrays: cur and nxt are pointers to 2‑dimensional arrays
       [PAT][2] of type seq_t */
    static seq_t dp_cur[PAT][2];
    static seq_t dp_next[PAT][2];
    seq_t (*cur)[2]  = dp_cur;
    seq_t (*nxt)[2]  = dp_next;

    /* Initialise cur for the first row */
    for (int p = 0; p < PAT; ++p) {
        cur[p][0] = cur[p][1] = 0;
        if (hsum[p] == 0) cur[p][0] = 1;
        else if (hsum[p] == 1) cur[p][1] = 1;
        /* patterns with hsum > 1 are discarded */
    }

    /* Process remaining rows */
    for (idx_t row = 2; row <= n; ++row) {
        /* zero nxt */
        for (int p = 0; p < PAT; ++p) {
            nxt[p][0] = nxt[p][1] = 0;
        }

        /* Transition */
        for (int p = 0; p < PAT; ++p) {
            /* From state used=0 */
            if (cur[p][0] != 0) {
                /* zero new pairs */
                for (int i = 0; i < zeroCnt[p]; ++i) {
                    int q = zeroIdx[p][i];
                    nxt[q][0] = nxt[q][0] + cur[p][0];
                }
                /* one new pair */
                for (int i = 0; i < oneCnt[p]; ++i) {
                    int q = oneIdx[p][i];
                    nxt[q][1] = nxt[q][1] + cur[p][0];
                }
            }
            /* From state used=1 */
            if (cur[p][1] != 0) {
                /* only zero new pairs allowed */
                for (int i = 0; i < zeroCnt[p]; ++i) {
                    int q = zeroIdx[p][i];
                    nxt[q][1] = nxt[q][1] + cur[p][1];
                }
            }
        }

        /* Prepare for next iteration */
        seq_t (*tmp)[2] = cur;
        cur = nxt;
        nxt = tmp;
    }

    /* Sum over states where exactly one pair has appeared */
    seq_t result = 0;
    for (int p = 0; p < PAT; ++p)
        result = result + cur[p][1];

    return result;
}
