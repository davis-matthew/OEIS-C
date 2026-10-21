/*
 * A245070  Smallest positive non-divisor of the n-th Lucas number (A000032).
 * https://oeis.org/A245070
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1000, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A245070:
   Smallest positive non‑divisor of the n-th Lucas number L(n),
   where L(0)=2, L(1)=1, L(n)=L(n‑1)+L(n‑2).  */
#include <stdint.h>

/* Fast doubling Fibonacci modulo `mod`.  Returns F(n) and F(n+1). */
static void fib_pair_mod(idx_t n, seq_t mod, seq_t *fn, seq_t *fn1)
{
    if (n == 0) {
        *fn  = (seq_t)0;
        *fn1 = (seq_t)(1 % mod);
        return;
    }
    seq_t a = (seq_t)0;                /* F(0)   */
    seq_t b = (seq_t)(1 % mod);        /* F(1)   */
    /* highest set bit of n */
    idx_t bit = (idx_t)1 << (63 - __builtin_clzll(n));
    while (bit) {
        /* c = a * (2*b - a) mod mod   (F(2k)) */
        seq_t two_b = (b + b) % mod;
        seq_t t     = (two_b + mod - a) % mod;
        seq_t c     = ((unsigned long long)a * (unsigned long long)t) % (unsigned long long)mod;
        /* d = a*a + b*b mod mod   (F(2k+1)) */
        seq_t d = (((unsigned long long)a * a) % (unsigned long long)mod +
                  ((unsigned long long)b * b) % (unsigned long long)mod) % (unsigned long long)mod;

        if ((n & bit) == 0) {
            a = c;
            b = d;
        } else {
            a = d;
            b = (c + d) % mod;
        }
        bit >>= 1;
    }
    *fn  = a;   /* F(n)   */
    *fn1 = b;   /* F(n+1) */
}

/* Lucas number modulo `mod`; uses the relation L(n)=2*F(n+1)-F(n). */
static seq_t lucas_mod(idx_t n, seq_t mod)
{
    seq_t fn, fn1;
    fib_pair_mod(n, mod, &fn, &fn1);
    seq_t two_fn1 = (fn1 + fn1) % mod;                 /* 2*F(n+1) */
    seq_t res     = (two_fn1 + mod - fn) % mod;        /* 2*F(n+1)-F(n) */
    return res;
}

/* The sequence: smallest positive integer that does NOT divide L(n). */
seq_t a(idx_t n)
{
    for (uint64_t k = 2;; ++k) {                     /* start at 2  */
        seq_t mod   = (seq_t)k;                      /* modulus = k   */
        seq_t lmod  = lucas_mod(n, mod);             /* L(n) mod k   */
        if (lmod != (seq_t)0)                         /* not divisible */
            return (seq_t)k;                         /* answer is k   */
    }
}
