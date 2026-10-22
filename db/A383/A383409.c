/*
 * A383409  Expansion of e.g.f.
 *          (exp(x)-1)*(exp(x)-x)*(exp(x)-x^2/2)*(exp(x)-x^3/6).
 * https://oeis.org/A383409
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 4^n - 3^n - n * ( 3^(n-1) - 2^(n-1) ) - C(n,2) * (
 *            3^(n-2) - 2^(n-2) ) - C(n,3) * ( 3^(n-3) - 2^(n-1) + 3 ) +
 *            C(n,4) * ( 2^(n-2) - 4 ) + 5 * C(n,5) * ( 2^(n-4) - 2 ) - 60 *
 *            C(n,6) (omit the last term when n = 6) (C(n,k) = binomial
 *            coefficient)
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 16, u64 n <= 24, u128 n <= 24,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

/* Compute base^exp with exponentiation by squaring.  exp is non‑negative. */
static inline seq_t power_seq(seq_t base, idx_t exp)
{
    seq_t result = 1;
    while (exp)
    {
        if (exp & 1) result *= base;
        base *= base;
        exp >>= 1;
    }
    return result;
}

/* Compute binomial coefficient C(n,k) for 0 <= k <= n.
   The routine uses the multiplicative definition and works with
   small k (here k <= 6).  For larger k it would still be correct but
   slower. */
static inline seq_t binom_seq(idx_t n, idx_t k)
{
    if (k > n) return 0;
    if (k > n - k) k = n - k;        /* use symmetry */
    seq_t res = 1;
    for (idx_t i = 1; i <= k; ++i)
        res = res * (n - k + i) / i;   /* division is exact */
    return res;
}

/* Main sequence function. */
seq_t a(idx_t n)
{
    if (n == 0) return 0;

    seq_t result = power_seq(4, n) - power_seq(3, n);

    if (n >= 1)
    {
        seq_t inner = power_seq(3, n-1) - ((seq_t)1 << (n-1));   /* 2^(n-1) */
        result -= ((seq_t)n) * inner;
    }

    if (n >= 2)
    {
        seq_t inner = power_seq(3, n-2) - ((seq_t)1 << (n-2));   /* 2^(n-2) */
        result -= binom_seq(n, 2) * inner;
    }

    if (n >= 3)
    {
        seq_t inner = power_seq(3, n-3) - ((seq_t)1 << (n-1)) + 3;   /* 2^(n-1) */
        result -= binom_seq(n, 3) * inner;
    }

    if (n >= 4)
    {
        seq_t inner = ((seq_t)1 << (n-2)) - 4;   /* 2^(n-2) */
        result += binom_seq(n, 4) * inner;
    }

    if (n >= 5)
    {
        seq_t inner = ((seq_t)1 << (n-4)) - 2;   /* 2^(n-4) */
        result += (seq_t)5 * binom_seq(n, 5) * inner;
    }

    if (n != 6)
    {
        result -= (seq_t)60 * binom_seq(n, 6);
    }

    return result;
}
