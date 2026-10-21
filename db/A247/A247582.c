/*
 * A247582  Triangle, read by rows, T(n,k) = (k+1)*Sum_{i=0..n-k}
 *          C(k+2*i,i)*C(n-i-1,n-k-i)/(k+i+1).
 * https://oeis.org/A247582
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 20, u16 n <= 54, u32 n <= 65, u64 n <= 65, u128 n <= 65,
 *            big any n
 * Helpers:   binom_u
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Symbolic computation of OEIS A247582
 *
 * Triangle T(n,k) = (k+1) * sum_{i=0..n-k}   C(k+2*i,i) * C(n-i-1,n-k-i) / (k+i+1)
 * Read by rows (n = 0,1,2,…, k = 0…n).  The function a(n) returns the nth
 * entry in this order (a(0)=1, a(1)=1, a(2)=2, …).
 *   a(0) = 1
 *   a(1) = 1
 *   a(2) = 2
 *   a(3) = 3
 *   a(4) = 1
 *
 * The implementation uses only integer arithmetic and the built‑in
 * seq_t type.  No external utilities are required.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/binom_u.h"

/* --------------------------------------------------------------------- */

/* --------------------------------------------------------------------- */
/*  Triangle entry T(n,k) for 0 <= k <= n
 *
 *  Implements the closed form with a single special term for i = n-k.
 */
static seq_t triangle_entry(idx_t n, idx_t k)
{
    seq_t sum = (seq_t)0;

    /* Sum over i = 0 .. n-k-1 (if any) */
    if (n > k)
    {
        for (idx_t i = 0; i < n - k; ++i)
        {
            seq_t term = ((seq_t)(k + 1)
                         * binom_u(k + 2 * i, i)
                         * binom_u(n - i - 1, n - k - i))
                         / ((seq_t)(k + i + 1));
            sum += term;
        }
    }

    /* Last term i = n-k:  binom(k-1,0) = 1 for all k (C(-1,0)=1 when k=0) */
    seq_t last = ((seq_t)(k + 1)
                 * binom_u(2 * n - k, n - k))
                 / (seq_t)(n + 1);
    return sum + last;
}

/* --------------------------------------------------------------------- */
/*  Public function required by the framework
 *
 *  Convert a linear index n into (row, column) of the triangular array,
 *  then evaluate the corresponding entry.
 */
seq_t a(idx_t n)
{
    /* Find row r such that r(r+1)/2 <= n < (r+1)(r+2)/2          */
    idx_t r = 0;
    idx_t cum = 0;                      /* number of elements before row r */
    while (cum + r + 1 <= n)
    {
        cum += r + 1;
        ++r;
    }
    idx_t k = n - cum;                   /* column in row r */

    return triangle_entry(r, k);
}
