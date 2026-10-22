/*
 * A350901  a(n) = F(n) * (2*F(n-1)^2 + (-1)^(n-1)) * (2*F(n)^2 + (-1)^n),
 *          where F(n) is the n-th Fibonacci number.
 * https://oeis.org/A350901
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 10, u64 n <= 19, u128 n <= 38,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  Sequence A350901, OEIS formula:
    a(n) = F(n) * (2*F(n-1)^2 + (-1)^(n-1)) * (2*F(n)^2 + (-1)^n)
    where F(0)=0, F(1)=1 are the Fibonacci numbers.
    The implementation uses a fast‑doubling recursion for Fibonacci
    so no external fib() helper is required.  The code works for any
    seq_t (uint8_t … A big‑int type) and for any idx_t (uint64_t). */
#include <stdint.h>

/* Fast doubling: compute F(n) and F(n+1) into *fn, *fn1. */
static void fib_pair_recursive(idx_t n, seq_t *fn, seq_t *fn1)
{
    if (n == 0) {
        *fn  = 0;
        *fn1 = 1;
        return;
    }
    seq_t a, b;          /* a = F(k), b = F(k+1), k = n/2 */
    fib_pair_recursive(n >> 1, &a, &b);

    /* F(2k) = F(k) * (2*F(k+1) - F(k)) */
    seq_t temp = (b << 1);            /* 2*F(k+1) */
    seq_t c    = a * (temp - a);

    /* F(2k+1) = F(k)^2 + F(k+1)^2 */
    seq_t d = a * a + b * b;

    if ((n & 1) == 0) {
        *fn  = c;
        *fn1 = d;
    } else {
        *fn  = d;
        *fn1 = c + d;        /* F(2k+2) = F(2k+1) + F(2k) */
    }
}

/* Public function for OEIS A350901 */
seq_t a(idx_t n)
{
    if (n == 0) return 0;   /* a(0) = 0 */

    seq_t fn, fnplus1;          /* fn = F(n), fnplus1 = F(n+1) */
    fib_pair_recursive(n, &fn, &fnplus1);

    /* Compute F(n-1) via the relation F(n-1) = F(n+1) - F(n) */
    seq_t fnminus1 = fnplus1 - fn;

    /* term1 = 2*F(n-1)^2 + (-1)^(n-1) */
    seq_t term1 = fnminus1 * fnminus1;
    term1 = term1 + term1;      /* 2*F(n-1)^2 */
    if ((n - 1) & 1)            /* odd n-1 => subtract 1 */
        term1 = term1 - 1;
    else                        /* even n-1 => add 1 */
        term1 = term1 + 1;

    /* term2 = 2*F(n)^2 + (-1)^n */
    seq_t term2 = fn * fn;
    term2 = term2 + term2;      /* 2*F(n)^2 */
    if (n & 1)                  /* odd n => subtract 1 */
        term2 = term2 - 1;
    else                        /* even n => add 1 */
        term2 = term2 + 1;

    /* a(n) = F(n) * term1 * term2 */
    return fn * term1 * term2;
}
