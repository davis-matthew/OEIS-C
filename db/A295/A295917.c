/*
 * A295917  Number of n X 7 0..1 arrays with each 1 adjacent to 0 or 3
 *          king-move neighboring 1s.
 * https://oeis.org/A295917
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 4, u32 n <= 9, u64 n <= 18, u128 n <= 37,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: Number of n × 7 0..1 arrays with each 1 adjacent to 0 or
 * 3 king‑move neighboring 1s.  a(1) = 34, a(2) = 277, … */
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

#define ROWS          7
#define MASKS_PER_ROW (1u << ROWS)      /* 128 */
#define STATES        (MASKS_PER_ROW * MASKS_PER_ROW)
#define MAX_TRANS     (MASKS_PER_ROW * MASKS_PER_ROW * MASKS_PER_ROW)

/* Transition tables – built once, then reused. */
static uint8_t  trans_c[MAX_TRANS];
static unsigned int  trans_start[STATES];
static unsigned short trans_len[STATES];
static unsigned char  trans_built = 0;

/*----------------------------------------------------------------------*/
/* Check that every 1 in the middle row b has 0 or 3 neighboring 1s
 * when the rows above and below are a and c. */
static inline bool valid_mid(uint8_t a, uint8_t b, uint8_t c)
{
    for (int j = 0; j < ROWS; ++j) {
        if (b & (1u << j)) {
            int neigh = 0;
            /* neighbors from row above a */
            if (j > 0   && (a & (1u << (j-1)))) neigh++;
            if          (a & (1u << j))            neigh++;
            if (j < ROWS-1 && (a & (1u << (j+1)))) neigh++;
            /* horizontal neighbors from same row b */
            if (j > 0   && (b & (1u << (j-1)))) neigh++;
            if (j < ROWS-1 && (b & (1u << (j+1)))) neigh++;
            /* neighbors from row below c */
            if (j > 0   && (c & (1u << (j-1)))) neigh++;
            if          (c & (1u << j))            neigh++;
            if (j < ROWS-1 && (c & (1u << (j+1)))) neigh++;
            if (neigh != 0 && neigh != 3) return false;
        }
    }
    return true;
}

/*--------------------------------------------------------------------*/
/* Build transition tables once.  Each state (a,b) stores all c such
 * that (a,b,c) satisfies the middle‑row condition. */
static void build_transitions(void)
{
    if (trans_built) return;
    unsigned int cur = 0;
    for (uint8_t a = 0; a < MASKS_PER_ROW; ++a) {
        for (uint8_t b = 0; b < MASKS_PER_ROW; ++b) {
            unsigned int idx = a * MASKS_PER_ROW + b;
            trans_start[idx] = cur;
            unsigned short cnt = 0;
            for (uint8_t c = 0; c < MASKS_PER_ROW; ++c) {
                if (valid_mid(a, b, c)) {
                    trans_c[cur++] = c;
                    ++cnt;
                }
            }
            trans_len[idx] = cnt;
        }
    }
    trans_built = 1;
}

/*====================================================================*/
/* Main sequence function.  Uses dynamic programming over rows. */
seq_t a(idx_t n)
{
    if (n == 0) return 0;

    build_transitions();

    /* Allocate DP buffers once. */
    static seq_t *dp_cur = NULL;
    static seq_t *dp_nxt = NULL;
    if (!dp_cur) {
        dp_cur = (seq_t*)calloc(STATES, sizeof(seq_t));
        dp_nxt = (seq_t*)calloc(STATES, sizeof(seq_t));
    }

    /* Initialise with first row (a = 0). */
    memset(dp_cur, 0, sizeof(seq_t) * STATES);
    for (int r0 = 0; r0 < MASKS_PER_ROW; ++r0)
        dp_cur[r0] = (seq_t)1;   /* state (0, r0) */

    /* Process remaining rows. */
    idx_t steps = n - 1;   /* number of transitions needed */
    for (idx_t k = 0; k < steps; ++k) {
        memset(dp_nxt, 0, sizeof(seq_t) * STATES);
        for (unsigned int s = 0; s < STATES; ++s) {
            seq_t ways = dp_cur[s];
            if (!ways) continue;
            uint8_t b = s & 0x7F;
            unsigned int start = trans_start[s];
            unsigned short len   = trans_len[s];
            for (unsigned short t = 0; t < len; ++t) {
                uint8_t c = trans_c[start + t];
                unsigned int ns = (b << 7) | c;
                dp_nxt[ns] += ways;
            }
        }
        seq_t *tmp = dp_cur;
        dp_cur   = dp_nxt;
        dp_nxt   = tmp;
    }

    /* Final check: last row is b, ensure its 1s are valid with
       previous row a and no below row.  (a,b,0). */
    seq_t result = 0;
    for (unsigned int s = 0; s < STATES; ++s) {
        seq_t ways = dp_cur[s];
        if (!ways) continue;
        uint8_t a = s >> 7;
        uint8_t b = s & 0x7F;
        if (valid_mid(a, b, 0))
            result += ways;
    }
    return result;
}
