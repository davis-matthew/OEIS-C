/*
 * A286785  Triangle T(n,k) read by rows: coefficients of polynomials P_n(t)
 *          defined in Formula section.
 * https://oeis.org/A286785
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabf
 * Formula:   not stated
 * Range:     u8 n <= 11, u16 n <= 37, u32 n <= 108, u64 n <= 358,
 *            u128 n <= 1289, big any n
 * Helpers:   build_primes, int_prime_exp, seq_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*   A286785   (OEIS)
 *   a(n) = T(r,k) where (r,k) is obtained by reading the triangle
 *   T(n,k) = C(n-1,k)*C(2*n+2,n-k)/(n+1) row‑by‑row.
 *   The triangle has 1 term in row 0 (T(0,0)=1), 1 in row 1, 2 in row 2, etc.
 *
 *   Implementation uses prime factorisation to avoid overflow.
 *   Primes up to 2*r+2 are generated once with build_primes.
 *   seq_pow is used to raise primes to the required exponent.
 */

#include <stdlib.h>
#include "../utils/seq_pow.h"
#include "../utils/build_primes.h"
#include "../utils/int_prime_exp.h"

/* exponent of prime p in n! (Legendre's formula) */
static idx_t factorial_prime_exp(idx_t n, idx_t p)
{
    idx_t e = 0;
    while (n)
    {
        n /= p;
        e += n;
    }
    return e;
}

/* stored prime table */
static idx_t *prime_table   = NULL;
static size_t prime_count   = 0;
static idx_t  prime_limit   = 0;

/* make sure primes up to 'limit' are available */
static void ensure_primes_up_to(idx_t limit)
{
    if (limit <= prime_limit)
        return;
    if (prime_table)
    {
        free(prime_table);
        prime_table = NULL;
    }
    build_primes(limit, &prime_table, &prime_count);
    prime_limit = limit;
}

/* main sequence function */
seq_t a(idx_t n)
{
    /* special case row 0 */
    if (n == 0)
        return (seq_t)1;

    /* find row r and column k for global index n */
    idx_t r        = 0;
    idx_t cum      = 1;        /* cumulative count after row 0 */
    while (n >= cum)
    {
        ++r;
        cum += r;              /* cum = 1 + r(r+1)/2 after loop */
    }
    idx_t prev_start = cum - r;          /* first index of row r */
    idx_t k          = n - prev_start;   /* column index in this row */

    /* compute T(r,k) using prime factorisation */
    ensure_primes_up_to(2 * r + 2);

    seq_t value = 1;
    idx_t limit = 2 * r + 2;            /* 2*r+2 for the second binomial */

    for (size_t i = 0; i < prime_count && prime_table[i] <= limit; ++i)
    {
        idx_t p = prime_table[i];

        /* exponent of p in C(r-1,k) */
        idx_t e = factorial_prime_exp(r - 1, p)
                - factorial_prime_exp(k, p)
                - factorial_prime_exp(r - 1 - k, p);

        /* exponent of p in C(2*r+2, r-k) */
        e += factorial_prime_exp(limit, p)
           - factorial_prime_exp(r - k, p)
           - factorial_prime_exp(limit - (r - k), p);

        /* subtract exponent of p in (r+1) */
        e -= int_prime_exp(r + 1, p);

        if (e)
        {
            seq_t pe = seq_pow((seq_t)p, e);
            value *= pe;
        }
    }

    return value;
}
