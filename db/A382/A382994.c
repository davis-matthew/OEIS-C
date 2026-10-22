/*
 * A382994  Square array A(n,k), n >= 1, k >= 1, read by antidiagonals
 *          downwards, where A(n,k) = -Sum_{d|n} phi(n/d) * (-k)^d.
 * https://oeis.org/A382994
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, tabl
 * Formula:   not stated
 * Range:     i8 n <= 23, i16 n <= 55, i32 n <= 55, i64 n <= 55, i128 n <= 55,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A382994: read the square array A(n,k) by antidiagonals downwards.
   A(n,k) = - Σ_{d | n} φ(n/d) * (-k)^d
   The returned value a(m) is the m-th term in this ordering. */
#include <stdint.h>

/* Return the Euler totient of integer e (0 < e <= 141).  The result is small
   and thus safely fits into seq_t. */
static seq_t phi_small(uint64_t e)
{
    seq_t result = (seq_t)e;
    uint64_t t = e;
    for (uint64_t p = 2; p * p <= t; ++p)
    {
        if (t % p == 0)
        {
            while (t % p == 0) t /= p;
            result -= result / p;
        }
    }
    if (t > 1) result -= result / t;
    return result;
}

/* Main sequence function. */
seq_t a(idx_t n)
{
    /* 1. Find the antidiagonal (sum = S) containing the n-th term.
       Number of elements up to diagonal S is (S-1)*S/2. */
    idx_t S = 1;
    idx_t cum = 0;
    while (cum < n)
    {
        ++S;
        cum = (S - 1) * S / 2;
    }

    /* 2. Determine position within that diagonal. */
    idx_t prev = (S - 2) * (S - 1) / 2;          /* elements before diagonal S */
    idx_t offset = n - prev - 1;                 /* 0‑based index inside the diagonal */
    idx_t row = offset + 1;                      /* n in A(n,k) */
    idx_t col = S - row;                         /* k in A(n,k) */

    /* 3. Pre‑compute powers of (-k) up to row. */
    seq_t powk[200]; /* plenty big for row <= 141 */
    powk[0] = (seq_t)1;
    seq_t base = (seq_t)(- (int64_t)col);
    for (idx_t d = 1; d <= row; ++d)
        powk[d] = powk[d - 1] * base;

    /* 4. Sum over divisors of row. */
    seq_t sum = (seq_t)0;
    for (idx_t d = 1; d <= row; ++d)
    {
        if (row % d == 0)
        {
            seq_t phi = phi_small(row / d);
            sum += phi * powk[d];
        }
    }

    /* 5. Return -sum. */
    return -sum;
}
