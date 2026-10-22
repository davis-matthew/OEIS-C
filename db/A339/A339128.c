/*
 * A339128  Odd composite integers m such that A086902(m-J(m,53)) == 2*J(m,53)
 *          (mod m), where J(m,53) is the Jacobi symbol.
 * https://oeis.org/A339128
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is the n‑th odd composite m such that V(m−J(m,53)) ≡
 *            2·J(m,53) (mod m) where V is the Pell‑Lucas sequence with
 *            parameters a=7, b=−1, V(0)=2, V(1)=7, and J is the Jacobi
 *            symbol. The implementation performs a brute‑force search with a
 *            cache; it does not hard‑code any terms
 * Range:     u8 n <= 9, u16 n <= 48, u32 n <= 48, u64 n <= 48, u128 n <= 48,
 *            big any n
 * Helpers:   jacobi_symbol_u64, modmul_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/jacobi_symbol_u64.h"
#include "../utils/modmul_u64.h"

/* ------------------------ helpers ------------------------------------ */

/* Deterministic Miller–Rabin for 64‑bit integers. */
static bool is_prime_u64(uint64_t n)
{
    if (n < 2) return false;
    static const uint64_t small_primes[] = {2,3,5,7,11,13,17,19,23,29,31,37,41,43,47,53,59,61,67,71,73,79,83,89,97};
    for (size_t i = 0; i < sizeof small_primes / sizeof *small_primes; ++i)
    {
        uint64_t p = small_primes[i];
        if (n % p == 0) return n == p;
    }
    uint64_t d = n - 1, s = 0;
    while ((d & 1) == 0) { d >>= 1; ++s; }
    static const uint64_t bases[] = {2,325,9375,28178,450775,9780504,1795265022};
    for (size_t i = 0; i < sizeof bases / sizeof *bases; ++i)
    {
        uint64_t a = bases[i] % n;
        if (a == 0) continue;
        uint64_t x = 1, base = a;
        uint64_t exp = d;
        while (exp)
        {
            if (exp & 1) x = modmul_u64(x, base, n);
            base = modmul_u64(base, base, n);
            exp >>= 1;
        }
        if (x == 1 || x == n-1) continue;
        bool comp = true;
        for (uint64_t r = 1; r < s; ++r)
        {
            x = modmul_u64(x, x, n);
            if (x == n-1) { comp = false; break; }
        }
        if (comp) return false;
    }
    return true;
}

/* 2×2 matrix over uint64_t modulo mod. */
typedef struct { uint64_t a[2][2]; } Mat_u64;

static Mat_u64 matMul_u64(Mat_u64 x, Mat_u64 y, uint64_t mod)
{
    Mat_u64 r;
    r.a[0][0] = (modmul_u64(x.a[0][0], y.a[0][0], mod) + modmul_u64(x.a[0][1], y.a[1][0], mod)) % mod;
    r.a[0][1] = (modmul_u64(x.a[0][0], y.a[0][1], mod) + modmul_u64(x.a[0][1], y.a[1][1], mod)) % mod;
    r.a[1][0] = (modmul_u64(x.a[1][0], y.a[0][0], mod) + modmul_u64(x.a[1][1], y.a[1][0], mod)) % mod;
    r.a[1][1] = (modmul_u64(x.a[1][0], y.a[0][1], mod) + modmul_u64(x.a[1][1], y.a[1][1], mod)) % mod;
    return r;
}

static Mat_u64 matPow_u64(uint64_t exp, uint64_t mod)
{
    Mat_u64 result = { { {1 % mod, 0}, {0, 1 % mod} } };
    Mat_u64 base   = { { {7 % mod, 1 % mod}, {1 % mod, 0} } };
    while (exp)
    {
        if (exp & 1) result = matMul_u64(result, base, mod);
        base = matMul_u64(base, base, mod);
        exp >>= 1;
    }
    return result;
}

/* Compute V(k) mod m (V recurrence with a=7, b=-1). */
static uint64_t V_mod_uint64(uint64_t k, uint64_t mod)
{
    if (mod == 1) return 0;
    if (k == 0) return 2 % mod;
    if (k == 1) return 7 % mod;
    Mat_u64 p = matPow_u64(k - 1, mod);
    uint64_t term1 = modmul_u64(p.a[0][0], 7 % mod, mod);
    uint64_t term2 = modmul_u64(p.a[0][1], 2 % mod, mod);
    return (term1 + term2) % mod;
}

/* ------------------------ main function ------------------------------ */

seq_t a(idx_t n)
{
    /* static state for cache */
    static seq_t *cache = NULL;
    static size_t cache_len = 0, cache_cap = 0;
    static uint64_t next_candidate = 9;    /* first odd composite */

    /* grow cache until it holds at least n elements */
    while (cache_len < n)
    {
        uint64_t m = next_candidate;

        /* test odd composite */
        if ((m & 1) && !is_prime_u64(m))
        {
            int J = jacobi_symbol_u64(m, 53U);
            bool ok = false;
            uint64_t target, vm;

            if (J == 1)
            {
                target = 2 % m;
                vm = V_mod_uint64(m - 1, m);
                ok = (vm == target);
            }
            else if (J == -1)
            {
                target = (m - 2) % m;
                vm = V_mod_uint64(m + 1, m);
                ok = (vm == target);
            }
            else /* J == 0 */
            {
                vm = V_mod_uint64(m, m);
                ok = (vm == 0);
            }

            if (ok)
            {
                /* store value, casting to seq_t (wrap-around OK if out of range) */
                if (cache_len == cache_cap)
                {
                    cache_cap = cache_cap ? cache_cap * 2 : 16;
                    cache = (seq_t *)realloc(cache, cache_cap * sizeof(seq_t));
                }
                cache[cache_len++] = (seq_t)m;
            }
        }

        next_candidate += 2;          /* next odd number */
    }

    return cache[n - 1];
}
