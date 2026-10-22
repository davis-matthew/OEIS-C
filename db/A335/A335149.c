/*
 * A335149  Perimeters of Heronian triangles whose smallest side length is
 *          coprime to their middle side and whose smallest side length
 *          divides their perimeter.
 * https://oeis.org/A335149
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   sequence A335149 computed by searching all Heronian triangles
 *            with smallest side coprime to the middle side and dividing the
 *            perimeter
 * Range:     u8 n <= 1, u16 n <= 8, u32 n <= 58, u64 n <= 58, u128 n <= 58,
 *            big any n
 * Helpers:   floor_sqrt, gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stddef.h>
#include "../utils/gcd.h"
#include "../utils/floor_sqrt.h"

/* Dynamic array helpers */
static void grow_seq_array(seq_t **arr, size_t *cap, size_t new_cap)
{
    seq_t *tmp = (*cap == 0) ? (seq_t *)malloc(new_cap * sizeof(seq_t))
                            : (seq_t *)realloc(*arr, new_cap * sizeof(seq_t));
    *arr = tmp;
    *cap = new_cap;
}

/* Stored perimeters */
static seq_t *perims      = NULL;
static size_t perims_len  = 0;
static size_t perims_cap  = 0;
/* Last processed perimeter */
static idx_t  max_p_seen  = 0;

/* Append a new perimeter */
static void add_perim(seq_t p)
{
    if (perims_len == perims_cap)
        grow_seq_array(&perims, &perims_cap, perims_cap * 2 + 10);
    perims[perims_len++] = p;
}

/* Main sequence function */
seq_t a(idx_t n)
{
    /* Generate perimeters until we have at least n terms */
    while (perims_len < n) {
        /* Advance to next perimeter; only even perimeters can be Heronian */
        max_p_seen++;
        if (max_p_seen % 2 == 1) continue;          /* skip odd perimeters */

        seq_t p = (seq_t)max_p_seen;
        seq_t s = p / 2;                            /* semiperimeter */

        bool found = false;
        idx_t limit = max_p_seen / 3;               /* a <= p/3 */

        for (idx_t d = 1; d * d <= max_p_seen && !found; ++d) {
            if (max_p_seen % d != 0) continue;

            /* Process divisor d as possible smallest side a */
            if (d <= limit) {
                seq_t a_side = (seq_t)d;
                seq_t b_min = ((s - a_side) + 1 > a_side) ? ((s - a_side) + 1) : a_side;
                seq_t b_max = (seq_t)((max_p_seen - d) / 2);
                seq_t b = b_min;
                while (b <= b_max && !found) {
                    if (gcd(a_side, b) != 1) { ++b; continue; }
                    seq_t c = p - a_side - b;
                    /* Heronian test: check if s*(s-a)*(s-b)*(s-c) is a perfect square */
                    seq_t val = s * (s - a_side) * (s - b) * (s - c);
                    seq_t r = floor_sqrt(val);
                    if (r * r == val) {
                        add_perim(p);
                        found = true;
                        break;
                    }
                    ++b;
                }
            }

            /* Process the paired divisor d2 = p/d */
            idx_t d2 = max_p_seen / d;
            if (d2 != d && d2 <= limit && !found) {
                seq_t a_side = (seq_t)d2;
                seq_t b_min = ((s - a_side) + 1 > a_side) ? ((s - a_side) + 1) : a_side;
                seq_t b_max = (seq_t)((max_p_seen - d2) / 2);
                seq_t b = b_min;
                while (b <= b_max && !found) {
                    if (gcd(a_side, b) != 1) { ++b; continue; }
                    seq_t c = p - a_side - b;
                    seq_t val = s * (s - a_side) * (s - b) * (s - c);
                    seq_t r = floor_sqrt(val);
                    if (r * r == val) {
                        add_perim(p);
                        found = true;
                        break;
                    }
                    ++b;
                }
            }
        }
        /* If no triangle satisfies the conditions for this perimeter, simply continue. */
    }

    return perims[n - 1];
}
