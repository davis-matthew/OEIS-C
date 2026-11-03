/*
 * A354438  Square array A(n, k), n, k >= 0, read by antidiagonals; the
 *          factorial base expansion of A(n, k) is obtained by adding
 *          componentwise and reducing modulo their radix the digits of the
 *          factorial base expansions of n and k.
 * https://oeis.org/A354438
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl, base
 * Formula:   a(n) is the square array A(n,k) read by antidiagonals, where
 *            A(n,k) is defined by adding the factorial‑base expansions of n
 *            and k component‑wise modulo their radix
 * Range:     u8 n <= 1325, u16 n <= 1325, u32 n <= 1325, u64 n <= 1325,
 *            u128 n <= 1325, big any n
 * Validated: standard, pass, 94 terms through n = 1259 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The following function computes the value of the sequence */
#include <stdbool.h>

/* mapping from linear index n to pair (i,j) on the antidiagonals of T */
static inline void antidiagonal_coords(idx_t n, idx_t *i, idx_t *j)
{
    /* find s such that (s+1)(s+2)/2 <= n < (s+2)(s+3)/2 */
    idx_t s = 0;
    while (((s + 1) * (s + 2)) / 2 <= n) ++s;
    idx_t pos = n - (s * (s + 1)) / 2;  /* 0 <= pos <= s */
    *i = pos;
    *j = s - pos;
}

/* Compute the table entry A(u,v) according to the factorial‑base rule */
static seq_t factorial_base_add(idx_t u, idx_t v)
{
    seq_t result = 0;
    seq_t fac    = 1;          /* will hold the factorial of the current position (k!) */
    idx_t k      = 1;          /* position counter, starting at 1 for the least significant digit */

    /* Continue as long as either number still has a non‑zero digit in higher positions */
    while (u || v)
    {
        /* digits for the current position */
        seq_t du = (seq_t)(u % (k + 1));
        seq_t dv = (seq_t)(v % (k + 1));

        /* mod addition of the digits */
        seq_t digit = (du + dv) % (k + 1);

        /* add to the result */
        result += digit * fac;

        /* update factorial for the next higher position */
        fac *= (k + 1);

        /* shift numbers to the next position */
        u /= (k + 1);
        v /= (k + 1);

        ++k;
    }

    return result;
}

/* Public sequence function */
seq_t a(idx_t n)
{
    idx_t i, j;
    antidiagonal_coords(n, &i, &j);          /* get coordinates (i,j) of n on the antidiagonals */
    return factorial_base_add(i, j);          /* compute A(i,j) */
}
