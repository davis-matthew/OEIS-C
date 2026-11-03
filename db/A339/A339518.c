/*
 * A339518  Odd composite integers m such that A006497(2*m-J(m,13)) ==
 *          3*J(m,13) (mod m), where J(m,13) is the Jacobi symbol.
 * https://oeis.org/A339518
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 43, u32 n <= 43, u64 n <= 43, u128 n <= 43,
 *            big any n
 * Validated: partial, pass, 43 terms through n = 43 (2026-11-03)
 * Helpers:   is_prime_u128, is_prime_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A339518
 *
 *  a(n) is the n-th odd composite integer m such that
 *  A006497(2*m - J(m,13)) ≡ 3*J(m,13) (mod m),
 *  where J(m,13) is the Jacobi symbol and A006497 is the Pell‑Lucas
 *  sequence with parameters (a,b) = (3,-1) (V(0)=2,V(1)=3, V(k+2)=3*V(k+1)+V(k)).
 *
 *  The function searches odd composites (skipping 1,2) and tests the
 *  congruence using a fast matrix‑exponentiation modulo m.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#include <limits.h>
#include "../utils/is_prime_u128.h"
#include "../utils/is_prime_u64.h"

/* Jacobi symbol (a|n) for odd positive n <= 2^64-1.
   Returns 1, 0, or -1. */
static int jacobi_symbol(uint64_t a, uint64_t n) {
    if (n == 0) return 0;
    a %= n;
    if (a == 0) return 0;
    int res = 1;
    while (a != 0) {
        while ((a & 1) == 0) {
            a >>= 1;
            uint64_t n_mod8 = n & 7;
            if (n_mod8 == 3 || n_mod8 == 5)
                res = -res;
        }
        uint64_t tmp = a;
        a = n;
        n = tmp;
        if ((a & 3) == 3 && (n & 3) == 3)
            res = -res;
        a %= n;
    }
    return (n == 1) ? res : 0;
}

/* -----------------  Generic primality test ------------------------ */
static bool isPrimeGeneric(seq_t n)
{
    /* all seq_t are unsigned; use the smallest type that fits */
    if ((seq_t)(ULLONG_MAX) >= n) {
        return is_prime_u64((uint64_t)n);
    }
#if defined(__SIZEOF_INT128__)
    return is_prime_u128((__uint128_t)n);
#else
    /* fallback: if seq_t > 64‑bit but no 128‑bit support,
       order the test as simple trial division up to sqrt */
    seq_t limit = 0;
    for (seq_t i = 2; i*i <= n; ++i) {
        if (n % i == 0) return false;
    }
    return true;
#endif
}

typedef struct { seq_t a,b,c,d; } mat2;

static mat2 mul(mat2 x, mat2 y, seq_t mod) {
    mat2 r;
    r.a = (x.a*y.a + x.b*y.c) % mod;
    r.b = (x.a*y.b + x.b*y.d) % mod;
    r.c = (x.c*y.a + x.d*y.c) % mod;
    r.d = (x.c*y.b + x.d*y.d) % mod;
    return r;
}

static seq_t V_mod(idx_t n, seq_t mod) {
    if (mod == 0) return 0;            /* defensive, should not happen */
    if (n == 0) return 2 % mod;
    if (n == 1) return 3 % mod;

    mat2 T = {3 % mod, 1 % mod, 1 % mod, 0};   /* transition matrix */
    mat2 R = {1, 0, 0, 1};                     /* identity matrix */
    idx_t e = n - 1;

    while (e) {
        if (e & 1) R = mul(R, T, mod);
        T = mul(T, T, mod);
        e >>= 1;
    }
    /* R * [V1, V0]^T gives [V_n, V_{n-1}]^T */
    return ( (R.a * (seq_t)3) + (R.b * (seq_t)2) ) % mod;
}

seq_t a(idx_t n) {
    if (n == 0) return (seq_t)0;
    idx_t count = 0;

    for (idx_t m = 9; ; m += 2) {           /* start with first odd composite */
        if (m % 13 == 0) continue;          /* Jacobi symbol would be 0 */
        if (isPrimeGeneric((seq_t)m)) continue;   /* skip primes */

        int j = jacobi_symbol(m, 13);       /* Jacobi symbol (m/13) */
        if (j == 0) continue;              /* skip multiples of 13 */

        idx_t idx = (j == 1) ? (2*m - 1) : (2*m + 1);
        seq_t V = V_mod(idx, (seq_t)m);

        seq_t rhs = (j == 1) ? (seq_t)3 : ((seq_t)m - (seq_t)3);
        if (V == rhs) {
            if (++count == n) return (seq_t)m;
        }
    }
}
