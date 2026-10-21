/*
 * A252141  Number of (n+2)X(1+2) 0..3 arrays with every 3X3 subblock row and
 *          column sum equal to 0 3 5 6 or 7 and every 3X3 diagonal and
 *          antidiagonal sum not equal to 0 3 5 6 or 7.
 * https://oeis.org/A252141
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 7, u32 n <= 26, u64 n <= 63, u128 n <= 137,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Formula: a(n) = number of (n+2)×3 arrays with entries 0–3 such that
 * for every 3×3 subblock the row and column sums are in {0,3,5,6,7}
 * and the main and anti‑diagonal sums are NOT in that set.
 * Computation is performed by a transfer‑matrix dynamic programming
 * over overlapping 3‑row blocks.
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

static const bool allowed[10] = {
    true,            /* 0  */
    false,           /* 1  */
    false,           /* 2  */
    true,            /* 3  */
    false,           /* 4  */
    true,            /* 5  */
    true,            /* 6  */
    true,            /* 7  */
    false,           /* 8  */
    false            /* 9  */
};

static int rowSum[64];                     /* pre‑computed row sums */
static uint16_t outCnt[4096];
static uint16_t outDest[4096][64];          /* adjacency lists */

/* Decode a 3‑cell row from 0..63 into its cells (2‑bit each) */
static inline int cell(unsigned int r, int j) {
    return (r >> (j << 1)) & 3;
}

/* Test if a 3×3 block defined by rows r1,r2,r3 satisfies all constraints */
static bool blockValid(unsigned int r1, unsigned int r2, unsigned int r3) {
    /* Row sums */
    int rs1 = rowSum[r1], rs2 = rowSum[r2], rs3 = rowSum[r3];
    if (!allowed[rs1] || !allowed[rs2] || !allowed[rs3])
        return false;

    /* Column sums */
    int cs0 = cell(r1,0) + cell(r2,0) + cell(r3,0);
    int cs1 = cell(r1,1) + cell(r2,1) + cell(r3,1);
    int cs2 = cell(r1,2) + cell(r2,2) + cell(r3,2);
    if (!allowed[cs0] || !allowed[cs1] || !allowed[cs2])
        return false;

    /* Diagonals must NOT be in the allowed set */
    int diag   = cell(r1,0) + cell(r2,1) + cell(r3,2);
    int anti   = cell(r1,2) + cell(r2,1) + cell(r3,0);
    if (allowed[diag] || allowed[anti])
        return false;

    return true;
}

/* Build the transfer matrix once */
static void buildAdjacency(void) {
    for (unsigned int r = 0; r < 64; ++r) {
        int a = (r >> 0) & 3, b = (r >> 2) & 3, c = (r >> 4) & 3;
        rowSum[r] = a + b + c;
    }

    for (int s = 0; s < 4096; ++s) {
        unsigned int r1 = s >> 6;
        unsigned int r2 = s & 63;
        for (int r3 = 0; r3 < 64; ++r3) {
            if (blockValid(r1, r2, r3)) {
                outDest[s][outCnt[s]++] = (unsigned int)((r2 << 6) | r3);
            }
        }
    }
}

seq_t a(idx_t n) {
    static seq_t dpCur[4096];
    static seq_t dpNext[4096];
    static bool ready = false;
    if (!ready) {
        buildAdjacency();
        ready = true;
    }

    /* initialize dpCur to zero */
    for (int i = 0; i < 4096; ++i)
        dpCur[i] = 0;

    /* initial block (n == 1) */
    for (int r1 = 0; r1 < 64; ++r1) {
        for (int r2 = 0; r2 < 64; ++r2) {
            for (int r3 = 0; r3 < 64; ++r3) {
                if (blockValid(r1, r2, r3)) {
                    unsigned int state = ((unsigned int)r2 << 6) | (unsigned int)r3;
                    dpCur[state] += 1;          /* seq_t addition */
                }
            }
        }
    }

    if (n == 1) {
        seq_t sum = 0;
        for (int i = 0; i < 4096; ++i)
            sum += dpCur[i];
        return sum;
    }

    /* dynamic programming for blocks 2..n */
    for (idx_t step = 2; step <= n; ++step) {
        /* zero dpNext */
        for (int i = 0; i < 4096; ++i)
            dpNext[i] = 0;

        for (int s = 0; s < 4096; ++s) {
            seq_t val = dpCur[s];
            if (val == 0) continue;
            int cnt = outCnt[s];
            for (int k = 0; k < cnt; ++k) {
                unsigned int dest = outDest[s][k];
                dpNext[dest] += val;
            }
        }
        /* swap dpCur and dpNext */
        for (int i = 0; i < 4096; ++i)
            dpCur[i] = dpNext[i];
    }

    seq_t sum = 0;
    for (int i = 0; i < 4096; ++i)
        sum += dpCur[i];
    return sum;
}
