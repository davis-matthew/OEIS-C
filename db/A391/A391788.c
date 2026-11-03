/*
 * A391788  Array read by antidiagonals: A(n,k) = phi(n+k) with k >= 0 and
 *          A(0,0) = 0.
 * https://oeis.org/A391788
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, tabl
 * Formula:   a(n) = φ( ⌊(√(8·n + 1) − 1)/2⌋ )
 * Range:     u8 n <= 90, u16 n <= 90, u32 n <= 90, u64 n <= 90, u128 n <= 90,
 *            big any n
 * Validated: partial, pass, 91 terms through n = 90 (2026-11-01)
 * Helpers:   totient
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Uses the totient routine from utils/phi.h */

#include <stdint.h>
#include "../utils/totient.h"

/* find m such that m·(m+1)/2 ≤ n < (m+1)·(m+2)/2  */
static idx_t diag_for_idx(idx_t n)
{
    /* special case */
    if (n == 0) return 0;

    idx_t low  = 0;
    idx_t high = 1;

    /* grow high until high·(high+1)/2 > n   */
    while (1)
    {
        unsigned __int128 val =
            (unsigned __int128)high * ((unsigned __int128)high + 1) / 2;
        if (val > n) break;

        /* avoid overflow of the next shift */
        if (high > n / 2)
        {
            high = n + 1;   /* guarantees > n for the following test */
            break;
        }
        high <<= 1;
    }

    /* binary search on [low, high) where low satisfies the inequality   */
    while (low + 1 < high)
    {
        idx_t mid = low + ((high - low) >> 1);
        unsigned __int128 val_mid =
            (unsigned __int128)mid * ((unsigned __int128)mid + 1) / 2;
        if (val_mid <= n)
            low = mid;
        else
            high = mid;
    }
    return low;
}

/* external function required by the build system */
seq_t a(idx_t n)
{
    idx_t m = diag_for_idx(n);
    if (m == 0) return 0;   /* φ(0) is defined as 0 in the OEIS table  */
    return totient(m);
}
