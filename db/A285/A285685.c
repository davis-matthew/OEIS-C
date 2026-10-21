/*
 * A285685  Characteristic sequence of the Beatty sequence, A022839, of
 *          sqrt(5).
 * https://oeis.org/A285685
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 1 - floor((n+1)*(1-1/r) - floor(n*(1-1/r))), where r =
 *            sqrt(5)
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/*  sqrt(5) ≈ 2236067977 / 1e9          (error < 5×10⁻¹⁰)   */
static const uint64_t SQRT5_NUM  = 2236067977ULL;
static const uint64_t SQRT5_DEN  = 1000000000ULL;

/* 1 / sqrt(5) ≈ 1e9 / 2236067977     (error < 5×10⁻¹⁰)   */
static const uint64_t INV_SQRT5_NUM = 1000000000ULL;
static const uint64_t INV_SQRT5_DEN = 2236067977ULL;

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* offset 1 – behavior below the first term is irrelevant */

    idx_t n_plus_one = n + 1;
    /* k = floor((n+1)/sqrt(5))  */
    unsigned __int128 k = (unsigned __int128)n_plus_one * INV_SQRT5_NUM / INV_SQRT5_DEN;

    /* t = floor(k * sqrt(5))   */
    unsigned __int128 t = (unsigned __int128)k * SQRT5_NUM / SQRT5_DEN;

    return (seq_t)(t == n ? 1 : 0);
}
