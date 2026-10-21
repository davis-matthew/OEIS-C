/*
 * A250427  Number of (n+1)X(3+1) 0..1 arrays with nondecreasing sum of every
 *          two consecutive values in every row and column.
 * https://oeis.org/A250427
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 7, u32 n <= 41, u64 n <= 210, u128 n <= 210,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A250427 – Number of (n+1)×4 0..1 arrays with
    nondecreasing sum of every two consecutive values
    in each row and column.

    The sequence satisfies empirical polynomial formulas:
    for even n  : a(n) = (n⁸ + 32n⁷ + 440n⁶ + 3392n⁵ + 16016n⁴
                          + 47360n³ + 85504n² + 86016n + 36864)/4096
    for odd  n  : a(n) = (n⁸ + 32n⁷ + 444n⁶ + 3488n⁵ + 16966n⁴
                          + 52320n³ + 99900n² + 108000n + 50625)/4096
*/

#include <stddef.h>  /* for size_t */

seq_t a(idx_t n)
{
    /* The function is valid for n ≥ 1; return 0 for n = 0. */
    if (n == 0) return 0;

    /* Use unsigned 128‑bit arithmetic to avoid overflow during
       intermediate calculations.  The final result is guaranteed
       to fit in seq_t for the tested range. */
    unsigned __int128 p[9];
    uint64_t n64 = (uint64_t)n;            /* guarantee 64‑bit source */
    p[0] = 1;
    for (size_t i = 1; i <= 8; ++i)
        p[i] = p[i - 1] * (unsigned __int128)n64;

    unsigned __int128 num = 0;

    if ((n & 1ULL) == 0) /* even n */
    {
        num  = p[8];
        num += (unsigned __int128)32  * p[7];
        num += (unsigned __int128)440  * p[6];
        num += (unsigned __int128)3392 * p[5];
        num += (unsigned __int128)16016* p[4];
        num += (unsigned __int128)47360* p[3];
        num += (unsigned __int128)85504* p[2];
        num += (unsigned __int128)86016* p[1];
        num += (unsigned __int128)36864;
    }
    else /* odd n */
    {
        num  = p[8];
        num += (unsigned __int128)32   * p[7];
        num += (unsigned __int128)444   * p[6];
        num += (unsigned __int128)3488  * p[5];
        num += (unsigned __int128)16966* p[4];
        num += (unsigned __int128)52320* p[3];
        num += (unsigned __int128)99900* p[2];
        num += (unsigned __int128)108000* p[1];
        num += (unsigned __int128)50625;
    }

    num /= 4096;                       /* divide by constant factor */

    return (seq_t)num;                 /* final value fits in seq_t */
}
