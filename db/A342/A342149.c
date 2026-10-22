/*
 * A342149  Infinite square matrix A(m,n) = F(m+1) mod (n+1), m,n >= 1, where
 *          F = Fibonacci = A000045, read by falling antidiagonals.
 * https://oeis.org/A342149
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 120, u16 n <= 120, u32 n <= 120, u64 n <= 120,
 *            u128 n <= 120, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A342149: a(n) = F(m+1) mod (ncol+1)
   where n is mapped to row m and column ncol by
   falling antidiagonal traversal of the matrix
   A(m,n) = F(m+1) mod (n+1) (m,n ≥ 1). */

#include <stdint.h>
#include <stdbool.h>

/* Integer square root floor for 64‑bit unsigned integers */
static idx_t floor_sqrt(idx_t x)
{
    idx_t lo = 0, hi = (idx_t)1ULL << 32;   /* 2^32 > sqrt(ULLONG_MAX) */
    while (hi > lo) {
        idx_t mid = (lo + hi + 1) >> 1;
        if (mid * mid <= x)
            lo = mid;
        else
            hi = mid - 1;
    }
    return lo;
}

/* Fast doubling algorithm for Fibonacci numbers modulo *mod*.
   Returns F(n) in *fn* and F(n+1) in *fn1* (both < mod). */
static void fib_pair(idx_t n, idx_t mod, seq_t *fn, seq_t *fn1)
{
    if (n == 0) {
        *fn  = 0;
        *fn1 = 1;
        return;
    }

    seq_t a, b;
    fib_pair(n >> 1, mod, &a, &b);                /* a=F(k), b=F(k+1) */

    seq_t t = ((2 * b) % mod + mod - a) % mod;     /* 2*F(k+1)-F(k) */
    seq_t c = (a * t) % mod;                       /* F(2k)   */
    seq_t d = ((a * a) % mod + (b * b) % mod) % mod; /* F(2k+1) */

    if (n & 1) {
        *fn  = d;
        *fn1 = (c + d) % mod;
    } else {
        *fn  = c;
        *fn1 = d;
    }
}

/* Main sequence function */
seq_t a(idx_t n)
{
    if (n < 1) return 0;        /* undefined for n=0, safeguard */

    /* Find antidiagonal index k such that k(k-1)/2 ≥ n */
    idx_t t    = 1 + 8 * n;
    idx_t isq  = floor_sqrt(t);
    idx_t k    = (1 + isq) / 2;            /* floor of formula */
    while ((k * (k - 1)) / 2 < n) ++k;

    /* Position inside the antidiagonal */
    idx_t prev_triang = ((k - 2) * (k - 1)) / 2;
    idx_t offset      = n - prev_triang;   /* 1 … k-1 */
    idx_t m           = offset;            /* row index (1‑based)   */
    idx_t ncol        = k - offset;        /* column index (1‑based)*/
    idx_t mod         = ncol + 1;          /* modulus for the entry  */

    /* Compute F(m+1) mod (ncol+1) */
    seq_t fn, fn1;
    fib_pair(m + 1, mod, &fn, &fn1);
    return fn;
}
