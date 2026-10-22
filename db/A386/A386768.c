/*
 * A386768  a(n) = Sum_{k=0..n} 5^k * 2^(n-k) * binomial(3*n+k-1,k).
 * https://oeis.org/A386768
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 6, u64 n <= 12, u128 n <= 16,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A386768
   Formula: a(n) = Σ_{k=0..n} 5^k · 2^(n-k) · binomial(3·n + k – 1, k)
   The routine evaluates the sum iteratively, simplifying each term
   before multiplication so that the result never overflows a
   fixed‑width seq_t while still being exact for arbitrary‑precision
   builds. */
#include "../utils/pow2.h"

/* gcd for seq_t; works for both built‑in and arbitrary‑precision types. */
static inline seq_t seq_gcd(seq_t a, seq_t b)
{
    while (b != (seq_t)0)
    {
        seq_t t = a % b;
        a = b;
        b = t;
    }
    return a;
}

seq_t a(idx_t n)
{
    /* term for k = 0 : 5^0 * 2^(n-0) * binomial(3n-1,0) = 2^n */
    seq_t sum  = pow2(n);
    seq_t term = sum;          /* current term T_k */

    for (idx_t k = 1; k <= n; ++k)
    {
        /* compute T_k = T_{k-1} * 5 * (3n + k - 1) / (2*k) */
        seq_t denom = (seq_t)(2 * k);
        seq_t num   = term;                 /* start with previous term */

        /* cancel common factors between num and denom */
        seq_t g = seq_gcd(num, denom);
        num   /= g;
        denom  = denom / g;

        /* remaining factor (3n + k - 1) */
        seq_t extra = (seq_t)(3 * n + k - 1);
        g = seq_gcd(extra, denom);
        extra /= g;
        denom  = denom / g;

        /* factor 5 */
        g = seq_gcd((seq_t)5, denom);
        denom = denom / g;

        /* now multiply the reduced parts */
        term = num * extra;
        term = term * (seq_t)5;

        /* if a denominator remains, divide */
        if (denom != (seq_t)1)
            term = term / denom;

        sum += term;
    }
    return sum;
}
