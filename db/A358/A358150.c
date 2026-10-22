/*
 * A358150  Squares visited by a knight moving on a square-spiral numbered
 *          board where the knight moves to the smallest numbered unvisited
 *          square and where the square number is more than the number of
 *          currently visited squares.
 * https://oeis.org/A358150
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, walk
 * Formula:   This function simulates the knight walk on an integer‑valued
 *            square‑spiral board. The knight starts at square 1 (the center).
 *            At each step it moves to the smallest numbered unvisited square
 *            that it can reach with a standard knight move and whose number
 *            is larger than the count of squares already...
 * Range:     u8 n <= 60, u16 n <= 71, u32 n <= 71, u64 n <= 71, u128 n <= 71,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <string.h>

/* --- Configuration ------------------------------------------------------- */
#define MAXNUM    50000          /* number of spiral cells precomputed          */
#define RADIUS    130            /* grid offset to accommodate all coordinates   */
#define STRIDE   (2*RADIUS+1)    /* size of the 2‑D coordinate array             */
#define MAPSIZE  (STRIDE*STRIDE) /* flattened 2‑D array size                    */

/* --- Global tables ------------------------------------------------------ */
static int32_t  coordX[MAXNUM+1];          /* coordinate of number i           */
static int32_t  coordY[MAXNUM+1];
static int32_t  mapCoord[MAPSIZE];         /* number at coordinate (x,y)       */
static unsigned char visitedFlag[MAXNUM+1];/* 1 if the square has been visited */

/* Results of the walk:  a[0] = a(1), a[1] = a(2), … */
static seq_t    seqArr[200000];             /* capacity larger than needed  */
static size_t   seqLen = 0;                 /* current number of computed terms */

/* Static initialization guard */
static int      initialized = 0;

/* Knight moves */
static const int kx[8] = { 2, 1,-1,-2,-2,-1, 1, 2};
static const int ky[8] = { 1, 2, 2, 1,-1,-2,-2,-1};

/* ------------------------------------------------------------------------- */
/* Build the square‑spiral tables up to MAXNUM.                                 */
static void init_spiral(void) {
    if (initialized) return;

    /* ----- Generate coordinates for numbers 1 .. MAXNUM ------------------- */
    int x = 0, y = 0;
    size_t num = 1;
    coordX[num] = x;
    coordY[num] = y;

    int stepLen = 1;   /* number of steps on current leg   */
    int dir     = 0;   /* 0=right,1=up,2=left,3=down        */

    while (num < (size_t)MAXNUM) {
        for (int rep = 0; rep < 2; ++rep) {   /* two legs per stepLen  */
            int dx = 0, dy = 0;
            switch (dir) {
                case 0: dx =  1; dy =  0; break; /* right */
                case 1: dx =  0; dy =  1; break; /* up    */
                case 2: dx = -1; dy =  0; break; /* left  */
                case 3: dx =  0; dy = -1; break; /* down  */
            }
            for (int s = 0; s < stepLen && num < (size_t)MAXNUM; ++s) {
                x += dx;
                y += dy;
                ++num;
                coordX[num] = x;
                coordY[num] = y;
            }
            ++dir;
            if (dir == 4) dir = 0;
        }
        ++stepLen;
    }

    /* ----- Map coordinates back to numbers ------------------------------ */
    memset(mapCoord, 0, sizeof(mapCoord));
    for (size_t i = 1; i <= (size_t)MAXNUM; ++i) {
        int ix = coordX[i] + RADIUS;
        int iy = coordY[i] + RADIUS;
        mapCoord[(size_t)ix*STRIDE + iy] = (int32_t)i;
    }

    initialized = 1;
}

/* Coordinate → number lookup (returns 0 if not in table) */
static inline idx_t map_coord_to_num(int x, int y) {
    int ix = x + RADIUS;
    int iy = y + RADIUS;
    if (ix < 0 || ix >= STRIDE || iy < 0 || iy >= STRIDE) return 0;
    return (idx_t)mapCoord[(size_t)ix*STRIDE + iy];
}

/* ------------------------------------------------------------------------- */
/* Public function required by the exercise.                                */
seq_t a(idx_t n) {
    if (!initialized) init_spiral();

    /* Grow the computed sequence up to n terms                           */
    while (seqLen < (size_t)n) {
        if (seqLen == 0) {          /* first term a(1) = 1 */
            seqArr[0] = (seq_t)1;
            visitedFlag[1] = 1;
            ++seqLen;
            continue;
        }

        seq_t  cur = seqArr[seqLen-1];
        int32_t cx  = coordX[(size_t)cur];
        int32_t cy  = coordY[(size_t)cur];
        size_t  vis = seqLen;            /* number of already visited squares */

        seq_t best = 0;                  /* 0 means no candidate found */

        for (int m = 0; m < 8; ++m) {
            int nx = cx + kx[m];
            int ny = cy + ky[m];
            idx_t num = map_coord_to_num(nx, ny);
            if (!num)          continue;          /* outside precomputed area */
            if (num <= vis)   continue;           /* not larger than visited count */
            if (visitedFlag[(size_t)num]) continue;/* already visited */
            if (best == 0 || num < best)          /* smallest qualifying number */
                best = (seq_t)num;
        }

        if (best == 0) break;                 /* no valid move, sequence ends */

        seqArr[seqLen] = best;
        visitedFlag[(size_t)best] = 1;
        ++seqLen;
    }

    /* Return the requested term (1‑based).  If n is beyond the sequence the
       function will return 0 (unused in the test set). */
    return seqArr[(size_t)n-1];
}
