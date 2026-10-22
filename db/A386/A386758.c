/*
 * A386758  Number of decimal digits in the n-th Lucas number.
 * https://oeis.org/A386758
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  base, nonn, easy
 * Formula:   a(n) = 1 + floor(n*log10(phi))
 * Range:     u8 n <= 1220, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* log10(phi) ≈ 0.20898764024997873...      */

#include <stdint.h>

/* integers sufficient for n ≤ 2^64:  log10(phi) ≈ 208987640249978730 / 1e18 */
static const uint64_t LOG10PHI_SCALED = 208987640249978730ULL;  /* log10(phi) * 1e18 */
static const uint64_t LOG10PHI_SCALE  = 1000000000000000000ULL; /* 1e18 */

/* External linkage required */
seq_t a(idx_t n)
{
    /* use 128‑bit intermediate to prevent overflow */
    unsigned __int128 prod = (unsigned __int128)n * LOG10PHI_SCALED;
    seq_t digits = (seq_t)(prod / LOG10PHI_SCALE) + 1;
    return digits;
}
