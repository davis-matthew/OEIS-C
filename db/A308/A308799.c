/*
 * A308799  Primes p such that A001177(p) = (p-1)/6.
 * https://oeis.org/A308799
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 40, u32 n <= 40, u64 n <= 40, u128 n <= 40,
 *            big any n
 * Helpers:   next_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A308799: primes p such that the rank of appearance of p in the
 * Fibonacci sequence equals (p-1)/6.  This is equivalent to:
 *   A001177(p) = (p-1)/6
 * where A001177(n) is the smallest k with n | F_k.
 *
 * The implementation uses a fast‐doubling Fibonacci modulus routine
 * and checks that (p-1)/6 is the minimal exponent for which F_k ≡ 0 mod p.
 * It generates primes on demand with the helper `next_prime`.  The
 * sequence values are cached to allow calls in any order.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>

/* Include helper that provides next_prime(uint64_t start) */
#include "../utils/next_prime.h"

/* --- modular arithmetic helpers ---------------------------------------- */
static uint64_t mul_mod64(uint64_t a, uint64_t b, uint64_t mod)
{
    return (uint64_t)((unsigned __int128)a * b % mod);
}

/* --- fast doubling Fibonacci modulo `mod` ----------------------------- */
static void fib_mod_uint64(uint64_t n, uint64_t mod,
                           uint64_t *f_n, uint64_t *f_n1)
{
    if (n == 0) {
        *f_n  = 0;
        *f_n1 = 1;
        return;
    }
    fib_mod_uint64(n >> 1, mod, f_n, f_n1);    /* recursion: f_n=F_k, f_n1=F_{k+1} */
    uint64_t a = *f_n;
    uint64_t b = *f_n1;
    uint64_t two_b = mul_mod64(b, 2, mod);
    uint64_t c = mul_mod64(a, (two_b + mod - a) % mod, mod);   /* F_{2k}   */
    uint64_t d = (mul_mod64(a, a, mod) + mul_mod64(b, b, mod)) % mod; /* F_{2k+1} */

    if (n & 1) {
        *f_n  = d;
        *f_n1 = (c + d) % mod;
    } else {
        *f_n  = c;
        *f_n1 = d;
    }
}

/* --- trial division factorisation of n (uint64_t) --------------------- */
static void factorize_uint64(uint64_t n,
                             uint64_t *primes, int *exps, size_t *cnt)
{
    size_t i = 0;
    if (n % 2 == 0) {
        primes[i] = 2;
        exps[i]   = 0;
        while (n % 2 == 0) { n /= 2; exps[i]++; }
        i++;
    }
    for (uint64_t d = 3; d * d <= n; d += 2) {
        if (n % d == 0) {
            primes[i] = d;
            exps[i]   = 0;
            while (n % d == 0) { n /= d; exps[i]++; }
            i++;
        }
    }
    if (n > 1) {
        primes[i] = n;
        exps[i]   = 1;
        i++;
    }
    *cnt = i;
}

/* --- primality test for the special condition ------------------------ */
static bool good_prime(uint64_t p)
{
    /* p must be 1 mod 6 to ensure integer (p-1)/6 */
    if (p % 6u != 1u)
        return false;

    uint64_t k = (p - 1) / 6;            /* candidate rank */
    uint64_t f_k, f_k1;
    fib_mod_uint64(k, p, &f_k, &f_k1);
    if (f_k % p != 0)
        return false;                    /* k not a rank */

    /* Remove any factor that still keeps F_{k/prime} ≡ 0 (mod p) */
    uint64_t prime_factors[32];
    int exps[32];
    size_t fac_cnt;
    factorize_uint64(k, prime_factors, exps, &fac_cnt);

    uint64_t d = k;
    for (size_t i = 0; i < fac_cnt && d > 1; ++i) {
        uint64_t r = prime_factors[i];
        while (d % r == 0) {
            uint64_t cand = d / r;
            fib_mod_uint64(cand, p, &f_k, &f_k1);
            if (f_k % p == 0)
                d = cand;            /* still a rank: reduce further */
            else
                break;
        }
    }
    return d == k;                    /* minimality check */
}

/* --- static cache for sequence terms ------------------------------- */
static seq_t *seq_cache       = NULL;
static size_t seq_size        = 0;
static size_t seq_capacity    = 0;

static uint64_t last_checked_prime = 5;     /* start after prime 5 */

/* --- dynamic array helpers ---------------------------------------- */
static void grow_cache(size_t needed)
{
    if (seq_capacity >= needed)
        return;
    seq_capacity = needed * 2;
    seq_cache = (seq_t *)realloc(seq_cache, seq_capacity * sizeof(seq_t));
}

static void append_term(seq_t val)
{
    grow_cache(seq_size + 1);
    seq_cache[seq_size++] = val;
}

/* --- main sequence function ---------------------------------------- */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;                      /* unused (offset 1) */

    /* generate terms until we have at least n of them */
    while (seq_size < (size_t)n) {
        last_checked_prime = next_prime(last_checked_prime); /* next prime > previous */
        if (good_prime(last_checked_prime))
            append_term((seq_t)last_checked_prime);
    }
    return seq_cache[n - 1];
}
