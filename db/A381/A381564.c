/*
 * A381564  2-tone chromatic number of a path with n-2 vertices joined to two
 *          adjacent vertices.
 * https://oeis.org/A381564
 *
 * Offset:    4    (a(4) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 9, u16 n <= 60, u32 n <= 60, u64 n <= 60, u128 n <= 60,
 *            big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: A381564
 * Formula: a(n) = ceil((9 + sqrt(8*n - 15))/2) for n > 8.
 * Initial values (n = 4..8): 8, 9, 9, 9, 9.
 */

#include <stdint.h>
#include "../utils/floor_sqrt.h"  /* floor_sqrt(seq_t) is available */

seq_t a(idx_t n)
{
    /* Handle the small values explicitly (offset = 4) */
    if (n < 9) {
        switch (n) {
            case 4: return (seq_t)8;
            case 5: return (seq_t)9;
            case 6: return (seq_t)9;
            case 7: return (seq_t)9;
            case 8: return (seq_t)9;
            default: break;   /* This will never happen for n < 9 */
        }
    }

    /* Compute the formula for n > 8 */
    seq_t m = (seq_t)8 * (seq_t)n - (seq_t)15;          /* 8*n - 15 */
    seq_t s = floor_sqrt(m);                           /* floor(sqrt(m)) */

    seq_t prod = s * s;                                 /* s^2 */
    seq_t a_val = (seq_t)9 + s;                         /* 9 + floor(sqrt) */

    if (prod == m) {                                   /* perfect square case */
        return (a_val + (seq_t)1) / (seq_t)2;           /* ceil((9+sqrt)/2) */
    } else {                                            /* non‑perfect square */
        return (a_val / (seq_t)2) + (seq_t)1;           /* floor(a_val/2)+1 */
    }
}
