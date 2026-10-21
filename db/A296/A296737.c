/*
 * A296737  Number of nX6 0..1 arrays with each 1 adjacent to 1, 2 or 4
 *          king-move neighboring 1s.
 * https://oeis.org/A296737
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of n×6 binary arrays where each 1 has 1, 2 or 4
 *            king‑move neighbors
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 7, u64 n <= 14, u128 n <= 29,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implementation uses DP over pairs of consecutive rows.  */
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdbool.h>

/* 6 columns -> 64 possible row masks */
#define WIDTH 6
#define MAXMASK (1U << WIDTH)
#define STATECNT (MAXMASK * MAXMASK)

/* Precomputed transition lists: for each pair (prev,cur) the set of next rows
   that make cur valid.  */
static unsigned char next_mask[MAXMASK][MAXMASK][MAXMASK];
static unsigned char next_len[MAXMASK][MAXMASK];

/* Forward declaration */
static bool valid_row(unsigned char prev, unsigned char cur, unsigned char next);

/* Precompute the transition tables once */
static void init_transitions(void)
{
    static bool done = false;
    if (done) return;
    done = true;

    for (unsigned int prev = 0; prev < MAXMASK; ++prev) {
        for (unsigned int cur = 0; cur < MAXMASK; ++cur) {
            unsigned char *list = next_mask[prev][cur];
            unsigned char cnt = 0;
            for (unsigned int next = 0; next < MAXMASK; ++next) {
                if (valid_row((unsigned char)prev, (unsigned char)cur, (unsigned char)next)) {
                    list[cnt++] = (unsigned char)next;
                }
            }
            next_len[prev][cur] = cnt;
        }
    }
}

/* Check whether the row 'cur' is valid given the rows above (prev) and below (next).
   'cur' is a 6‑bit mask.  */
static bool valid_row(unsigned char prev, unsigned char cur, unsigned char next)
{
    /* For each 1 in cur, count its king‑move neighbors. */
    for (uint8_t j = 0; j < WIDTH; ++j) {
        if ((cur >> j) & 1U) {
            unsigned int cnt = 0;

            /* Same row neighbors */
            if (j > 0)   cnt += (cur >> (j-1)) & 1U;
            if (j < WIDTH-1) cnt += (cur >> (j+1)) & 1U;

            /* Neighbors in the row above */
            if (j > 0)   cnt += (prev >> (j-1)) & 1U;
            cnt        +=  (prev >> j)     & 1U;
            if (j < WIDTH-1) cnt += (prev >> (j+1)) & 1U;

            /* Neighbors in the row below */
            if (j > 0)   cnt += (next >> (j-1)) & 1U;
            cnt        +=  (next >> j)     & 1U;
            if (j < WIDTH-1) cnt += (next >> (j+1)) & 1U;

            /* Must be 1, 2 or 4 */
            if (cnt != 1 && cnt != 2 && cnt != 4) {
                return false;
            }
        }
    }
    return true;
}

/* Public function required by the harness */
seq_t a(idx_t n)
{
    /* Handle n < 1 is irrelevant for this problem */
    init_transitions();

    static seq_t dp[STATECNT];
    static seq_t ndp[STATECNT];
    seq_t *cur = dp;
    seq_t *nxt = ndp;

    /* initialise dp for row 1: prev = 0 */
    memset(cur, 0, sizeof(dp));
    for (unsigned int currow = 0; currow < MAXMASK; ++currow) {
        cur[currow] = 1;          /* prev=0 (index 0), cur=row */
    }

    /* Iterate rows 2 .. n */
    for (idx_t row = 2; row <= n; ++row) {
        memset(nxt, 0, sizeof(ndp));

        for (unsigned int prev = 0; prev < MAXMASK; ++prev) {
            unsigned int basePrev = prev * MAXMASK;
            for (unsigned int currow = 0; currow < MAXMASK; ++currow) {
                seq_t ways = cur[basePrev + currow];
                if (!ways) continue;
                unsigned char *list = next_mask[prev][currow];
                unsigned char len   = next_len[prev][currow];
                unsigned int baseCur = currow * MAXMASK;
                for (unsigned char k = 0; k < len; ++k) {
                    unsigned int nxtrow = list[k];
                    nxt[baseCur + nxtrow] += ways;
                }
            }
        }
        /* swap cur and nxt */
        seq_t *tmp = cur;
        cur = nxt;
        nxt = tmp;
    }

    /* Sum over final states where the bottom row (ndp) has no row below (next=0) */
    seq_t total = 0;
    for (unsigned int prev = 0; prev < MAXMASK; ++prev) {
        unsigned int basePrev = prev * MAXMASK;
        for (unsigned int currow = 0; currow < MAXMASK; ++currow) {
            seq_t ways = cur[basePrev + currow];
            if (!ways) continue;
            if (valid_row((unsigned char)prev, (unsigned char)currow, 0)) {
                total += ways;
            }
        }
    }
    return total;
}
