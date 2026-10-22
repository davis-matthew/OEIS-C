/*
 * A340237  Odd composite integers m such that A052918(3*m-J(m,29)) == 5 (mod
 *          m), where J(m,29) is the Jacobi symbol.
 * https://oeis.org/A340237
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 10, u16 n <= 48, u32 n <= 48, u64 n <= 48, u128 n <= 48,
 *            big any n
 * Helpers:   jacobi_uint64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A340237: Odd composite integers m such that A052918(3*m‑J(m,29)) ≡ 5 (mod m),
   where A052918 is the Lucas U‑sequence with parameters a = 5, b = –1
   (recurrence: U(0)=0, U(1)=1, U(n+2)=5·U(n+1)+U(n)).  The function
   searches for the next qualifying m by testing odd integers in order;
   results are cached so that calls with any order of n are fast. */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/jacobi_uint64.h"

/* --------------------------------------------------------------------- */
/* Helper: multiply modulo 64‑bit modulus using unsigned __int128          */
static inline uint64_t mul_mod_uint64(uint64_t a, uint64_t b, uint64_t mod)
{
    return (uint64_t)((__uint128_t)a * b % mod);
}

/* --------------------------------------------------------------------- */

/* --------------------------------------------------------------------- */
/* Helper: simple primality test for 64‑bit unsigned values.              */
static bool is_prime_uint64(uint64_t n)
{
    if (n < 2u) return false;
    if (n % 2u == 0u) return n == 2u;
    if (n % 3u == 0u) return n == 3u;
    for (uint64_t i = 5u; i * i <= n; i += 6u) {
        if (n % i == 0u || n % (i + 2u) == 0u)
            return false;
    }
    return true;
}

/* --------------------------------------------------------------------- */
/* Compute the Lucas U‑sequence modulo m.  Recurrence matrix:
   M = [[5,1],[1,0]].
   M^n applied to [1,0]^T gives [U(n+1), U(n)]^T.                       */
static seq_t lucas_U_mod(uint64_t n, uint64_t mod)
{
    if (mod <= 1u) return 0;
    uint64_t r00 = 1 % mod, r01 = 0, r10 = 0, r11 = 1 % mod;   /* Identity */
    uint64_t b00 = 5 % mod, b01 = 1 % mod, b10 = 1 % mod, b11 = 0;/* Base matrix */
    uint64_t exp = n;
    while (exp > 0) {
        if (exp & 1u) {
            uint64_t t00 = (mul_mod_uint64(r00, b00, mod) + mul_mod_uint64(r01, b10, mod)) % mod;
            uint64_t t01 = (mul_mod_uint64(r00, b01, mod) + mul_mod_uint64(r01, b11, mod)) % mod;
            uint64_t t10 = (mul_mod_uint64(r10, b00, mod) + mul_mod_uint64(r11, b10, mod)) % mod;
            uint64_t t11 = (mul_mod_uint64(r10, b01, mod) + mul_mod_uint64(r11, b11, mod)) % mod;
            r00 = t00; r01 = t01; r10 = t10; r11 = t11;
        }
        uint64_t tb00 = (mul_mod_uint64(b00, b00, mod) + mul_mod_uint64(b01, b10, mod)) % mod;
        uint64_t tb01 = (mul_mod_uint64(b00, b01, mod) + mul_mod_uint64(b01, b11, mod)) % mod;
        uint64_t tb10 = (mul_mod_uint64(b10, b00, mod) + mul_mod_uint64(b11, b10, mod)) % mod;
        uint64_t tb11 = (mul_mod_uint64(b10, b01, mod) + mul_mod_uint64(b11, b11, mod)) % mod;
        b00 = tb00; b01 = tb01; b10 = tb10; b11 = tb11;
        exp >>= 1;
    }
    return (seq_t)(r10 % mod); /* U(n) is the (1,0) entry of M^n */
}

/* --------------------------------------------------------------------- */
/* Main sequence function */
seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static size_t cache_len = 0;
    static size_t cache_cap = 0;
    static uint64_t search_m = 9;          /* first odd composite */

    while (cache_len < (size_t)n) {
        uint64_t m = search_m;
        search_m += 2;                     /* next odd integer */

        /* Skip primes */
        if (is_prime_uint64(m))
            continue;

        int j = jacobi_uint64(m, 29);
        uint64_t k = (j > 0) ? 3ull * m - 1ull
                     : (j < 0) ? 3ull * m + 1ull
                     : 3ull * m;

        seq_t u = lucas_U_mod(k, m);

        if (u == (seq_t)5) {               /* condition satisfied */
            if (cache_len == cache_cap) {   /* grow cache */
                size_t new_cap = cache_cap ? cache_cap * 2 : 8;
                seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
                if (!tmp)
                    abort();                /* allocation failure */
                cache = tmp;
                cache_cap = new_cap;
            }
            cache[cache_len++] = (seq_t)m;
        }
    }

    return cache[(size_t)n - 1];
}
