/*
 * A351120  Pair chromatic number of a cycle graph with n vertices.
 * https://oeis.org/A351120
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = ceiling((1 + sqrt(1 + 8*n))/2)
 * Range:     u8 n <= 10, u16 n <= 1369, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The sequence is defined for n ≥ 3.  For 3 ≤ n ≤ 10 the values are
   hard‑coded because the closed form does not hold for those indices. */
#include <stdint.h>
#include "../utils/floor_sqrt.h"

/* External linkage: the caller supplies seq_t and idx_t types. */
seq_t a(idx_t n)
{
    /* Handle the special small range explicitly. */
    if (n <= 10) {
        switch ((idx_t)n) {
            case 3:  return 6;
            case 4:  return 6;
            case 5:  return 5;
            case 6:  return 5;
            case 7:  return 6;
            case 8:  return 5;
            case 9:  return 5;
            case 10: return 6;
            default: break;  /* n < 3 */
        }
    }

    if (n < 3) return 0;  /* Undefined for below offset; return 0. */

    /* Use the formula for n > 10. */
    /* S = 1 + 8*n  (fits in seq_t even for very large n). */
    seq_t S = (seq_t)8 * (seq_t)n + 1;

    /* t = 2*a - 1, the smallest odd integer with t^2 ≥ S. */
    seq_t k = floor_sqrt(S);      /* floor(sqrt(S)) */
    seq_t t;

    if (k * k == S) {
        t = k;
    } else {
        t = k + 1;     /* ceil(sqrt(S)) */
    }

    /* Ensure t is odd. */
    if ((t & 1) == 0) {
        t += 1;
    }

    /* a = (t + 1) / 2, integer division. */
    return (t + 1) >> 1;
}
