/*
 * A245832  The Szeged index of the parallelogram-shaped benzenoid Q_k (see
 *          Fig. 5.7 of the Diudea et al. reference).
 * https://oeis.org/A245832
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (12*n^6 + 72*n^5 + 137*n^4 + 92*n^3 + 13*n^2 - 2*n)/6
 * Range:     u8 n <= 1, u16 n <= 4, u32 n <= 28, u64 n <= 28, u128 n <= 28,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* Use 128‑bit arithmetic to avoid overflow for n up to at least 10000 */
#if defined(__SIZEOF_INT128__)
    __int128 k = ( __int128 )n;
    __int128 num = 12 * k * k * k * k * k * k;   /* 12*n^6   */
    num += 72 * k * k * k * k * k;              /* 72*n^5   */
    num += 137 * k * k * k * k;                /* 137*n^4  */
    num += 92 * k * k * k;                    /* 92*n^3   */
    num += 13 * k * k;                        /* 13*n^2   */
    num -= 2 * k;                             /* -2*n     */
    return (seq_t)(num / 6);
#else
    /* Fallback for compilers without __int128.  Compute using 64-bit
       arithmetic while dividing by 6 as early as possible.  This
       should still be safe for all n that produce a result fitting
       into seq_t. */
    uint64_t k = (uint64_t)n;
    /* Compute stepwise, dividing by 2 and 3 separately to keep
       intermediate values small. */
    /* numerator = k * (12*k^5 + 72*k^4 + 137*k^3 + 92*k^2 + 13*k - 2) */
    uint64_t a1 = 12 * k * k * k * k * k;
    uint64_t a2 = 72 * k * k * k * k;
    uint64_t a3 = 137 * k * k * k;
    uint64_t a4 = 92 * k * k;
    uint64_t a5 = 13 * k;
    __uint128_t num = (__uint128_t)k * ( (__uint128_t)a1 + a2 + a3 + a4 + a5 );
    num -= 2 * k;
    num /= 6;
    return (seq_t)num;
#endif
}
