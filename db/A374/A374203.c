/*
 * A374203  The 3-adic valuation of A328845(n), where A328845 is a
 *          Fibonacci-based variant of the arithmetic derivative.
 * https://oeis.org/A374203
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = v₃(D(n)) where D(n) = Σ_{p^e∥n} e * F_p * (n/p) (F_p is
 *            the p‑th Fibonacci number, using the recurrence
 *            D(ab)=aD(b)+bD(a)). The 3‑adic valuation is the maximum exponent
 *            t such that 3ᵗ divides D(n)
 * Range:     u8 n <= 1942, u16 n <= 9517, u32 n <= 9517, u64 n <= 9517,
 *            u128 n <= 9517, big any n
 * Helpers:   factorize
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include "../utils/factorize.h"

/* multiply modulo mod, safe for 64‑bit and 128‑bit seq_t */
static inline seq_t mul_mod(seq_t a, seq_t b, seq_t mod)
{
    if (sizeof(seq_t) <= 8) {
        unsigned __int128 prod = (unsigned __int128)(a % mod) * (unsigned __int128)(b % mod);
        return (seq_t)(prod % mod);
    } else {
        return (a % mod) * (b % mod) % mod;
    }
}

/* fast doubling, returns F(n) and F(n+1) modulo mod */
static inline void fib_pair_mod(idx_t n, seq_t mod, seq_t *fn, seq_t *fn1)
{
    if (n == 0) {
        *fn  = 0;
        *fn1 = 1 % mod;
        return;
    }
    seq_t a, b;
    fib_pair_mod(n >> 1, mod, &a, &b);          /* a=F(k), b=F(k+1) */
    seq_t two_b = mul_mod((seq_t)2, b, mod);     /* 2*b mod mod */
    seq_t temp  = (two_b + mod - a) % mod;       /* (2*b - a) mod mod   */
    seq_t c     = mul_mod(a, temp, mod);         /* F(2k)  */
    seq_t d     = (mul_mod(a, a, mod) + mul_mod(b, b, mod)) % mod; /* F(2k+1) */
    if (n & 1) {
        *fn  = d;
        *fn1 = (c + d) % mod;
    } else {
        *fn  = c;
        *fn1 = d;
    }
}

/* F(n) modulo mod */
static inline seq_t fib_mod(idx_t n, seq_t mod)
{
    seq_t fn, fn1;
    fib_pair_mod(n, mod, &fn, &fn1);
    return fn;
}

/* compute D(n) modulo given modulus */
static seq_t D_mod(idx_t n, size_t cnt, uint64_t *primes, int *exps, seq_t mod)
{
    seq_t sum = 0;
    for (size_t i = 0; i < cnt; ++i) {
        seq_t e  = (seq_t)exps[i];
        seq_t p  = (seq_t)primes[i];
        seq_t np = (seq_t)n / primes[i];          /* n/p */
        /* modulo reductions to keep numbers small */
        seq_t np_mod = np % mod;
        seq_t Fp = fib_mod(primes[i], mod);
        seq_t eF = mul_mod(e % mod, Fp, mod);
        seq_t term = mul_mod(eF, np_mod, mod);
        sum += term;
        if (sum >= mod) sum -= mod;
    }
    return sum;
}

/* ===================================================================== */
/* External function required by the harness                            */
/* ===================================================================== */
seq_t a(idx_t n)
{
    if (n < 2) return 0;                      /* below offset – undefined */

    /* factorize n --------------------------------------------------- */
    uint64_t primes[64];
    int      exps[64];
    size_t   cnt = factorize((uint64_t)n, primes, exps);

    /* iterate over powers of 3 to find v₃(D(n)) ----------------------- */
    seq_t power_of_three = 3;                  /* 3¹ */
    int   k = 1;
    while (true) {
        seq_t Dmod = D_mod(n, cnt, primes, exps, power_of_three);
        if (Dmod != 0) return (seq_t)(k - 1); /* first non‑zero => v₃ found */
        /* try next power of 3 -------------------------------------- */
        power_of_three *= 3;
        k++;
    }
}
