/*
 * A338011  Odd composite integers m such that A004187(m)^2 == 1 (mod m).
 * https://oeis.org/A338011
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   This function returns the nth term of OEIS A338011 – odd
 *            composite integers m such that U_m(7,1)^2 ≡ 1 (mod m)
 * Range:     u8 n <= 2, u16 n <= 43, u32 n <= 43, u64 n <= 43, u128 n <= 43,
 *            big any n
 * Validated: partial, pass, 43 terms through n = 43 (2026-11-03)
 * Helpers:   is_prime_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>
#include "../utils/is_prime_u64.h"

static seq_t* seq_realloc(seq_t* old, size_t old_cap, size_t new_cap) {
#ifdef __cplusplus
    seq_t* new_ptr = new seq_t[new_cap];
    for (size_t i = 0; i < old_cap; ++i)
        new_ptr[i] = old[i];
    delete[] old;
    return new_ptr;
#else
    return (seq_t*)realloc(old, new_cap * sizeof(seq_t));
#endif
}

/* Miller–Rabin primality test for seq_t (supports 8/16/32/64/128/ arbitrary-precision) */
/* n fits in uint64_t (n >= 0 here) */
static bool is_prime_big_fits_u64(seq_t v)
{
    if (sizeof(seq_t) <= 8) return true;
    return !((seq_t)UINT64_MAX < v);
}

static bool is_prime_big(seq_t n) {
    if (n < 2) return false;
    /* exact in 64 bits when n fits: the seq_t products below overflow at
       u64/i64 once n passes 2^32, and are slow under big */
    if (is_prime_big_fits_u64(n)) return is_prime_u64((uint64_t)n);
    if (n == 2 || n == 3) return true;
    if ((n & 1) == 0) return false;

    /* write n-1 as d * 2^s with d odd */
    seq_t d = n - 1;
    unsigned s = 0;
    while ((d & 1) == 0) {
        d >>= 1;
        s++;
    }

    /* deterministic bases covering 128‑bit values */
    static const int base_list[] = {2,3,5,7,11,13,17,19,23,29,31,37};
    const int num_bases = sizeof(base_list)/sizeof(base_list[0]);

    for (int i = 0; i < num_bases; i++) {
        seq_t a = (seq_t)base_list[i];
        if (a >= n) continue;

        /* compute a^d mod n */
        seq_t x = 1;
        seq_t pow = a % n;
        seq_t exp = d;
        while (exp > 0) {
            if (exp & 1) x = (x * pow) % n;
            pow = (pow * pow) % n;
            exp >>= 1;
        }

        if (x == 1 || x == n-1) continue;

        bool cont = false;
        for (unsigned r = 1; r < s; r++) {
            x = (x * x) % n;
            if (x == n-1) { cont = true; break; }
        }
        if (!cont) return false;
    }
    return true;
}

/* Generic modular multiplication for seq_t.
   In C++ the operators are overloaded; in C we use 128‑bit. */
#ifdef __cplusplus
static inline seq_t mulmod(const seq_t a, const seq_t b, const seq_t mod) {
    return (a * b) % mod;
}
#else
static inline seq_t mulmod(const seq_t a, const seq_t b, const seq_t mod) {
    return (unsigned __int128)a * b % mod;
}
#endif

/* 2×2 matrix over seq_t. */
typedef struct {
    seq_t a[2][2];
} mat2;

/* Matrix multiplication modulo mod. */
static inline mat2 matmul(const mat2 *x, const mat2 *y, const seq_t mod) {
    mat2 r;
    r.a[0][0] = mulmod(x->a[0][0], y->a[0][0], mod) + mulmod(x->a[0][1], y->a[1][0], mod);
    if (r.a[0][0] >= mod) r.a[0][0] -= mod;
    r.a[0][1] = mulmod(x->a[0][0], y->a[0][1], mod) + mulmod(x->a[0][1], y->a[1][1], mod);
    if (r.a[0][1] >= mod) r.a[0][1] -= mod;
    r.a[1][0] = mulmod(x->a[1][0], y->a[0][0], mod) + mulmod(x->a[1][1], y->a[1][0], mod);
    if (r.a[1][0] >= mod) r.a[1][0] -= mod;
    r.a[1][1] = mulmod(x->a[1][0], y->a[0][1], mod) + mulmod(x->a[1][1], y->a[1][1], mod);
    if (r.a[1][1] >= mod) r.a[1][1] -= mod;
    return r;
}

/* Binary exponentiation of a 2×2 matrix modulo mod. */
static mat2 matpow(mat2 base, idx_t exp, const seq_t mod) {
    mat2 result = { .a = { {1 % mod, 0}, {0, 1 % mod} } };
    while (exp > 0) {
        if (exp & 1)
            result = matmul(&result, &base, mod);
        base = matmul(&base, &base, mod);
        exp >>= 1;
    }
    return result;
}

/* Compute U_m(7,1) modulo m. */
static seq_t lucas_U_mod(const seq_t m) {
    /* Base matrix [[7, -1],[1, 0]] modulo m. */
    mat2 base = { .a = { {7 % m, (m - 1) % m}, {1 % m, 0} } };
    mat2 r = matpow(base, (idx_t)m, m);
    /* U_m ≡ r[1][0] (mod m). */
    return r.a[1][0] % m;
}

/* Static state for memoisation. */
static seq_t *cache = NULL;
static size_t cache_len = 0;
static idx_t next_candidate = 3;  /* next odd number to test */

/* Main sequence function. */
seq_t a(idx_t n)
{
    if (n == 0) return 0;  /* offset 1: a(0) is undefined */

    /* Grow cache until we have at least n terms. */
    while (cache_len < (size_t)n) {
        seq_t cand_seq = (seq_t)next_candidate;

        /* Skip primes and even numbers. */
        if (!is_prime_big(cand_seq)) {
            seq_t U = lucas_U_mod(cand_seq);
            seq_t U2 = mulmod(U, U, cand_seq);
            if (U2 == (seq_t)(1 % cand_seq)) {
                seq_t *new_cache = seq_realloc(cache, cache_len, cache_len + 1);
                if (!new_cache) return 0;  /* allocation failure */
                cache = new_cache;
                cache[cache_len++] = cand_seq;
            }
        }

        next_candidate += 2;  /* stay odd */
    }

    return cache[n - 1];
}
