/*
 * A395605  a(n) = ((Lucas(prime(n)) - 1) / prime(n)) mod prime(n).
 * https://oeis.org/A395605
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, look
 * Formula:   a(n) = ((Lucas(prime(n)) - 1) / prime(n)) mod prime(n)
 * Range:     u8 n <= 6, u16 n <= 54, u32 n <= 6399, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/get_prime.h"

/* modular multiplication: (a * b) % mod for any seq_t type */
static inline seq_t mod_mul_seq(seq_t a, seq_t b, seq_t mod) {
    seq_t res = 0;
    /* binary multiplication – safe for uint64_t and arbitrary‑precision types */
    while (b) {
        if (b & 1) res = (res + a) % mod;
        a = (a << 1) % mod;
        b >>= 1;
    }
    return res;
}

/* fast doubling to compute F(n) and F(n+1) modulo mod */
static void fib_mod_u64(idx_t n, seq_t mod, seq_t *fn, seq_t *fn1) {
    if (n == 0) {
        *fn  = 0;
        *fn1 = 1;
        return;
    }
    fib_mod_u64(n >> 1, mod, fn, fn1);
    seq_t a = *fn;    /* F(k)   */
    seq_t b = *fn1;   /* F(k+1) */

    seq_t two_b   = (b + b) % mod;          /* 2*F(k+1) mod mod */
    seq_t t_minus = (two_b + mod - a) % mod;/* (2*F(k+1)-F(k)) */
    seq_t c = mod_mul_seq(a, t_minus, mod);/* F(2k) */

    seq_t a2 = mod_mul_seq(a, a, mod);
    seq_t b2 = mod_mul_seq(b, b, mod);
    seq_t d = (a2 + b2) % mod;              /* F(2k+1) */

    if (n & 1) {
        *fn  = d;
        *fn1 = (c + d) % mod;
    } else {
        *fn  = c;
        *fn1 = d;
    }
}

/* sequence term */
seq_t a(idx_t n) {
    /* nth prime (1‑based) */
    uint64_t p_uint = get_prime(n);
    seq_t p = (seq_t)p_uint;
    seq_t mod = p * p;                    /* modulo p² */

    /* Lucas(p) modulo p² : Lucas(p) = 2*F(p+1) – F(p) */
    seq_t fp, fpp;
    fib_mod_u64(p_uint, mod, &fp, &fpp);

    seq_t two_fpp   = (fpp + fpp) % mod;
    seq_t lucas_mod = (two_fpp + mod - fp) % mod; /* 0 ≤ lucas_mod < mod */

    /* (Lucas(p) – 1) / p  (exact division) */
    seq_t r = (lucas_mod - 1) / p;
    return r;
}
