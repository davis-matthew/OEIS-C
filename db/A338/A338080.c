/*
 * A338080  Odd composite integers such that A005668(m)^2 == 1 (mod m).
 * https://oeis.org/A338080
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 51, u32 n <= 51, u64 n <= 51, u128 n <= 51,
 *            big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The sequence a(n) lists odd composite integers m for which the
   Lucas sequence defined by U(0)=0, U(1)=1 and
   U(k+2)=6·U(k+1)+U(k) satisfies    U(m)^2 ≡ 1 (mod m).
   The function a(n) finds the nth such integer by brute‑force search. */

#include <stdbool.h>
#include "../utils/is_prime.h"

static inline seq_t mul_mod(seq_t a, seq_t b, seq_t mod)
{
    return (seq_t)(((unsigned __int128)a * b) % mod);
}

/* Compute U(n) modulo mod using fast matrix exponentiation for the
   recurrence U(k+2)=6·U(k+1)+U(k).  */
static seq_t lucas_mod(idx_t n, seq_t mod)
{
    if (n == 0) return 0;
    /* Base matrix [[6,1],[1,0]] */
    seq_t r00 = 1, r01 = 0, r10 = 0, r11 = 1;   /* identity */
    seq_t a00 = 6 % mod, a01 = 1 % mod, a10 = 1 % mod, a11 = 0;
    idx_t e = n - 1;

    while (e) {
        if (e & 1) {
            seq_t t00 = mul_mod(r00, a00, mod) + mul_mod(r01, a10, mod);
            if (t00 >= mod) t00 -= mod;
            seq_t t01 = mul_mod(r00, a01, mod) + mul_mod(r01, a11, mod);
            if (t01 >= mod) t01 -= mod;
            seq_t t10 = mul_mod(r10, a00, mod) + mul_mod(r11, a10, mod);
            if (t10 >= mod) t10 -= mod;
            seq_t t11 = mul_mod(r10, a01, mod) + mul_mod(r11, a11, mod);
            if (t11 >= mod) t11 -= mod;
            r00 = t00; r01 = t01; r10 = t10; r11 = t11;
        }
        /* square the base matrix */
        seq_t t00 = mul_mod(a00, a00, mod) + mul_mod(a01, a10, mod);
        if (t00 >= mod) t00 -= mod;
        seq_t t01 = mul_mod(a00, a01, mod) + mul_mod(a01, a11, mod);
        if (t01 >= mod) t01 -= mod;
        seq_t t10 = mul_mod(a10, a00, mod) + mul_mod(a11, a10, mod);
        if (t10 >= mod) t10 -= mod;
        seq_t t11 = mul_mod(a10, a01, mod) + mul_mod(a11, a11, mod);
        if (t11 >= mod) t11 -= mod;
        a00 = t00; a01 = t01; a10 = t10; a11 = t11;
        e >>= 1;
    }
    return r00 % mod;   /* U(n) is the (0,0) entry of the matrix times [1,0]ᵀ */
}

seq_t a(idx_t n)
{
    idx_t found = 0;
    for (idx_t m = 9; ; m += 2) {            /* start at first odd composite */
        if (is_prime(m)) continue;          /* skip primes */
        seq_t u   = lucas_mod(m, (seq_t)m); /* U(m) mod m */
        seq_t sq  = mul_mod(u, u, (seq_t)m);/* U(m)^2 mod m */
        if (sq == 1) {
            ++found;
            if (found == n) return (seq_t)m;
        }
    }
}
