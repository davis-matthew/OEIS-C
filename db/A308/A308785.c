/*
 * A308785  Primes p such that A001175(p) = 2*(p+1)/7.
 * https://oeis.org/A308785
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 40, u32 n <= 40, u64 n <= 40, u128 n <= 40,
 *            big any n
 * Helpers:   is_prime_big, next_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A308785: primes p for which the Fibonacci period modulo p
   equals 2*(p+1)/7.  The n‑th term is returned by a(n).
   The algorithm generates primes p≡6 (mod 7) in increasing order,
   checks that M^k≡I (mod p) where k=2*(p+1)/7 and M=[[1,1],[1,0]],
   then verifies that k is the minimal exponent.  The sequence
   values are stored in a static cache so that calls in any order are
   O(1) after the initial generation.  */
#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#include "../utils/is_prime_big.h"   /* bool is_prime_big(seq_t) */
#include "../utils/next_prime.h"     /* uint64_t next_prime(uint64_t) */

/* 2x2 matrix over the integers modulo a prime. */
typedef struct { uint64_t a, b, c, d; } mat_t;

/* modular multiplication using 128‑bit intermediate product. */
static inline uint64_t mod_mul(uint64_t x, uint64_t y, uint64_t m)
{
    return (unsigned __int128)x * y % m;
}

/* matrix multiplication C = A*B mod m. */
static void mat_mul(const mat_t *A, const mat_t *B, uint64_t m, mat_t *C)
{
    C->a = (mod_mul(A->a, B->a, m) + mod_mul(A->b, B->c, m)) % m;
    C->b = (mod_mul(A->a, B->b, m) + mod_mul(A->b, B->d, m)) % m;
    C->c = (mod_mul(A->c, B->a, m) + mod_mul(A->d, B->c, m)) % m;
    C->d = (mod_mul(A->c, B->b, m) + mod_mul(A->d, B->d, m)) % m;
}

/* binary exponentiation of a matrix.  result = M^e (mod m).  */
static void mat_pow(const mat_t *M, uint64_t e, uint64_t m, mat_t *result)
{
    mat_t res = {1,0,0,1};
    mat_t base = *M;
    while (e) {
        if (e & 1) {
            mat_t tmp;
            mat_mul(&res, &base, m, &tmp);
            res = tmp;
        }
        e >>= 1;
        if (e) {
            mat_t tmp;
            mat_mul(&base, &base, m, &tmp);
            base = tmp;
        }
    }
    *result = res;
}

/* test whether M^e ≡ I (mod m).  */
static bool mat_pow_is_identity(const mat_t *M, uint64_t e, uint64_t mod)
{
    mat_t R;
    mat_pow(M, e, mod, &R);
    return R.a == 1 && R.b == 0 && R.c == 0 && R.d == 1;
}

/* integer square root (floor) of a 64‑bit unsigned integer. */
static uint64_t isqrt_u64(uint64_t x)
{
    uint64_t lo = 0, hi = 0xFFFFFFFFULL;
    while (lo <= hi) {
        uint64_t mid = lo + ((hi - lo) >> 1);
        if (mid > 0 && mid > x / mid) hi = mid - 1;
        else {
            if ((mid + 1) > 0 && (mid + 1) > x / (mid + 1)) return mid;
            lo = mid + 1;
        }
    }
    return lo;
}

/* factor a 64‑bit unsigned integer into its unique prime factors.
   The results are written into fac and the count into *cnt.  */
static void factor_unique(uint64_t n, uint64_t *fac, size_t *cnt)
{
    *cnt = 0;
    if (n % 2 == 0) {
        fac[(*cnt)++] = 2;
        while (n % 2 == 0) n /= 2;
    }
    for (uint64_t p = 3; p <= isqrt_u64(n); p += 2) {
        if (n % p == 0) {
            fac[(*cnt)++] = p;
            while (n % p == 0) n /= p;
        }
    }
    if (n > 1) fac[(*cnt)++] = n;
}

/* ----- cache and generation machinery ----- */
static seq_t *cache = NULL;          /* array of found primes (seq_t) */
static size_t cache_n = 0;           /* number of primes stored          */
static size_t cache_cap = 0;         /* allocated capacity                */
static uint64_t last_checked = 2;    /* last prime examined              */

/* enlarge cache capacity if needed.  */
static void ensure_cache(size_t need)
{
    if (cache_cap < need) {
        size_t new_cap = need + 16;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) { /* allocation failed – crash gracefully */
            abort();
        }
        cache = tmp;
        cache_cap = new_cap;
    }
}

/* ----- main sequence function ----- */
seq_t a(idx_t n)
{
    if (n == 0) return 0;   /* offset 1: n>=1 */

    while (cache_n < (size_t)n) {
        /* find next prime candidate p≡6 (mod 7) */
        uint64_t p = next_prime(last_checked + 1);
        last_checked = p;
        if (p % 7 != 6) continue;          /* only primes with (p+1)/7 integer */

        /* compute k = 2*(p+1)/7 (exact division) */
        uint64_t k = (uint64_t)(((__int128)p + 1) * 2) / 7;

        /* check that M^k ≡ I (mod p) */
        const mat_t M = {1,1,1,0};
        if (!mat_pow_is_identity(&M, k, p)) continue;

        /* determine minimal exponent by dividing out factors of k */
        uint64_t order = k;
        uint64_t facs[64];
        size_t fc;
        factor_unique(order, facs, &fc);
        for (size_t i = 0; i < fc; ++i) {
            uint64_t f = facs[i];
            while (order % f == 0 && mat_pow_is_identity(&M, order / f, p))
                order /= f;
        }
        if (order != k) continue;  /* not minimal */

        /* accept p */
        ensure_cache(cache_n + 1);
        cache[cache_n++] = p;
    }

    return cache[(size_t)n - 1];
}
