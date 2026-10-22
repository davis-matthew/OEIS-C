/*
 * A364829  a(n) is the number of regions inside a regular n-gon after
 *          inscribing (always starting from the same point), for each proper
 *          divisor d_i of n greater than 1, a regular d_i-gon.
 * https://oeis.org/A364829
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 59, u16 n <= 70, u32 n <= 70, u64 n <= 70, u128 n <= 70,
 *            big any n
 * Helpers:   intersection_diameter_polygon, intersection_polygon_polygon
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A364829
    Formula: a(n) = 1 + (sum of edges of all inscribed polygons) +
                     (total number of interior intersections of all chords)
    a(1)=0, a(2)=0  */
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/intersection_diameter_polygon.h"
#include "../utils/intersection_polygon_polygon.h"

static int cmp_uint64(const void *p1, const void *p2)
{
    uint64_t v1 = *(const uint64_t *)p1;
    uint64_t v2 = *(const uint64_t *)p2;
    if (v1 < v2) return -1;
    if (v1 > v2) return  1;
    return 0;
}

seq_t a(idx_t n)
{
    if (n <= 2)
        return 0;            /* a(1)=0, a(2)=0 */

    /* gather proper divisors greater than 1 */
    idx_t *divs = (idx_t *)malloc((size_t)n * sizeof(idx_t));
    size_t div_cnt = 0;
    for (idx_t i = 2; i * i <= n; ++i) {
        if (n % i == 0) {
            if (i < n) {
                divs[div_cnt++] = i;
            }
            idx_t j = n / i;
            if (j != i && j < n) {
                divs[div_cnt++] = j;
            }
        }
    }
    /* sort divisors */
    qsort(divs, div_cnt, sizeof(idx_t), cmp_uint64);

    /* sum of edges */
    seq_t sum_edges = 0;
    for (size_t i = 0; i < div_cnt; ++i) {
        if (divs[i] == 2)
            sum_edges += 1;
        else
            sum_edges += (seq_t)divs[i];
    }

    /* total intersections */
    seq_t total_intersections = 0;
    for (size_t i = 0; i < div_cnt; ++i) {
        for (size_t j = i + 1; j < div_cnt; ++j) {
            idx_t d1 = divs[i];
            idx_t d2 = divs[j];
            /* handle the case where one polygon is the 2-gon (diameter) */
            if (d1 == 2 && d2 == 2) {
                /* impossible, only one divisor 2 */
                continue;
            } else if (d1 == 2) {
                total_intersections += intersection_diameter_polygon(n, d2);
            } else if (d2 == 2) {
                total_intersections += intersection_diameter_polygon(n, d1);
            } else {
                total_intersections += intersection_polygon_polygon(n, d1, d2);
            }
        }
    }

    free(divs);
    return 1 + sum_edges + total_intersections;
}
