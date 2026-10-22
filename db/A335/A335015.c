/*
 * A335015  Smallest side lengths of equable Heronian triangles (with
 *          multiplicity).
 * https://oeis.org/A335015
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full
 * Formula:   a(n) = smallest side length of an equable Heronian triangle
 *            (sorted in non‑decreasing order)
 * Range:     u8 n <= 5, u16 n <= 5, u32 n <= 5, u64 n <= 5, u128 n <= 5,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>

/* Comparator for qsort on seq_t values. */
static int cmp_seq(const void *p1, const void *p2)
{
    seq_t a = *(const seq_t *)p1;
    seq_t b = *(const seq_t *)p2;
    if (a < b) return -1;
    if (a > b) return 1;
    return 0;
}

seq_t a(idx_t n)
{
    /* Cache the precomputed values. */
    static seq_t *vals = NULL;
    static size_t sz = 0;

    if (!vals) {
        /* enumerate all positive integer triples (u,v,w) that satisfy
           u*v*w = 4*(u+v+w) and u >= v >= w >= 1.  For each triple
           compute the sides of the triangle:
               a = v + w
               b = w + u
               c = u + v
           store the smallest side.  Only five distinct triangles exist. */
        const idx_t max_w = 8;   /* derived from the inequality v*w-4 <= 4(v+w) */
        const idx_t max_v = 200; /* sufficiently large for all solutions */

        seq_t tmp[50];   /* temporary storage – there will be far fewer than 50 */
        size_t tcnt = 0;

        for (idx_t w = 1; w <= max_w; ++w) {
            for (idx_t v = w; v <= max_v; ++v) {
                idx_t denom = v * w - 4;
                if (denom <= 0) continue;
                idx_t num = 4 * (v + w);
                if (num % denom != 0) continue;
                idx_t u = num / denom;
                if (u < v) continue;   /* enforce ordering u >= v >= w */

                seq_t a_side = (seq_t)(v + w);
                seq_t b_side = (seq_t)(w + u);
                seq_t c_side = (seq_t)(u + v);
                seq_t min_side = a_side;
                if (b_side < min_side) min_side = b_side;
                if (c_side < min_side) min_side = c_side;

                if (tcnt < sizeof(tmp) / sizeof(tmp[0])) {
                    tmp[tcnt++] = min_side;
                }
            }
        }

        /* Sort the collected minima. */
        qsort(tmp, tcnt, sizeof(seq_t), cmp_seq);

        /* Allocate final result array. */
        vals = (seq_t *)malloc(tcnt * sizeof(seq_t));
        for (size_t i = 0; i < tcnt; ++i) vals[i] = tmp[i];
        sz = tcnt;
    }

    if (n == 0 || n > sz) return (seq_t)0;  /* undefined indices return 0 */
    return vals[n - 1];
}
