/*
 * A356601  Triangle read by rows. T(n, k) = denominator(Integral_{z=0..1}
 *          Eulerian(n, k)*z^(k + 1)*(z - 1)^(n - k - 1) dz), where
 *          Eulerian(n, k) = A173018(n, k), for n >= 1, and T(0, 0) = 1.
 * https://oeis.org/A356601
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl, frac
 * Formula:   not stated
 * Range:     u8 n <= 14, u16 n <= 29, u32 n <= 57, u64 n <= 65, u128 n <= 65,
 *            big any n
 * Helpers:   eulerian_row_value, gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A356601 – Triangle T(n,k) read by rows  
   T(n,k) = denominator(-1^(k-n+1)*E(n,k)*(k+1)!*(n-k-1)!/(n+1)!)
   where E(n,k) = Eulerian number (OEIS A173018).  
   The sequence a(n) enumerates T(n,k) row‑wise, with a(0)=1, a(1)=2, a(2)=1, ... */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/gcd.h"   /* gcd(seq_t,seq_t) */
#include "../utils/eulerian_row_value.h"

static seq_t factorial_u64(idx_t m)
{
    seq_t f = 1;
    for (idx_t i = 1; i <= m; ++i)
        f = f * (seq_t)i;
    return f;
}

/* Denominator of the entry T(n,k), 0 <= k <= n.
   For k==n the value is defined as 1. */
static seq_t triangle_denominator(idx_t n, idx_t k)
{
    if (k == n) return 1;

    /* Eulerian number */
    seq_t E = eulerian_row_value(n, k);

    /* Factorials needed in R(n,k) */
    seq_t fac_k1   = factorial_u64(k + 1);        /* (k+1)! */
    seq_t fac_rk1  = factorial_u64(n - k - 1);    /* (n-k-1)! */
    seq_t denom    = factorial_u64(n + 1);        /* (n+1)! */

    /* Numerator */
    seq_t numerator = E * fac_k1 * fac_rk1;

    /* Reduce fraction */
    seq_t g = gcd(numerator, denom);
    denom = denom / g;          /* denominator in lowest terms */
    return denom;
}

/* Main function: a(idx_t n) returns the nth term of the sequence. */
seq_t a(idx_t n)
{
    /* Find row r such that r*(r+1)/2 <= n < (r+1)*(r+2)/2 */
    idx_t r = 0;
    while (((r + 1) * (r + 2)) / 2 <= n) r++;

    idx_t start = r * (r + 1) / 2;   /* first index in row r */
    idx_t c = n - start;             /* column within the row */

    if (c == r) return 1;            /* last element of the row */

    return triangle_denominator(r, c);
}
