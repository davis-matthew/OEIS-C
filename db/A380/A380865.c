/*
 * A380865  Triangle read by rows: T(n, k) = 2^(2*n)*JacobiP(n - k, k, -1/2 -
 *          n, -1).
 * https://oeis.org/A380865
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   T(n,k) = 2^(n+k) * ∏_{j=1}^{n-k} (2k+2j-1)/j Sequence a(n) is
 *            the flattened triangle of T(n,k) with row r, column k starting
 *            at indices 0
 * Range:     u8 n <= 9, u16 n <= 22, u32 n <= 44, u64 n <= 44, u128 n <= 44,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"

static seq_t T_value(idx_t n, idx_t k)
{
    seq_t result = pow2(n + k);           /* 2^(n+k) */
    idx_t m = n - k;                      /* number of factors */
    for (idx_t j = 1; j <= m; ++j) {
        seq_t factor = (seq_t)2 * k + (seq_t)2 * j - 1;  /* odd number 2k+2j-1 */
        result *= factor;
        result /= j;                    /* exact division */
    }
    return result;
}

seq_t a(idx_t n)
{
    /* Find the row r such that cumulative elements < n < cumulative + row_length */
    idx_t r = 0;
    idx_t cum = 0;
    while (true) {
        idx_t row_len = r + 1;          /* length of current row */
        if (n < cum + row_len) break;
        cum += row_len;
        ++r;
    }
    idx_t k = n - cum;                  /* column index within row r */
    return T_value(r, k);
}
