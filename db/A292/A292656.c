/*
 * A292656  Rank of n*Pi when all the numbers h, j*e, k*Pi, for h>=1, j>=1,
 *          k>=1, are jointly ranked.
 * https://oeis.org/A292656
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = ⌊n·π⌋ + ⌊n·π / e⌋ + n
 * Range:     u8 none, u16 none, u32 none, u64 n <= 1000, u128 n <= 1000,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The function uses a fixed‑point approximation of π and e with 10^18 scaling. */

#include <stdint.h>

/* 10^18 scaling factor (fits in 64 bits) */
static const seq_t SCALE      = 1000000000000000000ULL;        /* 10^18 */

/* floor(π  · 10^18) – 64‑bit truncation of π */
static const seq_t PI_SCALED  = 3141592653589793238ULL;        /* π ≈ 3.141592653589793238 */

/* floor(e  · 10^18) – 64‑bit truncation of e */
static const seq_t E_SCALED   = 2718281828459045235ULL;        /* e  ≈ 2.718281828459045235 */

seq_t a(idx_t n)
{
    if (n == 0) return 0;                     /* not part of the sequence */
    /* Compute ⌊n·π⌋  and  ⌊n·π/e⌋ using 128‑bit intermediate to avoid overflow. */
    seq_t floor_npi      = (seq_t)(((unsigned __int128)(seq_t)n * PI_SCALED) / SCALE);
    seq_t floor_npi_div_e =
        (seq_t)(((unsigned __int128)(seq_t)n * PI_SCALED) / E_SCALED);
    return floor_npi + floor_npi_div_e + (seq_t)n;
}
