/*
 * A336012  a(n) is the number of chains from {} to a top element in the poset
 *          of even sized subsets of {1,2,...,n} ordered by inclusion.
 * https://oeis.org/A336012
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = C(n) for even n, a(n) = n*C(n-1) for odd n, where C(0)=1
 *            and for even m>0: C(m)=∑_{k=1}^{m/2} binom(m,2k)*C(m-2k)
 * Range:     u8 n <= 6, u16 n <= 9, u32 n <= 14, u64 n <= 22, u128 n <= 25,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* Cache for the values C(m) (defined only for even m; odd indices store 0) */
static seq_t *C = NULL;
static idx_t C_len = 0;

/* Ensure that C[0..n] are computed. */
static void ensure_C(idx_t n)
{
    if (C_len > n)
        return;

    idx_t new_len = n + 1;
    seq_t *tmp = (seq_t *)realloc(C, new_len * sizeof(seq_t));
    if (!tmp) {
        /* Allocation failed – abort */
        exit(1);
    }
    C = tmp;

    for (idx_t m = C_len; m <= n; ++m) {
        if (m % 2 != 0) {          /* skip odd m; store 0 for symmetry */
            C[m] = 0;
            continue;
        }
        if (m == 0) {              /* base case */
            C[0] = 1;
            continue;
        }
        seq_t sum = 0;
        seq_t binom = 1;           /* binom(m,0) = 1 */
        for (idx_t k = 1; k <= m / 2; ++k) {
            idx_t r = 2 * k;
            /* Update binom to binom(m, r) from binom(m, r-2) */
            seq_t num1 = (seq_t)(m - r + 2);
            seq_t num2 = (seq_t)(m - r + 1);
            seq_t den1 = (seq_t)r;
            seq_t den2 = (seq_t)(r - 1);
            binom = binom * num1;
            binom = binom * num2;
            binom = binom / den1;
            binom = binom / den2;
            sum += binom * C[m - r];
        }
        C[m] = sum;
    }
    C_len = new_len;
}

/* Public function */
seq_t a(idx_t n)
{
    ensure_C(n);
    if ((n & 1) == 0)                /* n even */
        return C[n];
    else                               /* n odd */
        return (seq_t)n * C[n - 1];
}
