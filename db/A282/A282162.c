/*
 * A282162  Difference sequence of the upper Wythoff sequence, A001950, with 2
 *          prepended.
 * https://oeis.org/A282162
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 1 + ⌊(n+1)·φ⌋ – ⌊n·φ⌋ (φ = (1+√5)/2)
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Integer square root of a 128‑bit unsigned integer:
   returns floor(√x) */
static uint64_t isqrt_u128(__uint128_t x)
{
    uint64_t r = 0;
    for (int i = 127; i >= 0; --i) {
        uint64_t b = r | ((uint64_t)1ULL << i);
        if ((unsigned __int128)b * b <= x)
            r = b;
    }
    return r;
}

/* floor(k·φ)   (k ≥ 0) */
static seq_t floor_phi(idx_t k)
{
    /* floor(k·φ) = floor((k + floor(k·√5))/2) */
    __uint128_t prod = (unsigned __int128)5 * k * k;   /* k²·5 ≤ 2¹²⁸‑1 for k ≤ 10⁴ */
    uint64_t sqrtk = isqrt_u128(prod);                /* floor(k·√5) */
    return ((seq_t)k + (seq_t)sqrtk) / 2;
}

/* OEIS sequence A282162 */
seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)2;                 /* explicit prepend */
    seq_t pn  = floor_phi(n);
    seq_t pn1 = floor_phi(n + 1);
    return (seq_t)1 + pn1 - pn;
}
