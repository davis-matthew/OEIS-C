/*
 * A295181  Square array A(n,k), n >= 0, k >= 0, read by antidiagonals, where
 *          column k is the expansion of e.g.f. exp(-k*x)/(1 - x)^k.
 * https://oeis.org/A295181
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   A(n,k) = (n-1)*(A(n-1,k)+k*A(n-2,k)), A(0,k)=1, A(1,k)=0. The
 *            sequence a(n) is the values A(i,j) read along the antidiagonals
 *            of the square array, i.e. for n≥0: let s = ⌊(√(8n+1)-1)/2⌋ (s =
 *            n + k) let cum = s*(s+1)/2 let i = n - cum // row index let k =
 *            s - i // column index then a(n) = A(i,k)
 * Range:     u8 n <= 33, u16 n <= 63, u32 n <= 77, u64 n <= 77, u128 n <= 77,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

/* helper: compute A(i,k) by the recurrence, iteratively.
   Works for i up to about sqrt(2*max_n). */
static inline seq_t compute_A(idx_t i, idx_t k)
{
    if (i == 0) return (seq_t)1;
    if (i == 1) return (seq_t)0;
    seq_t a0 = (seq_t)1;          /* A(0,k) */
    seq_t a1 = (seq_t)0;          /* A(1,k) */
    for (idx_t t = 2; t <= i; ++t) {
        seq_t term = a1 + (seq_t)k * a0;            /* A(t-1,k)+k*A(t-2,k) */
        term = term * (seq_t)(t-1);                /* *(t-1) */
        a0 = a1;
        a1 = term;
    }
    return a1;
}

/* Main sequence function */
seq_t a(idx_t n)
{
    /* find the antidiagonal index s such that
       s*(s+1)/2 <= n < (s+1)*(s+2)/2 */
    idx_t s = 0;
    while ((s + 1) * (s + 2) / 2 <= n)
        ++s;

    idx_t cum = s * (s + 1) / 2;          /* sum of 1..s */
    idx_t i   = n - cum;                  /* row index */
    idx_t k   = s - i;                    /* column index */

    return compute_A(i, k);
}
