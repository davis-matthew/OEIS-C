/*
 * A254014  Number of (n+2)X(2+2) 0..1 arrays with every 3X3 subblock diagonal
 *          sum plus antidiagonal median nondecreasing horizontally,
 *          vertically and ne-to-sw antidiagonally.
 * https://oeis.org/A254014
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) equals the number of admissible binary 4‑column arrays of
 *            height (n+2) that satisfy the nondecreasing
 *            diagonal‑plus‑anti‑diagonal‑median constraint. The computation
 *            uses dynamic programming over admissible triples of rows
 * Range:     u8 none, u16 n <= 3, u32 n <= 16, u64 n <= 45, u128 n <= 106,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

/*  2^(4*3) possible triples of 4‑bit rows  */
#define TOTAL_TRIPLES 4096
/*  maximum outgoing transitions per state  */
#define MAX_DEGREE   16

/* tables used during initialization  */
static int  idx_map[TOTAL_TRIPLES];          /* mapping triple -> admissible state index, -1 if invalid */
static uint8_t f0[TOTAL_TRIPLES];           /* value of diagonal+anti‑median for offset 0 */
static uint8_t f1[TOTAL_TRIPLES];           /* value for offset 1 */
static int  valid_triples[TOTAL_TRIPLES];   /* list of admissible triple encodings */
static int  valid_count = 0;                /* number of admissible triples */

/* adjacency lists for each admissible state */
static int  trans[TOTAL_TRIPLES][MAX_DEGREE];
static int  trans_deg[TOTAL_TRIPLES];

/* DP buffers – two static arrays that will be swapped by pointer */
static seq_t dp_buf1[TOTAL_TRIPLES];
static seq_t dp_buf2[TOTAL_TRIPLES];

/* helper: retrieve the j‑th bit (0‑least) of a 4‑bit value */
static inline uint8_t bit(uint8_t row, uint8_t pos)
{
    return (row >> pos) & 1U;
}

/* compute F(a,b,c,j) for a triple (a,b,c) and offset j (0 or 1) */
static inline uint8_t compute_f(uint8_t a, uint8_t b, uint8_t c, uint8_t j)
{
    uint8_t diag = bit(a, j) + bit(b, j + 1) + bit(c, j + 2);
    uint8_t ant0 = bit(a, j + 2);
    uint8_t ant1 = bit(b, j + 1);
    uint8_t ant2 = bit(c, j);
    uint8_t median = (ant0 + ant1 + ant2 >= 2) ? 1 : 0;
    return diag + median;              /* range 0 … 4 */
}

/* lazy initialization of all tables  */
static void init(void)
{
    static int initialized = 0;
    if (initialized) return;
    initialized = 1;

    /* initialise idx_map to -1  */
    for (int i = 0; i < TOTAL_TRIPLES; ++i) idx_map[i] = -1;

    /* find all admissible triples */
    for (int enc = 0; enc < TOTAL_TRIPLES; ++enc) {
        uint8_t a = (enc >> 8) & 0xF;
        uint8_t b = (enc >> 4) & 0xF;
        uint8_t c = enc & 0xF;
        f0[enc] = compute_f(a, b, c, 0);
        f1[enc] = compute_f(a, b, c, 1);
        if (f0[enc] <= f1[enc]) {            /* horizontal non‑decreasing */
            idx_map[enc] = valid_count;
            valid_triples[valid_count++] = enc;
        }
    }

    /* build transitions */
    for (int vi = 0; vi < valid_count; ++vi) {
        int enc0 = valid_triples[vi];
        uint8_t a0 = (enc0 >> 8) & 0xF;
        uint8_t b0 = (enc0 >> 4) & 0xF;
        uint8_t c0 = enc0 & 0xF;
        uint8_t f0_0 = f0[enc0];
        uint8_t f1_0 = f1[enc0];

        int deg = 0;
        for (uint8_t d = 0; d < 16; ++d) {
            int enc1 = (b0 << 8) | (c0 << 4) | d;
            int vi1 = idx_map[enc1];
            if (vi1 < 0) continue;          /* not admissible horizontally */

            uint8_t f0_1 = f0[enc1];
            uint8_t f1_1 = f1[enc1];

            /* vertical & anti‑diagonal non‑decreasing */
            if (f0_0 <= f0_1 && f1_0 <= f1_1 && f1_0 <= f0_1) {
                trans[vi][deg++] = vi1;
                if (deg == MAX_DEGREE) break;
            }
        }
        trans_deg[vi] = deg;
    }
}

/* --------------------------------------------------------------- */
/*  Main function required by the judge                           */
seq_t a(idx_t n)
{
    init();                        /* tables ready */

    /* DP pointers */
    seq_t *cur = dp_buf1;
    seq_t *next = dp_buf2;

    /* initialise current layer for n = 1 */
    for (int i = 0; i < valid_count; ++i) cur[i] = (seq_t)1;
    for (int i = valid_count; i < TOTAL_TRIPLES; ++i) cur[i] = (seq_t)0;

    /* perform (n-1) transitions */
    for (idx_t step = 1; step < n; ++step) {
        /* clear next DP layer */
        for (int i = 0; i < valid_count; ++i) next[i] = (seq_t)0;

        /* relax edges */
        for (int i = 0; i < valid_count; ++i) {
            seq_t val = cur[i];
            if (val == (seq_t)0) continue;
            int deg = trans_deg[i];
            for (int e = 0; e < deg; ++e) {
                int j = trans[i][e];
                next[j] += val;
            }
        }
        /* swap current and next */
        seq_t *tmp = cur; cur = next; next = tmp;
    }

    /* sum over all ending states */
    seq_t result = (seq_t)0;
    for (int i = 0; i < valid_count; ++i) result += cur[i];
    return result;
}
