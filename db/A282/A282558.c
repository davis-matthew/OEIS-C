/*
 * A282558  Number of nX6 0..1 arrays with no 1 equal to more than two of its
 *          king-move neighbors, with the exception of exactly one element.
 * https://oeis.org/A282558
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of n×6 0‑1 matrices in which no 1 has more * than
 *            two king‑move neighbours except for exactly one 1
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 6, u64 n <= 13, u128 n <= 27,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*   offset 1 : a(1)=0, a(2)=652, … as listed in the OEIS entry. */

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

/* --------------------------------------------------------------------- */
/*  constants for the 6–column width                                       */
/* --------------------------------------------------------------------- */
#define W       6                     /* number of columns           */
#define PAT     (1U << W)             /* number of row patterns     */
#define STATES  (PAT * PAT)           /* pair of rows                */

/* --------------------------------------------------------------------- */
/*  precomputed tables                                                    */
/* --------------------------------------------------------------------- */
static unsigned char viol_cnt[PAT][PAT][PAT];    /* 0/1/2+ violating cells   */
static unsigned char zlist[STATES][PAT];        /* newrow indices with 0 viol */
static uint8_t       zcnt [STATES];             /* count of such rows        */
static unsigned char olist[STATES][PAT];        /* newrow indices with 1 viol */
static uint8_t       ocnt [STATES];             /* count of such rows        */
static bool          tbl_inited = false;

/* --------------------------------------------------------------------- */
/*  compute the number of 1‑cells in row 'row' that have >2 king‑move
 *  neighbours when the row above is 'up' and the row below is 'down'.
 * --------------------------------------------------------------------- */
static unsigned char compute_viol_cnt(unsigned int up,
                                      unsigned int row,
                                      unsigned int down)
{
    unsigned char viol = 0;
    for (unsigned int col = 0; col < W; ++col) {
        if ((row >> col) & 1U) {                       /* this cell is 1   */
            unsigned int neigh = 0;
            /* above */
            if ((up >> col) & 1U)        ++neigh;
            if (col > 0U && (up >> (col-1U)) & 1U)    ++neigh;
            if (col + 1U < W && (up >> (col+1U)) & 1U)++neigh;
            /* same row */
            if (col > 0U && (row >> (col-1U)) & 1U)   ++neigh;
            if (col + 1U < W && (row >> (col+1U)) & 1U)++neigh;
            /* below */
            if ((down >> col) & 1U)        ++neigh;
            if (col > 0U && (down >> (col-1U)) & 1U)    ++neigh;
            if (col + 1U < W && (down >> (col+1U)) & 1U)++neigh;
            if (neigh > 2U)                ++viol;
        }
    }
    return viol;      /* 0,1 or >1 (capped, we only care 0/1/non‑zero)   */
}

/* --------------------------------------------------------------------- */
/*  initialise the tables                                               */
/* --------------------------------------------------------------------- */
static void init_tables(void)
{
    if (tbl_inited) return;

    for (unsigned int up = 0; up < PAT; ++up)
        for (unsigned int row = 0; row < PAT; ++row)
            for (unsigned int down = 0; down < PAT; ++down) {
                unsigned char v = compute_viol_cnt(up, row, down);
                viol_cnt[up][row][down] = (v > 1U ? 2U : v); /* 0,1,2 */
            }

    /* Build lists of admissible new rows for zero/one violations.      */
    for (unsigned int p2 = 0; p2 < PAT; ++p2)
        for (unsigned int p1 = 0; p1 < PAT; ++p1) {
            unsigned int idx = p2 * PAT + p1;
            uint8_t   kz = 0, ko = 0;
            for (unsigned int nr = 0; nr < PAT; ++nr) {
                unsigned char v = viol_cnt[p2][p1][nr];
                if (v == 0U) {
                    zlist[idx][kz++] = (unsigned char)nr;
                } else if (v == 1U) {
                    olist[idx][ko++] = (unsigned char)nr;
                } /* v==2 ignored (invalid) */
            }
            zcnt[idx] = kz;
            ocnt[idx] = ko;
        }

    tbl_inited = true;
}

/* --------------------------------------------------------------------- */
/*  main sequence function                                               */
/* --------------------------------------------------------------------- */
seq_t a(idx_t n)
{
    init_tables();

    if (n < 2U) return (seq_t)0;           /* a(1)=0, below is never queried    */
    /* arrays for DP:  zero[] = number of ways to reach the pair with
     *                 zero violations so far; one[]  = exactly one viol.   */
    seq_t zero[STATES] = {0};
    seq_t one [STATES] = {0};

    /* initialise using the first two rows (row0, row1).                 */
    for (unsigned int r0 = 0; r0 < PAT; ++r0)
        for (unsigned int r1 = 0; r1 < PAT; ++r1) {
            unsigned char v = viol_cnt[0][r0][r1];   /* first row violation */
            unsigned int idx = r0 * PAT + r1;      /* pair index           */
            if (v == 0U)                           /* no violation yet     */
                zero[idx] += (seq_t)1;
            else if (v == 1U)                       /* first row violates    */
                one [idx] += (seq_t)1;
        }

    /* temporary arrays for next step                                 */
    seq_t zero_next[STATES] = {0};
    seq_t one_next [STATES] = {0};

    /* process all remaining rows (row index 2 … n‑1)                   */
    for (unsigned int row = 2U; row < n; ++row) {
        /* zero out next step arrays                                 */
        for (size_t i = 0; i < STATES; ++i) {
            zero_next[i] = (seq_t)0;
            one_next [i] = (seq_t)0;
        }

        for (unsigned int idx = 0; idx < STATES; ++idx) {
            seq_t cnt0 = zero[idx];
            seq_t cnt1 = one [idx];
            if (cnt0 == (seq_t)0 && cnt1 == (seq_t)0) continue;

            unsigned int prev1 = idx & 63;               /* rightmost row of the pair */

            /* transitions that preserve “zero violations”            */
            uint8_t *zl = zlist[idx];
            uint8_t  kz = zcnt[idx];
            for (uint8_t i = 0; i < kz; ++i) {
                unsigned int target = (prev1 << 6) | zl[i];
                zero_next[target] += cnt0;      /* from zero → zero */
                one_next [target] += cnt1;      /* from one  → one  */
            }

            /* transitions that produce the single violating row      */
            uint8_t *ol = olist[idx];
            uint8_t  ko = ocnt[idx];
            for (uint8_t i = 0; i < ko; ++i) {
                unsigned int target = (prev1 << 6) | ol[i];
                one_next [target] += cnt0;       /* zero → one    */
            }
        }

        /* copy results for next iteration                         */
        for (size_t i = 0; i < STATES; ++i) {
            zero[i] = zero_next[i];
            one[i]  = one_next [i];
        }
    }

    /* final row (index n‑1) has no below neighbour – evaluate it   */
    seq_t result = (seq_t)0;
    for (unsigned int idx = 0; idx < STATES; ++idx) {
        seq_t cnt0 = zero[idx];
        seq_t cnt1 = one [idx];
        if (cnt0 == (seq_t)0 && cnt1 == (seq_t)0) continue;

        unsigned int up   = idx >> 6;       /* second row of the pair */
        unsigned int row1 = idx & 63;       /* last row               */
        unsigned char v_last = viol_cnt[up][row1][0];   /* below absent   */

        /* exact one violation: either it happened earlier or now */
        if (v_last == 0U && cnt1 > (seq_t)0) result += cnt1;
        if (v_last == 1U && cnt0 > (seq_t)0) result += cnt0;
        /* any other case (including v_last>1) is discarded          */
    }

    return result;
}
