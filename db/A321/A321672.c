/*
 * A321672  Number of chiral pairs of rows of length 5 using up to n colors.
 * https://oeis.org/A321672
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (n^5 - n^3)/2
 * Range:     u8 n <= 3, u16 n <= 10, u32 n <= 30, u64 n <= 30, u128 n <= 30,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* use 128‑bit arithmetic for the intermediate to avoid
       overflow on 64‑bit seq_t.  The final result is truncated
       to seq_t only when it is guaranteed to fit. */
#if defined(__SIZEOF_INT128__) || defined(__GNUC__) || defined(__clang__)
    typedef unsigned __int128 uint128_t;
    uint128_t n128 = (uint128_t)n;
    uint128_t n2   = n128 * n128;
    uint128_t n3   = n2 * n128;
    uint128_t n5   = n3 * n2;           /* n^5 */
    uint128_t tmp  = n5 - n3;           /* n^5 - n^3 */
    tmp /= 2;                          /* divide by 2 */
    return (seq_t)tmp;
#else
    /* Fallback for platforms lacking __int128: compute on
       seq_t directly as long as the value fits.  This
       branch is used only when the compiler does not provide
       an unsigned __int128 type. */
    seq_t N  = (seq_t)n;
    seq_t n2 = N * N;      /* n^2 */
    seq_t n3 = n2 * N;     /* n^3 */
    seq_t n5 = n3 * n2;    /* n^5 (potential overflow if seq_t is 64‑bit) */
    seq_t tmp = n5 - n3;   /* n^5 - n^3 */
    return tmp / 2;
#endif
}
