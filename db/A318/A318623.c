/*
 * A318623  a(n) = 2^phi(n) mod n.
 * https://oeis.org/A318623
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2^phi(n) mod n
 * Range:     u8 n <= 271, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Multiply two 64‑bit numbers modulo a 64‑bit modulus using 128‑bit intermediate */
static inline uint64_t mul_mod64(uint64_t a, uint64_t b, uint64_t mod)
{
    return (uint64_t)(((__int128)a * b) % mod);
}

seq_t a(idx_t n)
{
    /* Power of two (including 1) */
    if ((n & (n - 1)) == 0)
        return (seq_t)0;

    /* Odd numbers larger than 1 */
    if ((n & 1ULL) != 0)
        return (seq_t)1;

    /* n is even but not a power of two */
    idx_t t   = __builtin_ctzll((uint64_t)n);   /* exponent of 2 in n */
    idx_t s   = n >> t;                         /* odd part, s > 1 */
    uint64_t pow2t = 1ULL << t;                 /* 2^t */

    /* Inverse of 2^t modulo s:
       inv(2^t) = ((s+1)/2)^t (mod s), because (s+1)/2 is inverse of 2 mod s. */
    uint64_t inv = 1;
    uint64_t base = (s + 1) / 2;
    uint64_t exp  = t;
    while (exp)
    {
        if (exp & 1)
            inv = mul_mod64(inv, base, (uint64_t)s);
        base = mul_mod64(base, base, (uint64_t)s);
        exp >>= 1;
    }

    /* Result fits in 64 bits: pow2t * inv < n < 2^64 */
    return (seq_t)(pow2t * inv);
}
