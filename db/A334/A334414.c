/*
 * A334414  First differences of A334415.
 * https://oeis.org/A334414
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = round((n+1)/phi^2) - round(n/phi^2)
 * Range:     u8 n <= 88, u16 n <= 88, u32 n <= 88, u64 n <= 88, u128 n <= 88,
 *            big any n
 * Helpers:   floor_of_sqrt5_ratio, less_zero
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*          a(n) = round((n+1)/phi^2) - round(n/phi^2)
   where phi^2 = (3+sqrt(5))/2 */
#define _USE_MATH_DEFINES
#include <math.h>
#include <stdbool.h>
#include "../utils/floor_of_sqrt5_ratio.h"
#include "../utils/less_zero.h"

/* ------------------------------------------------------------------ */
/*  Main sequence function                                         */
/* ------------------------------------------------------------------ */
seq_t a(idx_t n)
{
    /* a(n) = round((n+1)/phi^2) - round(n/phi^2) = S(n+1)-S(n) */
    seq_t s_n  = floor_of_sqrt5_ratio(n);
    seq_t s_np = floor_of_sqrt5_ratio(n + 1);
    return (s_np > s_n) ? 1 : 0;
}
