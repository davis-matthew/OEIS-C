/*
 * A242655  Numbers of the form 2x^2+2xy+3y^2 with x and y nonnegative.
 * https://oeis.org/A242655
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the n‑th smallest integer representable as 2x²+2xy+3y²
 *            (x,y ≥ 0)
 * Range:     u8 n <= 4, u16 n <= 71, u32 n <= 71, u64 n <= 71, u128 n <= 71,
 *            big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "../utils/floor_sqrt.h"

/* Return true iff val can be written as 2*x^2 + 2*x*y + 3*y^2 with x,y ≥ 0. */
static bool isRepresentable(seq_t val)
{
    for (idx_t y = 0; ; ++y) {
        seq_t yy   = (seq_t)y * (seq_t)y;
        seq_t limit = (seq_t)8 * val;          /* 8*val  */
        seq_t sub   = (seq_t)20 * yy;           /* 20*y²  */
        if (sub > limit) break;                /* D < 0   */

        seq_t D = limit - sub;                 /* discriminant (8*val - 20*y²) */
        seq_t s = floor_sqrt(D);               /* √D  */
        if (s * s != D) continue;              /* D not a square */

        seq_t two_y = (seq_t)2 * (seq_t)y;
        if (s < two_y) continue;               /* √D must be ≥ 2y   */
        seq_t diff = s - two_y;                /* √D - 2y  */
        if (diff % 4 != 0) continue;           /* (√D-2y) must be divisible by 4 */

        return true;                           /* x = (√D-2y)/4 ≥ 0   */
    }
    return false;
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;   /* safety for index 0 */

    static seq_t *data   = NULL;
    static idx_t  size   = 0;      /* number of terms stored   */
    static idx_t  cap    = 0;      /* allocated capacity       */
    static idx_t  checked = 0;     /* last integer examined    */

    while (size < n) {
        /* Find the next representable integer. */
        for ( ; ; ++checked) {
            if (isRepresentable((seq_t)checked)) {
                if (size == cap) {
                    size_t new_cap = cap ? (size_t)cap * 2 : 128;
                    seq_t *new_data = (seq_t*)malloc(new_cap * sizeof(seq_t));
                    if (!new_data) abort();      /* allocation failure */
                    if (data) {
                        memcpy(new_data, data, size * sizeof(seq_t));
                        free(data);
                    }
                    data = new_data;
                    cap  = (idx_t)new_cap;
                }
                data[size++] = (seq_t)checked;
                break;    /* next term found */
            }
        }
        /* Advance to the next integer after the found one. */
        ++checked;
    }
    return data[n - 1];
}
