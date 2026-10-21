/*
 * A254884  a(n) = Fibonacci(2*n) + ((-1)^n-1)*Fibonacci(n).
 * https://oeis.org/A254884
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = Fibonacci(2*n) + ((-1)^n-1)*Fibonacci(n)
 * Range:     i8 n <= 5, i16 n <= 11, i32 n <= 23, i64 n <= 30, i128 n <= 30,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* (F(0)=0, F(1)=1, ...  standard Fibonacci)                 */

#include <stdint.h>

/*----- Fast‑doubling Fibonacci for any seq_t ---------------------------------*/
static inline void fib_pair(idx_t n, seq_t *fn, seq_t *fn1)
{
    if (n == 0) {
        *fn  = 0;
        *fn1 = 1;
        return;
    }
    seq_t a, b;               /* a = F(k), b = F(k+1) where k = n/2          */
    fib_pair(n >> 1, &a, &b);

    seq_t two_b = b << 1;                /* 2*F(k+1)                               */
    seq_t c     = a * (two_b - a);       /* F(2k)   = F(k) * (2*F(k+1) - F(k))      */
    seq_t d     = a * a + b * b;         /* F(2k+1) = F(k)^2 + F(k+1)^2              */

    if ((n & 1) == 0) {
        *fn  = c;
        *fn1 = d;
    } else {
        *fn  = d;
        *fn1 = c + d;
    }
}

static inline seq_t fib_val(idx_t n)
{
    seq_t f, f1;
    fib_pair(n, &f, &f1);
    return f;
}

/*----- Sequence a(n) ---------------------------------------------------------*/
seq_t a(idx_t n)
{
    seq_t f2 = fib_val(2 * n);   /* Fibonacci(2n)                        */
    seq_t fn = fib_val(n);       /* Fibonacci(n)                         */

    if ((n & 1ULL) == 0ULL) {     /* n even: (-1)^n-1 = 0                  */
        return f2;
    } else {                     /* n odd:  (-1)^n-1 = -2                 */
        return f2 - (seq_t)2 * fn;
    }
}
