/*
 * A373628  Triangle read by rows: T(n,k) is the number of occurrences of the
 *          periodic substring (0011)^k in the periodic string (000111)^n.
 * https://oeis.org/A373628
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) is the entry T(r,k) of the triangle where r =
 *            ⌊(√(8n+1)-1)/2⌋ and k = n - r(r+1)/2, T(r,0)=1 and for k>0
 *            T(r,k) = 9^k * Σ_{i=0}^{min(2k,r-k)} binom(2k,i) * 2^i *
 *            binom(4k + (r-k-i), r-k-i)
 * Range:     u8 n <= 6, u16 n <= 17, u32 n <= 44, u64 n <= 44, u128 n <= 44,
 *            big any n
 * Helpers:   binom, floor_triangle_index, pow_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*   This follows from the generating function:
 *   A(x,y) = (1-x)^3 / ((1-x)^4 - 9x(1+2x)^2 y)
 *   giving  T(n,k)=9^k·[x^{n-k}](1+2x)^{2k}(1-x)^{-(4k+1)}.     */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/binom.h"
#include "../utils/pow_seq.h"
#include "../utils/floor_triangle_index.h"

/* Compute T(r,k) for 0 ≤ k ≤ r */
static seq_t T_row_col(idx_t r, idx_t k)
{
    if (k == 0) return (seq_t)1;
    if (k > r) return (seq_t)0;

    idx_t m = r - k;                 /* remaining length after removing pattern */
    seq_t sum = 0;
    seq_t p2   = 1;                  /* 2^i, updated incrementally */

    idx_t i_max = (m < 2 * k) ? m : 2 * k;
    for (idx_t i = 0; i <= i_max; ++i) {
        seq_t term = binom(2 * k, i);         /* binom(2k, i) */
        term = term * p2;                    /* * 2^i */
        term = term * binom(4 * k + m - i, m - i); /* * binom(4k + m - i, m - i) */
        sum = sum + term;
        p2  = p2 * (seq_t)2;                /* next power of 2 */
    }
    seq_t nine_pow = pow_seq((seq_t)9, k);
    return nine_pow * sum;
}

/* Public entry point */
seq_t a(idx_t n)
{
    /* Special case n == 0 handled by table */
    idx_t r = floor_triangle_index(n);
    idx_t k = n - r * (r + 1) / 2;
    return T_row_col(r, k);
}
