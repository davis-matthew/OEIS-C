/*
 * A377866  Number of subwords of the form DUUD or DDUUD in nondecreasing Dyck
 *          paths of length 2n.
 * https://oeis.org/A377866
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (2*n*L(2*n-5) - 6*F(2*n-6) - F(2*n-7))/5 for n>=3
 * Range:     u8 n <= 5, u16 n <= 10, u32 n <= 21, u64 n <= 28, u128 n <= 28,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* ------------------------------------------------------------------ */
/*  Helper: fast doubling Fibonacci, returns F(k)                      */
/* ------------------------------------------------------------------ */
static void fib_pair_rec(idx_t n, seq_t *fn, seq_t *fn1)
{
    if (n == 0) {
        *fn = (seq_t)0;
        *fn1 = (seq_t)1;
        return;
    }
    seq_t a, b;
    fib_pair_rec(n >> 1, &a, &b);          /* a = F(k), b = F(k+1), k = n/2 */

    seq_t two_b = b + b;                   /* 2*F(k+1) */
    seq_t c = a * (two_b - a);             /* F(2k) = F(k)*(2*F(k+1) – F(k)) */
    seq_t d = a * a + b * b;               /* F(2k+1) = F(k)^2 + F(k+1)^2 */

    if ((n & 1) == 0) {
        *fn = c;    /* F(2k)  */
        *fn1 = d;   /* F(2k+1)*/
    } else {
        *fn = d;    /* F(2k+1)*/
        *fn1 = c + d; /* F(2k+2) */
    }
}

static seq_t get_fib(idx_t n)
{
    seq_t fn, fn1;
    fib_pair_rec(n, &fn, &fn1);
    return fn;
}

/* ------------------------------------------------------------------ */
/*  Main sequence function                                            */
/* ------------------------------------------------------------------ */
seq_t a(idx_t n)
{
    if (n < 3)
        return (seq_t)0;

    /* indices needed for the formula */
    idx_t idx_l = 2 * n - 5;          /* argument of L()   */
    idx_t idx_f2 = 2 * n - 6;         /* F(2n-6)           */
    long long idx_f3 = 2 * (long long)n - 7; /* F(2n-7), may be -1 */

    /* Fibonacci values */
    seq_t F_m2 = get_fib(idx_f2);                 /* F(2n-6)  */
    seq_t F_m3;
    if (idx_f3 < 0)                     /* only n==3 gives -1 */
        F_m3 = (seq_t)1;                /* F(-1) = 1 by extension */
    else
        F_m3 = get_fib((idx_t)idx_f3);  /* F(2n-7)  */

    /* Lucas value L(2n-5) = F(2n-6) + F(2n-4) */
    seq_t F_m1p1 = get_fib(idx_l + 1);   /* F(2n-4) */
    seq_t L_m1 = F_m2 + F_m1p1;          /* L(2n-5) */

    /* numerator: 2*n*L - 6*F(2n-6) - F(2n-7) */
    seq_t two_n = (seq_t)2 * (seq_t)n;
    seq_t numerator = two_n * L_m1 - (seq_t)6 * F_m2 - F_m3;

    /* division by 5 (exact) */
    return numerator / (seq_t)5;
}
