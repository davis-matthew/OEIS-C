/*
 * A353559  a(n) = 1 if A003968(n) is a multiple of n, where A003968 is
 *          multiplicative with a(p^e) = p*(p+1)^(e-1), otherwise 0.
 * https://oeis.org/A353559
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 1 iff A003968(n) is a multiple of n
 * Range:     u8 n <= 622, u16 n <= 65537, u32 n <= 65537, u64 n <= 65537,
 *            u128 n <= 65537, big any n
 * Validated: standard, pass, 136 terms through n = 8831 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Standard utilities for factorization and modular exponentiation are
 * available in the library for this project.  They are included below
 * via the prescribed header paths. */
#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

/* Trial‑division factorisation producing distinct primes
   pr[i] and their multiplicities ex[i].  The caller
   must provide arrays large enough for the factors. */
static void factor_u64(uint64_t n, uint64_t *pr, uint64_t *ex, size_t *cnt)
{
    size_t i = 0;
    if (n <= 1) { *cnt = 0; return; }
    /* Factor 2. */
    while ((n & 1) == 0) {
        if (i && pr[i - 1] == 2) {
            ++ex[i - 1];
        } else {
            pr[i] = 2;
            ex[i] = 1;
            ++i;
        }
        n >>= 1;
    }
    /* Odd factors. */
    for (uint64_t p = 3; p <= n / p; p += 2) {
        if (n % p == 0) {
            pr[i] = p;
            ex[i] = 0;
            while (n % p == 0) {
                ++ex[i];
                n /= p;
            }
            ++i;
        }
    }
    if (n > 1) {
        pr[i] = n;
        ex[i] = 1;
        ++i;
    }
    *cnt = i;
}

/* modular exponentiation using 128‑bit intermediate.
   Works for any seq_t. */
static seq_t mod_pow(seq_t base, uint64_t exp, seq_t mod)
{
    seq_t res = 1 % mod;
    base   %= mod;
    while (exp) {
        if (exp & 1)
            res = (unsigned __int128)res * base % mod;
        exp >>= 1;
        if (exp)
            base = (unsigned __int128)base * base % mod;
    }
    return res;
}

/* Sequence element type and index type are provided by the build. */
seq_t a(idx_t n)
{
    /* By definition the sequence is indexed from 1.  No meaningful value
     * is defined for n == 0, but the function is defensive and just
     * returns 0 in that case. */
    if (n == 0) return 0;

    /* Factor n into primes and exponents.  The arrays are large enough
     * for any 64‑bit integer. */
    size_t cnt = 0;
    uint64_t primes[64];
    uint64_t exps[64];
    factor_u64(n, primes, exps, &cnt);

    seq_t mod = (seq_t)n;   /* modulus for divisibility test */
    seq_t prod = 1;         /* running product of A003968 modulo n */

    for (size_t i = 0; i < cnt; ++i) {
        uint64_t p = primes[i];
        uint64_t e = exps[i];

        /* term = p * (p+1)^(e-1) mod n */
        seq_t term = (seq_t)(p % n);                     /* p mod n    */
        if (e > 1) {
            seq_t powmod = mod_pow((seq_t)(p + 1), e - 1, mod);
            term = (term * powmod) % mod;
        }

        prod = (prod * term) % mod;
    }

    /* Ensure prod is taken modulo n in the special case that n == 1.
     * 1 mod 1 == 0, but prod was initialized to 1 and remained 1. */
    prod %= mod;

    /* The return value is 1 when the product is 0 modulo n,
     * i.e. when A003968(n) is a multiple of n. */
    return (prod == 0) ? (seq_t)1 : (seq_t)0;
}
