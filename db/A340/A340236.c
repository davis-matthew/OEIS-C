/*
 * A340236  Odd composite integers m such that A006190(3*m-J(m,13)) == 3 (mod
 *          m), where J(m,13) is the Jacobi symbol.
 * https://oeis.org/A340236
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 36, u32 n <= 36, u64 n <= 36, u128 n <= 36,
 *            big any n
 * Helpers:   is_prime, jacobi_symbol, mod_mul
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A340236: odd composite integers m such that
   A006190(3*m - J(m,13)) ≡ 3 (mod m), where J(m,13) is the Jacobi symbol. */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include "../utils/jacobi_symbol.h"
#include "../utils/mod_mul.h"
#include "../utils/is_prime.h"

/* fast Lucas U(n) modulo m for parameters a=3, b=-1 (matrix [[3,1],[1,0]]) */
static seq_t lucas_u_mod(uint64_t m, idx_t n)
{
    if (n == 0) return (seq_t)0 % (seq_t)m;

    uint64_t mod = m;          /* ensure unsigned arithmetic */
    uint64_t a = 3 % mod;      /* matrix element */
    uint64_t b = 1 % mod;      /* since -b = 1 */

    /* result matrix initially identity */
    uint64_t r00 = 1 % mod, r01 = 0, r10 = 0, r11 = 1 % mod;
    /* power matrix */
    uint64_t p00 = a,  p01 = b,  p10 = 1 % mod, p11 = 0;
    idx_t e = n - 1;

    while (e) {
        if (e & 1u) {
            uint64_t t00 = (mod_mul(r00, p00, mod) + mod_mul(r01, p10, mod)) % mod;
            uint64_t t01 = (mod_mul(r00, p01, mod) + mod_mul(r01, p11, mod)) % mod;
            uint64_t t10 = (mod_mul(r10, p00, mod) + mod_mul(r11, p10, mod)) % mod;
            uint64_t t11 = (mod_mul(r10, p01, mod) + mod_mul(r11, p11, mod)) % mod;
            r00 = t00; r01 = t01; r10 = t10; r11 = t11;
        }
        uint64_t t00 = (mod_mul(p00, p00, mod) + mod_mul(p01, p10, mod)) % mod;
        uint64_t t01 = (mod_mul(p00, p01, mod) + mod_mul(p01, p11, mod)) % mod;
        uint64_t t10 = (mod_mul(p10, p00, mod) + mod_mul(p11, p10, mod)) % mod;
        uint64_t t11 = (mod_mul(p10, p01, mod) + mod_mul(p11, p11, mod)) % mod;
        p00 = t00; p01 = t01; p10 = t10; p11 = t11;
        e >>= 1;
    }
    return (seq_t)(r00 % mod);
}

/* cache of found terms */
static seq_t *cache = NULL;
static size_t cache_len = 0, cache_cap = 0;
static uint64_t next_m = 9;   /* next odd number to test */

/* ensure cache has capacity for at least 'need' elements */
static void ensure_cache(size_t need)
{
    if (cache_cap == 0) {
        cache_cap = 16;
        cache = (seq_t *)malloc(sizeof(seq_t) * cache_cap);
    } else if (need > cache_cap) {
        size_t newcap = cache_cap;
        while (newcap < need) newcap <<= 1;
        cache = (seq_t *)realloc(cache, sizeof(seq_t) * newcap);
        cache_cap = newcap;
    }
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;        /* unused, offset is 1 */

    /* generate terms until we have at least n terms */
    while (cache_len < (size_t)n) {
        /* skip primes */
        while (is_prime(next_m)) next_m += 2;
        /* composite odd number found */
        int Jint = jacobi_symbol(next_m, 13);

        uint64_t k = 3U * next_m;
        if (Jint == -1) k += 1;
        else if (Jint == 1) k -= 1;
        /* otherwise Jint == 0: no change to k */

        seq_t u = lucas_u_mod(next_m, k);
        seq_t three = (seq_t)3 % (seq_t)next_m;

        if (u == three) {
            ensure_cache(cache_len + 1);
            cache[cache_len++] = (seq_t)next_m;
        }
        next_m += 2; /* next odd number */
    }
    return cache[n - 1];
}
