/*
 * A277194  Number of integers k in range [n^2, ((n+1)^2)-1] for which 4 = the
 *          least number of squares that add up to k (A002828).
 * https://oeis.org/A277194
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 15, u16 n <= 494, u32 n <= 512, u64 n <= 512,
 *            u128 n <= 512, big any n
 * Validated: standard, pass, 108 terms through n = 495 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A277194: number of integers k in [n²,(n+1)²–1] that require 4 squares.
   Computed by pre‑computing squares and sums of two squares and using
   Legendre’s theorem to detect numbers that need 4 squares. */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

static inline int bitset_get(const uint8_t *bits, idx_t i)
{
    return (bits[(size_t)i >> 3] >> (i & 7)) & 1u;
}

/* Bit‑set helpers. */
static inline void bitset_set(uint8_t *bits, idx_t i)
{
    bits[(size_t)i >> 3] |= (uint8_t)(1u << (i & 7));
}

/* Legendre's theorem: true if n = 4^a*(8b+7). */
static inline bool is_legendre_form(seq_t n)
{
    while ((n & 3) == 0) {   /* divide by 4 while divisible */
        n >>= 2;
    }
    return (n & 7) == 7;
}

/* Integer square root for 64‑bit values. */
static inline idx_t floor_isqrt_u64(idx_t n)
{
    idx_t low = 0, high = n;
    while (low < high) {
        idx_t mid = (low + high + 1) >> 1;
        if (mid * mid <= n)
            low = mid;
        else
            high = mid - 1;
    }
    return low;
}

/* Cache for squares and sums of two squares. */
static uint8_t *square_bits = NULL;
static uint8_t *two_sq_bits = NULL;
static idx_t cache_limit   = 0;          /* maximum value covered */

/* Ensure precomputation up to (n+1)²‑1. */
static void ensure_cache(idx_t n)
{
    idx_t new_limit = (n + 1) * (n + 1) - 1;
    if (new_limit <= cache_limit) return; /* already covered */

    /* Allocate new bitsets. */
    if (square_bits) free(square_bits);
    if (two_sq_bits) free(two_sq_bits);
    size_t bytes = (new_limit >> 3) + 1;
    square_bits = (uint8_t*)calloc(bytes, 1);
    two_sq_bits = (uint8_t*)calloc(bytes, 1);
    if (!square_bits || !two_sq_bits) {
        /* Allocation failed – fall back. */
        if (square_bits) free(square_bits); square_bits = NULL;
        if (two_sq_bits) free(two_sq_bits); two_sq_bits = NULL;
        cache_limit = 0;
        return;
    }

    /* Mark perfect squares. */
    idx_t sqrt_lim = floor_isqrt_u64((idx_t)new_limit);
    for (idx_t i = 0; i <= sqrt_lim; ++i) {
        seq_t sq = (seq_t)i * i;
        bitset_set(square_bits, (idx_t)sq);
    }

    /* Mark sums of two squares. */
    for (idx_t a = 0; a <= sqrt_lim; ++a) {
        seq_t a2 = (seq_t)a * a;
        idx_t bmax = floor_isqrt_u64((idx_t)(new_limit - a2));
        for (idx_t b = 0; b <= bmax; ++b) {
            seq_t sum = a2 + (seq_t)b * b;
            bitset_set(two_sq_bits, (idx_t)sum);
        }
    }

    cache_limit = new_limit;
}

/* Sequence function. */
seq_t a(idx_t n)
{
    ensure_cache(n);
    if (!square_bits || !two_sq_bits) return (seq_t)0; /* allocation failed */

    seq_t count = (seq_t)0;
    idx_t low  = n * n;
    idx_t high = (n + 1) * (n + 1);   /* exclusive upper bound */

    for (idx_t k = low; k < high; ++k) {
        if (bitset_get(square_bits, k)) continue;   /* 1 square */
        if (bitset_get(two_sq_bits,   k)) continue; /* 2 squares */
        /* Not expressible with 1 or 2 squares. */
        if (is_legendre_form((seq_t)k)) {
            ++count;                               /* needs 4 squares */
        }
    }
    return count;
}
