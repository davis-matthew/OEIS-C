/*
 * A327724  Product of A003059 and A071797.
 * https://oeis.org/A327724
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = ceil(sqrt(n)) * [ n - (ceil(sqrt(n)) - 1)^2 ]
 * Range:     u8 n <= 142, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Uses only integer arithmetic and standard C11 features. */

#include <stdint.h>

/* --- Integer floor square root for unsigned 64‑bit values --- */
static inline idx_t floor_isqrt_u64(idx_t n)
{
    if (n < 2)
        return n;                     /* sqrt(0)=0, sqrt(1)=1        */

    /* Initial guess: 1 << (floor(log2(n))/2) */
    idx_t x = 1ULL << (63 - __builtin_clzll(n)) >> 1;

    /* Newton iteration: use a few steps to converge quickly */
    for (int i = 0; i < 2; ++i)
        x = (x + n / x) >> 1;

    /* Correct the guess if it overshoots or is below the exact root */
    while (x > (uint64_t)(n / x))
        --x;
    while ((x + 1) <= (uint64_t)(n / (x + 1)))
        ++x;

    return x;
}

/* --- Main sequence function --- */
seq_t a(idx_t n)
{
    /* Ceil(sqrt(n))
       If n is a perfect square, ceil = floor; otherwise ceil = floor + 1. */
    idx_t k_floor = floor_isqrt_u64(n);
    idx_t k = (k_floor * k_floor == n) ? k_floor : k_floor + 1;

    /* (k-1)^2, safe because k-1 <= 2^32-1 for a 64‑bit n */
    idx_t base = (k - 1) * (k - 1);

    return (seq_t)k * ((seq_t)n - (seq_t)base);
}
