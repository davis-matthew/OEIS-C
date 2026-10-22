/*
 * A351041  Minimal number of steps for a Racetrack car (using Moore
 *          neighborhood) to go around a circle of radius n.
 * https://oeis.org/A351041
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, more
 * Formula:   not stated
 * Range:     u8 n <= 35, u16 n <= 35, u32 n <= 35, u64 n <= 35, u128 n <= 35,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = ceil(sqrt(4*n)) + ceil(sqrt(8*n)) +
 *                ((floor(sqrt(8*n+1))-1)/2 + 1)
 *
 * This implements the conjectured closed form derived from the OEIS
 * description of A351041.  It does not rely on any external utilities
 * and works with any integer type assigned to seq_t and idx_t.
 */
#include <stdint.h>

/* Integer square root: floor of sqrt for a 64‑bit unsigned integer. */
static inline uint32_t floor_sqrt_uint64(uint64_t x)
{
    uint32_t low = 0, high = 65536;          /* 2^16 > sqrt(2^32) */
    while (low < high) {
        uint32_t mid = low + (high - low) / 2;
        uint64_t sq  = (uint64_t)mid * mid;
        if (sq <= x)
            low = mid + 1;
        else
            high = mid;
    }
    return low - 1;   /* low is first > sqrt(x) */
}

/* Computes a(n) for n >= 1.  idx_t is uint64_t; seq_t is unsigned. */
seq_t a(idx_t n)
{
    /* 4*n and 8*n fit in 64 bits for all n up to 10³⁴; safe here. */
    uint64_t four_n = 4u * (uint64_t)n;
    uint64_t eight_n = 8u * (uint64_t)n;
    uint64_t eight_n_plus1 = eight_n + 1u;

    /* ceil(sqrt(4*n)) */
    uint32_t fs4 = floor_sqrt_uint64(four_n);
    uint32_t ceil4 = fs4;
    if ((uint64_t)fs4 * fs4 < four_n)
        ceil4++;

    /* ceil(sqrt(8*n)) */
    uint32_t fs8 = floor_sqrt_uint64(eight_n);
    uint32_t ceil8 = fs8;
    if ((uint64_t)fs8 * fs8 < eight_n)
        ceil8++;

    /* delta = (floor(sqrt(8*n+1)) - 1) / 2 + 1 */
    uint32_t s8p1 = floor_sqrt_uint64(eight_n_plus1);
    uint32_t delta = (s8p1 - 1u) / 2u + 1u;

    /* Sum in a type that can hold the result; cast to seq_t. */
    uint32_t sum = ceil4 + ceil8 + delta;
    return (seq_t)sum;
}
