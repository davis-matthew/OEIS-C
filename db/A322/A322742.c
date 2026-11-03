/*
 * A322742  Sorted list of 120 distinct triangle areas corresponding to the
 *          unique solution to the problem of placing 10 points on a grid
 *          rectangle of minimal area, such that all triangles formed by any 3
 *          of the points have distinct areas > 0.
 * https://oeis.org/A322742
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full
 * Formula:   a(n) = the nth smallest doubled area of the 120 triangles formed
 *            by the 10 fixed grid points listed in the OEIS entry
 * Range:     u8 n <= 7, u16 n <= 120, u32 n <= 120, u64 n <= 120,
 *            u128 n <= 120, big any n
 * Validated: full, pass, 120 terms through n = 120 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stddef.h>
#include "../utils/mem.h"

static int seq_cmp(const void *p, const void *q)
{
    const seq_t *a = (const seq_t *)p;
    const seq_t *b = (const seq_t *)q;
    if (*a < *b) return -1;
    if (*a > *b) return 1;
    return 0;
}

seq_t a(idx_t n)
{
    /* static state: sorted list of 120 doubled areas */
    static seq_t *areas = NULL;
    static idx_t n_terms = 0;

    if (!areas) { /* first call – compute all 120 areas */
        /* coordinates of the 10 grid points */
        const int pts[10][2] = {
            {0, 3}, {1, 9}, {2, 18}, {5, 0},
            {5,10}, {12,17}, {15,13}, {17,4},
            {18,0}, {19,5}
        };
        const idx_t total = 120;          /* C(10,3) */
        areas = mem_new(seq_t, total);
        if (!areas) return 0;            /* allocation failed */

        idx_t pos = 0;
        for (idx_t i = 0; i < 10; ++i) {
            for (idx_t j = i + 1; j < 10; ++j) {
                for (idx_t k = j + 1; k < 10; ++k) {
                    int x1 = pts[i][0], y1 = pts[i][1];
                    int x2 = pts[j][0], y2 = pts[j][1];
                    int x3 = pts[k][0], y3 = pts[k][1];
                    int d = x1 * (y2 - y3) + x2 * (y3 - y1) + x3 * (y1 - y2);
                    if (d < 0) d = -d;          /* absolute value */
                    areas[pos++] = (seq_t)d;     /* doubled area */
                }
            }
        }
        n_terms = pos;
        mem_qsort(areas, n_terms, sizeof(seq_t), seq_cmp);
    }

    if (n < 1 || n > n_terms) return 0;   /* out of range */
    return areas[n - 1];
}
