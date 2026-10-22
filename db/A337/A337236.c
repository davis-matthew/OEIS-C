/*
 * A337236  Composite integers m such that A001076(m)^2 == 1 (mod m).
 * https://oeis.org/A337236
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 8, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   is_prime, mul64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Searches for composite m such that A001076(m)^2 ≡ 1 (mod m) */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>

#include "../utils/is_prime.h"
#include "../utils/mul64.h"

/* Compute A001076(n) modulo mod.
   A001076 is the Lucas sequence with a = 4, b = -1:
     U(0) = 0, U(1) = 1, U(n+2) = 4*U(n+1) + U(n)  (mod)
   For n >= 1 this returns U(n) mod mod.  */
static uint64_t A001076_mod(uint64_t n, uint64_t mod)
{
    if (n == 0) return 0 % mod;
    if (n == 1) return 1 % mod;

    /* 2x2 matrix exponentiation:  M = [[4,1],[1,0]]  */
    uint64_t a00 = 1, a01 = 0, a10 = 0, a11 = 1;            /* R = I */
    uint64_t b00 = 4 % mod, b01 = 1 % mod, b10 = 1 % mod, b11 = 0; /* B = M */
    uint64_t exp = n - 1;

    while (exp)
    {
        if (exp & 1)
        {
            uint64_t c00 = (mul64(a00, b00, mod) + mul64(a01, b10, mod)) % mod;
            uint64_t c01 = (mul64(a00, b01, mod) + mul64(a01, b11, mod)) % mod;
            uint64_t c10 = (mul64(a10, b00, mod) + mul64(a11, b10, mod)) % mod;
            uint64_t c11 = (mul64(a10, b01, mod) + mul64(a11, b11, mod)) % mod;
            a00 = c00; a01 = c01; a10 = c10; a11 = c11;
        }

        /* Square the base matrix */
        uint64_t c00 = (mul64(b00, b00, mod) + mul64(b01, b10, mod)) % mod;
        uint64_t c01 = (mul64(b00, b01, mod) + mul64(b01, b11, mod)) % mod;
        uint64_t c10 = (mul64(b10, b00, mod) + mul64(b11, b10, mod)) % mod;
        uint64_t c11 = (mul64(b10, b01, mod) + mul64(b11, b11, mod)) % mod;
        b00 = c00; b01 = c01; b10 = c10; b11 = c11;

        exp >>= 1;
    }

    /* U(n) = R[0][0] * U(1) + R[0][1] * U(0) = R[0][0] */
    return a00 % mod;
}

seq_t a(idx_t n)
{
    if (n == 0u)
        return (seq_t)0;   /* undefined for n=0 */

    /* Static cache: store the composite terms found so far */
    static seq_t *cache = NULL;
    static size_t cache_len = 0;
    static uint64_t next_candidate = 4ULL;   /* start checking from 4 */

    /* Grow the cache until it contains the requested index */
    while (cache_len < (size_t)n)
    {
        uint64_t m = next_candidate++;

        /* Skip primes; we only want composites */
        if (is_prime(m))
            continue;

        /* Compute A001076(m) modulo m and test the congruence */
        uint64_t val = A001076_mod(m, m);
        if (mul64(val, val, m) == 1)
        {
            /* Append to cache */
            seq_t *tmp = (seq_t *)realloc(cache, (cache_len + 1) * sizeof(seq_t));
            if (!tmp)
                abort();   /* allocation failure */
            cache = tmp;
            cache[cache_len++] = (seq_t)m;
        }
    }

    return cache[(size_t)n - 1];
}
