/*
 * A245692  Number T(n,k) of endofunctions f on [n] that are self-inverse on
 *          [k] but not on [k+1]; triangle T(n,k), n>=0, 0<=k<=n, read by
 *          rows.
 * https://oeis.org/A245692
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   T(n,k) = A245348(n,k) - A245348(n,k+1); a(n) is the triangle
 *            read by rows, i.e., a(n) = T(r,k) where r and k are found from
 *            the row/column indices. The value is computed by the closed form
 *            for A245348 *
 * Range:     u8 n <= 14, u16 n <= 27, u32 n <= 54, u64 n <= 152,
 *            u128 n <= 377, big any n
 * Helpers:   big_pow, binom, factorial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/binom.h"
#include "../utils/factorial.h"
#include "../utils/big_pow.h"

/* Pre‑computed involution numbers: I(m) = #involutions on m points */
static seq_t invols[201];
static bool invols_done = false;

/* Computes A(n,k) = Σ_{r=0}^{min(k,n-k)} C(k,r)*C(n-k,r)*r!*I(k-r)*n^{n-k-r} */
static seq_t compute_A(idx_t n, idx_t k)
{
    if (k > n) return (seq_t)0;
    seq_t sum = 0;
    idx_t limit = (k < n - k) ? k : (n - k);
    for (idx_t r = 0; r <= limit; ++r)
    {
        seq_t term = binom(k, r);
        term *= binom(n - k, r);
        term *= factorial(r);
        term *= invols[k - r];
        term *= big_pow((seq_t)n, (seq_t)(n - k - r));
        sum += term;
    }
    return sum;
}

seq_t a(idx_t n)
{
    /* find the row r and column k from the index n */
    idx_t r = 0;
    while ((r + 1) * (r + 2) / 2 <= n) ++r;
    idx_t k = n - r * (r + 1) / 2;

    /* initialise the involution table once */
    if (!invols_done)
    {
        invols[0] = (seq_t)1;
        invols[1] = (seq_t)1;
        for (idx_t i = 2; i <= 200; ++i)
        {
            invols[i] = invols[i - 1] + (seq_t)(i - 1) * invols[i - 2];
        }
        invols_done = true;
    }

    seq_t a_val   = compute_A(r, k);
    seq_t a_next  = compute_A(r, k + 1);   /* returns 0 when k==r */
    return a_val - a_next;
}
