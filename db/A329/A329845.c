/*
 * A329845  Beatty sequence for (3+sqrt(29))/5.
 * https://oeis.org/A329845
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = floor(n*(3+sqrt(29))/5) (Beatty sequence for
 *            (3+sqrt(29))/5)
 * Range:     u8 n <= 64, u16 n <= 64, u32 n <= 64, u64 n <= 64, u128 n <= 64,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <limits.h>

/* seq_t and idx_t are supplied by the build.  */
seq_t a(idx_t n)
{
    if (n == 0)  /* behaviour for offset 0 does not matter */
        return 0;

    /* a(1) = floor(r) = 1 */
    seq_t prev = 1;

    /* Compute successive terms up to n */
    for (idx_t i = 1; i < n; ++i) {
        /* Current index in the recurrence: i (so next index is i+1) */
        int64_t idx = (int64_t)i;     /* idx is i   */
        int64_t s   = (int64_t)(i + 1);/* s = i+1    */
        __int128 lhs = (__int128)s * s * 29; /* (i+1)^2 * 29 */

        /* Try candidates prev+1 and prev+2 respectively */
        seq_t next = prev + 2; /* default; will be overwritten if prev+1 works */
        for (seq_t cand = prev + 1; cand <= prev + 2; ++cand) {
            int64_t L = 5 * (int64_t)cand           - 3 * idx - 3;
            int64_t R = 5 * ((int64_t)cand + 1) - 3 * idx - 3;

            /* Check 5*cand <= (i+1)*(3+sqrt29)  < 5*(cand+1) */
            /* Left side: 5*cand - 3*(i+1) <= (i+1)*sqrt29 */
            if (L <= 0 || lhs >= (__int128)L * L) {
                /* Right side: (i+1)*sqrt29 < 5*(cand+1) - 3*(i+1) */
                if (R <= 0 || lhs < (__int128)R * R) {
                    next = cand;
                    break;
                }
            }
        }
        prev = next;
    }

    return prev;
}
