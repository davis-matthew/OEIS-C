/*
 * A268613  Lucas numbers mod 20.
 * https://oeis.org/A268613
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes Lucas numbers modulo 20
   a(0) = 2, a(1) = 1, a(n) = a(n-1) + a(n-2) (mod 20)  */
#include <stdbool.h>
#include <stdint.h>

/* Fast‑doubling Fibonacci pair (F(n), F(n+1)) modulo 20 */
static void fib_pair(idx_t n, seq_t* fn, seq_t* fn1) {
    const seq_t MOD = (seq_t)20;
    if (n == 0) {
        *fn  = (seq_t)0;
        *fn1 = (seq_t)1;
        return;
    }
    seq_t a, b;
    fib_pair(n >> 1, &a, &b);                     /* (F(k), F(k+1)) where k = n/2 */

    seq_t two_b = (b * (seq_t)2) % MOD;
    seq_t c = (a * ((two_b + MOD - a) % MOD)) % MOD;   /* F(2k) */
    seq_t d = ((a * a) % MOD + (b * b) % MOD) % MOD;   /* F(2k+1) */

    if (n & 1) {  /* odd n */
        *fn  = d;
        *fn1 = (c + d) % MOD;
    } else {     /* even n */
        *fn  = c;
        *fn1 = d;
    }
}

seq_t a(idx_t n) {
    const seq_t MOD = (seq_t)20;
    if (n == 0) return (seq_t)2;   /* L(0) */
    seq_t fn, fn1;
    fib_pair(n, &fn, &fn1);          /* fn = F(n), fn1 = F(n+1) */

    seq_t fn_minus1 = (fn1 + MOD - fn) % MOD;     /* F(n-1) = F(n+1) - F(n) */
    return (fn_minus1 + fn1) % MOD;               /* L(n) = F(n-1) + F(n+1) */
}
