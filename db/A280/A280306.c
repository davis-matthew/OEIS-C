/*
 * A280306  a(n) = A049501(A003418(n)).
 * https://oeis.org/A280306
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = major_index( binary( lcm(1..n) ) ) * where major_index
 *            counts the sum of positions of each "10" pattern * in the binary
 *            expansion of the LCM of 1..n
 * Range:     u8 n <= 30, u16 n <= 508, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   build_primes_upto
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stddef.h>
#include "../utils/build_primes_upto.h"

/* Cache of primes up to the largest n requested. */
static uint64_t *prime_cache      = NULL;
static size_t    prime_cnt        = 0;
static idx_t     prime_limit      = 0;

/* Big integer represented as little‑endian 64‑bit limbs.  Fixed
   capacity is sufficient for n up to at least 10000 (≈14400 bits). */
#define BIGINT_CAP 1024
static inline void mul_small(uint64_t *limbs, size_t *len, uint64_t factor)
{
    __uint128_t carry = 0;
    for (size_t i = 0; i < *len; ++i) {
        __uint128_t prod = (__uint128_t)limbs[i] * factor + carry;
        limbs[i] = (uint64_t)prod;
        carry   = prod >> 64;
    }
    while (carry) {
        limbs[(*len)++] = (uint64_t)carry;
        carry >>= 64;
    }
}

/* Return bitlength of the big integer.  Returns 0 if zero. */
static inline size_t bitlen(const uint64_t *limbs, size_t len)
{
    if (len == 0) return 0;
    uint64_t w = limbs[len - 1];
    /* Find most significant set bit in w. */
    int msb = 63;
    while ((w >> msb) == 0) msb--;
    return (len - 1) * 64 + msb + 1;
}

/* Return the i‑th bit (0 = least significant). */
static inline int bit_at(const uint64_t *limbs, size_t i)
{
    return (int)((limbs[i >> 6] >> (i & 63)) & 1ULL);
}

seq_t a(idx_t n)
{
    if (n < 2) return 0;          /* lcm(1..0) = lcm(1..1) = 1 → no "10" */
    /* Update prime cache as necessary. */
    if (n > prime_limit) {
        if (prime_cache) free(prime_cache);
        build_primes_upto(n, &prime_cache, &prime_cnt);
        prime_limit = n;
    }

    /* Big integer for the LCM, little‑endian limbs. */
    uint64_t limbs[BIGINT_CAP];
    size_t len = 1;
    limbs[0] = 1ULL;

    /* Multiply by each prime power p^e <= n. */
    for (size_t i = 0; i < prime_cnt; ++i) {
        uint64_t p = prime_cache[i];
        uint64_t pow = 1;
        while (pow <= (uint64_t)(n / p)) pow *= p;  /* p^e <= n */
        if (pow > 1) mul_small(limbs, &len, pow);
    }

    size_t totbits = bitlen(limbs, len);
    if (totbits == 0) return 0;   /* shouldn't happen */

    seq_t sum = 0;
    /* Scan from most significant bit (position 1) to least (position totbits). */
    for (size_t pos = 1; pos < totbits; ++pos) {
        size_t hi_idx = totbits - pos;     /* index of high bit, from LSB side */
        size_t lo_idx = hi_idx - 1;        /* index of low bit */
        int high = bit_at(limbs, hi_idx);
        int low  = bit_at(limbs, lo_idx);
        if (high && !low) sum += (seq_t)pos;
    }
    return sum;
}
