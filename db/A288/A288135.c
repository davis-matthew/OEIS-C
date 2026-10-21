/*
 * A288135  Coefficients of 1/(Sum_{k>=0} [(k+1)*r]*(-x)^k), where r =
 *          sqrt(7/3) and [ ] = floor.
 * https://oeis.org/A288135
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 7, u16 n <= 15, u32 n <= 31, u64 n <= 33, u128 n <= 33,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A288135
   a(0) = 1
   a(n) = Σ_{k=1..n} (-1)^{k+1} * c_k * a(n-k)   (n ≥ 1)
   where c_k = floor((k+1)*sqrt(7/3)).
   This is equivalent to the generating‑function formula
   1 / ( Σ_{k≥0} floor((k+1)*sqrt(7/3)) * (-x)^k ).
   All arithmetic is performed with integer types only. */

#include <stddef.h>
#include <stdlib.h>

/* Compute floor(sqrt(x)) for a 128‑bit integer x (x < 2^128). */
static inline unsigned long long isqrt128(__uint128_t x)
{
    unsigned long long lo = 0, hi = ((unsigned long long)1 << 64) - 1;
    while (lo <= hi)
    {
        unsigned long long mid = lo + (hi - lo) / 2;
        __uint128_t sq = (__uint128_t)mid * mid;
        if (sq == x) return mid;
        if (sq < x)   lo = mid + 1;
        else          hi = mid - 1;
    }
    return hi;          /* hi is floor(sqrt(x)) */
}

/* Cache for sequence values. */
static seq_t *a_cache   = NULL;
static size_t a_cap     = 0;
static idx_t  a_max     = 0;

/* Cache for the coefficients c_k. */
static unsigned long long *c_cache = NULL;
static size_t c_cap = 0;
static idx_t  c_max = 0;

seq_t a(idx_t n)
{
    /* ------------------------------------------------------------
       Grow the sequence cache if necessary.
       ------------------------------------------------------------ */
    if ((size_t)n >= a_cap)
    {
        size_t new_cap = (size_t)n + 1;
        seq_t *new_a = (seq_t *)realloc(a_cache, new_cap * sizeof(seq_t));
        if (!new_a) exit(EXIT_FAILURE);
        a_cache = new_a;
        a_cap   = new_cap;
        /* Initialise the first term (a(0)=1) if we just created the
           array and it hasn't been initialised yet. */
        if (a_cap == new_cap)            /* this is the first allocation */
            a_cache[0] = (seq_t)1;
    }

    /* ------------------------------------------------------------
       Grow the coefficient cache if necessary.
       ------------------------------------------------------------ */
    if ((size_t)n >= c_cap)
    {
        size_t new_cap = (size_t)n + 1;
        unsigned long long *new_c =
            (unsigned long long *)realloc(c_cache, new_cap * sizeof(unsigned long long));
        if (!new_c) exit(EXIT_FAILURE);
        c_cache = new_c;
        c_cap   = new_cap;
    }

    /* ------------------------------------------------------------
       Compute coefficients c_k for k = c_max+1 .. n.
       ------------------------------------------------------------ */
    for (idx_t k = c_max + 1; k <= n; ++k)
    {
        idx_t m = k + 1;                                 /* (k+1) */
        __uint128_t prod = (__uint128_t)21 * m * m;      /* 21·(k+1)^2   */
        unsigned long long sqrt_val = isqrt128(prod);    /* floor(sqrt)   */
        c_cache[k] = sqrt_val / 3;                       /* floor(.../3)  */
    }
    if (c_max < n) c_max = n;

    /* ------------------------------------------------------------
       Compute sequence terms up to n.
       ------------------------------------------------------------ */
    for (idx_t i = a_max + 1; i <= n; ++i)
    {
        __int128 sum = 0;
        for (idx_t k = 1; k <= i; ++k)
        {
            __int128 prod = (__int128)a_cache[i - k] * (__int128)c_cache[k];
            if (k & 1ULL)  sum += prod;
            else           sum -= prod;
        }
        a_cache[i] = (seq_t)sum;   /* truncation to seq_t is OK for values that fit */
    }
    if (a_max < n) a_max = n;

    return a_cache[n];
}
