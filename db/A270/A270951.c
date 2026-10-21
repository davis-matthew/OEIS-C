/*
 * A270951  Numbers k such that k | A000129(k-1).
 * https://oeis.org/A270951
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 26, u16 n <= 3311, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   grow_seq_array
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A270951: k such that k | A000129(k−1) (Pell numbers) */
#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/grow_seq_array.h"

/* Compute (a * b) mod mod using 128‑bit intermediate if available
   to avoid overflow. Used by pell_mod64. */
static inline uint64_t mul_mod64(uint64_t a, uint64_t b, uint64_t mod)
{
#ifdef __SIZEOF_INT128__
    return (uint64_t)(((__uint128_t)a * b) % mod);
#else
    /* fallback with repeated addition (slow for large mod but still
       correct for 64‑bit values). */
    uint64_t res = 0;
    a %= mod;
    while (b) {
        if (b & 1) {
            res += a;
            if (res >= mod) res -= mod;
        }
        a <<= 1;
        if (a >= mod) a -= mod;
        b >>= 1;
    }
    return res;
#endif
}

/* Compute the Pell number P(n) modulo mod.
   P(0)=0, P(1)=1, P(n)=2*P(n−1)+P(n−2)  (mod mod).  Use
   fast exponentiation of the transition matrix. */
static inline uint64_t pell_mod64(idx_t n, uint64_t mod)
{
    if (mod == 1) return 0;
    uint64_t r00 = 1, r01 = 0, r10 = 0, r11 = 1;  /* identity matrix */
    uint64_t m00 = 2 % mod, m01 = 1 % mod, m10 = 1 % mod, m11 = 0; /* transition matrix */

    idx_t e = n;
    while (e) {
        if (e & 1) {
            uint64_t t00 = (mul_mod64(r00, m00, mod) + mul_mod64(r01, m10, mod)) % mod;
            uint64_t t01 = (mul_mod64(r00, m01, mod) + mul_mod64(r01, m11, mod)) % mod;
            uint64_t t10 = (mul_mod64(r10, m00, mod) + mul_mod64(r11, m10, mod)) % mod;
            uint64_t t11 = (mul_mod64(r10, m01, mod) + mul_mod64(r11, m11, mod)) % mod;
            r00 = t00; r01 = t01; r10 = t10; r11 = t11;
        }
        uint64_t t00 = (mul_mod64(m00, m00, mod) + mul_mod64(m01, m10, mod)) % mod;
        uint64_t t01 = (mul_mod64(m00, m01, mod) + mul_mod64(m01, m11, mod)) % mod;
        uint64_t t10 = (mul_mod64(m10, m00, mod) + mul_mod64(m11, m10, mod)) % mod;
        uint64_t t11 = (mul_mod64(m10, m01, mod) + mul_mod64(m11, m11, mod)) % mod;
        m00 = t00; m01 = t01; m10 = t10; m11 = t11;
        e >>= 1;
    }
    /* The Pell number P(n) is the (0,1) entry of M^n. */
    return r01 % mod;
}

/* Static cache to store found terms */
static seq_t *cache = NULL;
static size_t cache_cap = 0;
static idx_t cache_len = 0;
static idx_t next_k = 1;  /* next integer to test */

/* Return the n‑th term of the sequence.  Sequence is 1‑indexed. */
seq_t a(idx_t n)
{
    /* Grow the cache until it contains at least n terms. */
    while (cache_len < n) {
        uint64_t mod = (uint64_t)next_k;
        /* P(next_k-1) modulo next_k is zero iff next_k divides the Pell number. */
        if (pell_mod64(next_k - 1, mod) == 0) {
            size_t new_len = cache_len + 1;
            if (new_len > cache_cap) {
                grow_seq_array(&cache, &cache_cap, new_len);
            }
            cache[cache_len++] = (seq_t)next_k;
        }
        ++next_k;
    }
    return cache[n - 1];
}
