/*
 * A275893  a(n) = A199134(n+1)-1.
 * https://oeis.org/A275893
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A199134(n+1) - 1, where A199134 lists the rows r whose
 *            greedy queen (A065188) sits in a column c < r. Computed by
 *            simulating the greedy queens placement row by row
 * Range:     u8 n <= 96, u16 n <= 19097, u32 n <= 19097, u64 n <= 19097,
 *            u128 n <= 19097, big any n
 * Validated: standard, pass, 84 terms through n = 9915 (2026-11-08)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>

#define GQ_MAXC 400000
#define GQ_OFF  GQ_MAXC
#define GQ_MAXD (2 * GQ_MAXC + GQ_MAXC + 2)

static uint8_t gq_col[GQ_MAXC + 2];
static uint8_t gq_d1[GQ_MAXD];
static uint8_t gq_d2[GQ_MAXD];
static uint32_t *gq_below = NULL;
static size_t gq_cap = 0, gq_len = 0;
static uint32_t gq_row = 0;
static uint32_t gq_low = 1;

seq_t a(idx_t n)
{
    size_t need = (size_t)n + 1;
    while (gq_len < need) {
        uint32_t r = ++gq_row;
        while (gq_col[gq_low]) gq_low++;
        uint32_t c = gq_low;
        for (;;) {
            if (c > GQ_MAXC) exit(EXIT_FAILURE);
            if (!gq_col[c]) {
                size_t i1 = (size_t)((int64_t)r - (int64_t)c + GQ_OFF);
                size_t i2 = (size_t)r + c;
                if (i1 < GQ_MAXD && i2 < GQ_MAXD && !gq_d1[i1] && !gq_d2[i2]) break;
            }
            c++;
        }
        gq_col[c] = 1;
        gq_d1[(size_t)((int64_t)r - (int64_t)c + GQ_OFF)] = 1;
        gq_d2[(size_t)r + c] = 1;
        if (c < r) {
            if (gq_len + 1 > gq_cap) {
                size_t nc = gq_cap ? gq_cap * 2 : 1024;
                uint32_t *t = (uint32_t *)realloc(gq_below, nc * sizeof(uint32_t));
                if (!t) exit(EXIT_FAILURE);
                gq_below = t;
                gq_cap = nc;
            }
            gq_below[gq_len++] = r;
        }
    }
    /* A199134(n+1) is the (n+1)-th entry, i.e. 0-based index n */
    return (seq_t)gq_below[(size_t)n] - 1;
}
