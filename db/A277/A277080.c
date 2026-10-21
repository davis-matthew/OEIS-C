/*
 * A277080  Irregular triangle read by rows: T(n,k) = number of size k subsets
 *          of S_n that remain unchanged by reverse.
 * https://oeis.org/A277080
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabf
 * Formula:   a(n) = the n‑th element of the irregular triangle T(m,k) read by
 *            rows, where T(m,k) = C(m!/2, k/2) if k is even, 0 if k is odd.
 *            For m = 0 or 1 all entries of the row are 1
 * Range:     u8 n <= 21, u16 n <= 46, u32 n <= 56, u64 n <= 59, u128 n <= 59,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

/* Compute binomial coefficient C(n,k) using the multiplicative formula.
   n and k are idx_t, but the intermediate result is stored in seq_t.
   The loop computes the coefficient exactly. */
static seq_t binom_idx(idx_t n, idx_t k)
{
    if (k > n - k)            /* use symmetry */
        k = n - k;
    seq_t r = 1;
    for (idx_t i = 1; i <= k; ++i) {
        r = r * (n - k + i) / i; /* exact division at each step */
    }
    return r;
}

seq_t a(idx_t n)
{
    idx_t row   = 0;   /* current row index */
    idx_t fact  = 1;   /* fact = row! (0! = 1) */
    idx_t start = 0;   /* first index belonging to this row */

    while (1) {
        idx_t row_len = fact + 1;  /* number of entries in this row: n! + 1 */

        if (n < start + row_len) {              /* n lies in this row */
            /* Rows 0 and 1 contain only 1’s */
            if (row <= 1)
                return (seq_t)1;

            idx_t offset = n - start;            /* column index k            */
            if (offset & 1)                      /* k odd → 0 */
                return (seq_t)0;

            idx_t k_half = offset >> 1;          /* k/2                      */
            idx_t m      = fact >> 1;            /* row! / 2                 */
            return binom_idx(m, k_half);
        }

        /* move to the next row */
        start += row_len;
        fact  *= (row + 1);   /* next factorial (row+1)!          */
        ++row;
    }
}
