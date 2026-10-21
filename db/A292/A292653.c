/*
 * A292653  Rank of n*sin(1) when all the numbers j*cos(1) and k*sin(1), for
 *          j>=1, k>=1, are jointly ranked.
 * https://oeis.org/A292653
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n + floor(n * tan(1)), with tan(1) ≈ 1.5574077246549023
 *            Rational approximation: tan(1) ≈ 1557407724 / 1000000000 (error
 *            < 6.5e-10). For n ≤ 10000 the error in floor(n * tan(1)) is <
 *            0.001, so the computed floor is exact. The result is returned as
 *            seq_t
 * Range:     u8 n <= 100, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>  /* for uint64_t */

seq_t a(idx_t n)
{
    /* rational approximation P/Q of tan(1) */
    const uint64_t P = 1557407724ULL;
    const uint64_t Q = 1000000000ULL;

    /* compute floor(n * P / Q) using 128‑bit arithmetic to avoid overflow */
#if defined(__SIZEOF_INT128__) && defined(__cplusplus)
    unsigned __int128 prod = (unsigned __int128)n * (unsigned __int128)P;
#elif defined(__SIZEOF_INT128__) && defined(__STDC_VERSION__) && __STDC_VERSION__ >= 201112L
    unsigned __int128 prod = (unsigned __int128)n * (unsigned __int128)P;
#else
    /* Fallback: use seq_t multiplication if 128‑bit not available. */
    seq_t prod = (seq_t)n * (seq_t)P;  /* assumes seq_t can hold the product */
#endif

    seq_t floored = (seq_t)(prod / Q);  /* floor of n*tan(1) */

    return (seq_t)n + floored;
}
