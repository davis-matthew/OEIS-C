/*
 * A338007  Odd composite integers m such that A001906(m)^2 == 1 (mod m).
 * https://oeis.org/A338007
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 45, u32 n <= 45, u64 n <= 45, u128 n <= 45,
 *            big any n
 * Validated: partial, pass, 45 terms through n = 45 (2026-11-03)
 * Helpers:   is_prime_u64, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation: scan odd composite integers and test the Lucas
   condition A001906(m)^2 ≡ 1 (mod m) for parameters a = 3, b = 1. */

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include "../utils/mem.h"
#include "../utils/is_prime_u64.h"

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

/* 2×2 matrix for the companion matrix of the Lucas sequence */
typedef struct {
    seq_t a, b, c, d;   /* [[a,b],[c,d]] */
} mat_t;

/* multiply two 2×2 matrices modulo m */
static mat_t mul_matrix(const mat_t *x, const mat_t *y, seq_t m)
{
    mat_t r;
    r.a = (x->a * y->a + x->b * y->c) % m;
    r.b = (x->a * y->b + x->b * y->d) % m;
    r.c = (x->c * y->a + x->d * y->c) % m;
    r.d = (x->c * y->b + x->d * y->d) % m;
    return r;
}

/* Lucas sequence U(n) modulo mod for a = 3, b = 1.
   Returns U(n) mod mod.  n fits in idx_t. */
static seq_t lucasU_mod(seq_t n, seq_t mod)
{
    if (n == (seq_t)0) return (seq_t)0 % mod;
    if (n == (seq_t)1) return (seq_t)1 % mod;

    mat_t base = { (seq_t)3 % mod, (mod - (seq_t)1) % mod,
                   (seq_t)1 % mod, (seq_t)0 };
    mat_t res  = { (seq_t)1 % mod, (seq_t)0,
                   (seq_t)0, (seq_t)1 % mod };

    idx_t e = (idx_t)n;
    while (e) {
        if (e & 1U) res = mul_matrix(&res, &base, mod);
        base = mul_matrix(&base, &base, mod);
        e >>= 1;
    }
    /* After exponentiation, res * [1;0] = [U(n+1); U(n)]ᵀ */
    return res.c % mod;
}

/* Cache to support arbitrary call order. */
static seq_t *cache     = NULL;
static idx_t  cache_sz  = 0;   /* number of terms stored */
static idx_t  cache_cap = 0;   /* allocated capacity */

/* Next odd candidate to test when extending the cache. */
static seq_t next_candidate = 9;   /* first odd composite > 2 */

/* Grow the cache array. */
static void enlarge_cache(idx_t needed)
{
    if (cache_cap >= needed) return;
    idx_t new_cap = cache_cap > 0 ? cache_cap * 2 : 16;
    while (new_cap < needed) new_cap *= 2;
    seq_t *tmp = mem_resize(seq_t, cache, new_cap);
    cache = tmp;      /* assume success; build system frees at exit */
    cache_cap = new_cap;
}

/* Return the nth term (1‑based) of A338007. */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;   /* undefined, but never asked */

    /* extend cache until it contains n terms */
    while (cache_sz < n) {
        seq_t m = next_candidate;

        /* skip primes and even numbers */
        if ((m & (seq_t)1) && !is_prime_big(m)) {
            seq_t u = lucasU_mod(m, m);
            if (((u * u) % m) == (seq_t)1) {
                enlarge_cache(cache_sz + 1);
                cache[cache_sz++] = m;
            }
        }
        next_candidate = m + (seq_t)2;
    }
    return cache[n - 1];
}
