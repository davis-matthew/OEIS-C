/*
 * A339725  Odd composite integers m such that A006497(3*m-J(m,13)) ==
 *          11*J(m,13) (mod m), where J(m,13) is the Jacobi symbol.
 * https://oeis.org/A339725
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 7, u16 n <= 46, u32 n <= 46, u64 n <= 46, u128 n <= 46,
 *            big any n
 * Helpers:   jacobi_mod13
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A339725: a(n) = nth odd composite m such that
   V(3*m - J(m,13)) ≡ 11*J(m,13) (mod m)
   where V is the Lucas sequence with a=3,b=-1 (V(0)=2,V(1)=3) and
   J(m,13) is the Jacobi symbol. */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/jacobi_mod13.h"

/* Helper for 64‑bit modular multiplication */
static inline uint64_t mulmod64(uint64_t a, uint64_t b, uint64_t mod)
{
#if defined(__SIZEOF_INT128__)
    return (uint64_t)((__uint128_t)a * b % mod);
#else
    /* Fallback: iterative doubling */
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

/* 64‑bit deterministic Miller–Rabin primality test (works for <2^64) */
static bool is_prime64(uint64_t n)
{
    if (n < 2) return false;
    static const uint64_t small_primes[] = {
        2,3,5,7,11,13,17,19,23,29,31,37,41,43,47
    };
    for (size_t i = 0; i < sizeof(small_primes)/sizeof(small_primes[0]); ++i) {
        if (n == small_primes[i]) return true;
        if (n % small_primes[i] == 0) return false;
    }

    /* write n-1 = d * 2^s */
    uint64_t d = n - 1;
    unsigned s = 0;
    while ((d & 1) == 0) {
        d >>= 1;
        ++s;
    }

    /* bases that guarantee correctness for 64‑bit */
    static const uint64_t bases[] = {2,325,9375,28178,450775,9780504,1795265022};
    for (size_t i = 0; i < sizeof(bases)/sizeof(bases[0]); ++i) {
        uint64_t a = bases[i] % n;
        if (a == 0) return true;
        uint64_t x = 1, power = d;
        uint64_t base = a % n;
        while (power) {
            if (power & 1) x = mulmod64(x, base, n);
            base = mulmod64(base, base, n);
            power >>= 1;
        }
        if (x == 1 || x == n - 1) continue;
        bool cont = false;
        for (unsigned r = 1; r < s; ++r) {
            x = mulmod64(x, x, n);
            if (x == n - 1) { cont = true; break; }
        }
        if (!cont) return false;
    }
    return true;
}

/* Multiply two seq_t modulo mod, reducing overflow for small seq_t */
static inline seq_t mul_mod_seq(seq_t a, seq_t b, seq_t mod)
{
    if (sizeof(seq_t) <= 8) {
        uint64_t A = (uint64_t)a;
        uint64_t B = (uint64_t)b;
        uint64_t M = (uint64_t)mod;
        uint64_t r = mulmod64(A, B, M);
        return (seq_t)r;
    } else {
        return (a * b) % mod;
    }
}

/* 2×2 matrix for Lucas V with a=3, b=-1 */
typedef struct {
    seq_t a00, a01, a10, a11;
} mat2_t;

/* matrix multiplication modulo mod */
static inline void mat_mul(const mat2_t *A, const mat2_t *B,
                           mat2_t *R, seq_t mod)
{
    R->a00 = (mul_mod_seq(A->a00, B->a00, mod) +
              mul_mod_seq(A->a01, B->a10, mod)) % mod;
    R->a01 = (mul_mod_seq(A->a00, B->a01, mod) +
              mul_mod_seq(A->a01, B->a11, mod)) % mod;
    R->a10 = (mul_mod_seq(A->a10, B->a00, mod) +
              mul_mod_seq(A->a11, B->a10, mod)) % mod;
    R->a11 = (mul_mod_seq(A->a10, B->a01, mod) +
              mul_mod_seq(A->a11, B->a11, mod)) % mod;
}

/* exponentiate matrix to power e modulo mod */
static void mat_pow(uint64_t e, seq_t mod, mat2_t *out)
{
    mat2_t res = { (seq_t)(1 % (uint64_t)mod), 0, 0, (seq_t)(1 % (uint64_t)mod) };
    mat2_t base = { (seq_t)(3 % (uint64_t)mod), (seq_t)(1 % (uint64_t)mod),
                    (seq_t)(1 % (uint64_t)mod), 0 };

    while (e) {
        if (e & 1) {
            mat2_t tmp;
            mat_mul(&res, &base, &tmp, mod);
            res = tmp;
        }
        e >>= 1;
        if (e) {
            mat2_t tmp;
            mat_mul(&base, &base, &tmp, mod);
            base = tmp;
        }
    }
    *out = res;
}

/* Compute V(n) modulo m.  n is natural, m>1. */
static inline seq_t lucasV_mod(uint64_t n, seq_t m)
{
    if (m == 1) return 0;
    if (n == 0) return 2 % m;
    if (n == 1) return 3 % m;

    mat2_t mat;
    mat_pow(n - 1, m, &mat);
    /* V = 3*a00 + 2*a01 mod m */
    seq_t v0 = (seq_t)3;
    seq_t v1 = (seq_t)2;
    seq_t val = (mul_mod_seq(mat.a00, v0, m) +
                 mul_mod_seq(mat.a01, v1, m)) % m;
    return val;
}

/* -------------------------------------------------------------- */
/*  Cache for the sequence values                                  */
static seq_t *cache = NULL;
static size_t cache_size = 0; /* number of stored terms */
static size_t cache_cap   = 0; /* allocated capacity        */
static uint64_t next_candidate = 9; /* first odd composite */

/* Grow the cache array as needed */
static void grow_cache(void)
{
    if (cache_cap == 0) {
        cache_cap = 16;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
    } else {
        cache_cap <<= 1;
        cache = (seq_t *)realloc(cache, cache_cap * sizeof(seq_t));
    }
}

/* -------------------------------------------------------------- */
/*  Main sequence function                                         */
seq_t a(idx_t n)
{
    if (n == 0) return 0;   /* offset is 1 */

    while (cache_size < n) {
        /* Test current candidate */
        if (!is_prime64(next_candidate)) {
            /* Jacobi symbol J(m,13) */
            int j = jacobi_mod13(next_candidate);

            /* w = 3*m - J */
            uint64_t w = 3 * next_candidate;
            if (j == 1)  w -= 1;
            else if (j == -1) w += 1;   /* J = -1 */

            /* Compute V(w) mod m */
            seq_t V = lucasV_mod(w, (seq_t)next_candidate);

            /* RHS = 11*J mod m */
            seq_t rhs;
            if (j == 0) {
                rhs = 0;
            } else if (j == 1) {
                rhs = (seq_t)11 % (seq_t)next_candidate;
            } else { /* j == -1 */
                seq_t mod = (seq_t)next_candidate;
                rhs = mod - ((seq_t)11 % mod);
                rhs %= mod;
            }

            /* Check congruence */
            if (V == rhs) {
                if (cache_size == cache_cap) grow_cache();
                cache[cache_size++] = (seq_t)next_candidate;
            }
        }
        next_candidate += 2ULL; /* move to next odd integer */
    }

    return cache[n - 1];   /* 1‑based indexing */
}
