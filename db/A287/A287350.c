/*
 * A287350  Number of independent vertex sets and vertex covers in the n-gear
 *          graph.
 * https://oeis.org/A287350
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2^n + Lucas(2*n) (Lucas(k) is the k‑th Lucas number with
 *            L(0)=2, L(1)=1, L(k)=L(k-1)+L(k-2))
 * Range:     u8 n <= 5, u16 n <= 11, u32 n <= 23, u64 n <= 46, u128 n <= 92,
 *            big any n
 * Helpers:   pow_uint
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow_uint.h"

/* fast doubling Fibonacci: returns F(n) in *fn, F(n+1) in *fn1 */
static void fib_fast(idx_t n, seq_t *fn, seq_t *fn1)
{
    if (n == 0) {
        *fn  = (seq_t)0;
        *fn1 = (seq_t)1;
        return;
    }
    seq_t f, f1;
    fib_fast(n >> 1, &f, &f1);          /* f  = F(k),   f1 = F(k+1)  where k = n/2 */

    seq_t two_f1 = (f1 << 1);             /* 2*F(k+1) */
    seq_t c = f * (two_f1 - f);           /* F(2k)   = F(k)*[2*F(k+1) – F(k)] */
    seq_t d = f * f + f1 * f1;            /* F(2k+1) = F(k)^2 + F(k+1)^2 */

    if ((n & 1) == 0) {
        *fn  = c;
        *fn1 = d;
    } else {
        *fn  = d;
        *fn1 = c + d;
    }
}

seq_t a(idx_t n)
{
    /* 2^n */
    seq_t pow2 = pow_uint((uint64_t)2, n);

    /* compute Fibonacci numbers for k = 2*n */
    idx_t k = n << 1;                 /* k = 2*n */
    seq_t f_k, f_kp1;
    fib_fast(k, &f_k, &f_kp1);        /* f_k  = F(k),  f_kp1 = F(k+1) */

    /* Lucas(2n) = 2*F(k+1) - F(k) */
    seq_t lucas = (f_kp1 << 1) - f_k;

    /* a(n) = 2^n + Lucas(2n) */
    return pow2 + lucas;
}
