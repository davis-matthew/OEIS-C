/*
 * A341684  Expansion of the 2-adic integer Sum_{k>=0} k!.
 * https://oeis.org/A341684
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = (A341680(n+1) - A341680(n))/2^n Computes the n-th binary
 *            digit (0‑based) of the 2‑adic integer \sum_{k≥0} k!. The
 *            computation uses arbitrary‑precision binary arithmetic
 *            implemented with an array of 64‑bit limbs so that it works for
 *            any seq_t width (uint8_t … arbitrary precision)
 * Range:     u8 n <= 1000, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>

/* Helper: number of bits of k containing 1‑bits (popcount) */
static inline idx_t popcnt64(idx_t v) {
    return (idx_t)__builtin_popcountll((unsigned long long)v);
}

seq_t a(idx_t n) {
    /* Number of bits needed to represent 2^(n+1) */
    size_t bits_needed = (size_t)n + 1;
    /* Number of 64‑bit words required */
    size_t L = (bits_needed + 63) >> 6;

    uint64_t *fact = (uint64_t *)malloc(L * sizeof(uint64_t));
    uint64_t *sum  = (uint64_t *)malloc(L * sizeof(uint64_t));
    if (!fact || !sum) return (seq_t)0;

    for (size_t i = 0; i < L; ++i) fact[i] = sum[i] = 0;

    /* 0! = 1 */
    fact[0] = 1;
    sum[0] = 1;

    /* Accumulate factorials k! with 2‑adic valuation ≤ n */
    for (idx_t k = 1; ; ++k) {
        idx_t v2 = k - popcnt64(k);   /* v2(k!) = k - popcount(k) */
        if (v2 > n) break;

        /* Multiply fact by k modulo 2^(n+1) */
        __uint128_t carry = 0;
        for (size_t i = 0; i < L; ++i) {
            __uint128_t temp = (__uint128_t)fact[i] * k + carry;
            fact[i] = (uint64_t)temp;
            carry   = temp >> 64;
        }

        /* Add to running sum modulo 2^(n+1) */
        carry = 0;
        for (size_t i = 0; i < L; ++i) {
            __uint128_t temp = (__uint128_t)sum[i] + fact[i] + carry;
            sum[i] = (uint64_t)temp;
            carry = temp >> 64;
        }
    }

    size_t word  = n >> 6;
    unsigned shift = n & 63;
    uint64_t bit = (sum[word] >> shift) & 1ULL;

    free(fact);
    free(sum);

    return (seq_t)bit;
}
