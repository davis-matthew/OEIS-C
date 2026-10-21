/*
 * A256600  Triangle, read by rows consisting of terms T(n,k), k=0..2^n-1,
 *          where row n+1 equals the concatenation of the partial sums of the
 *          prior row with itself.
 * https://oeis.org/A256600
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabf
 * Formula:   a(n) = T(r,k) where r = floor(log2(n+1)), k = n - (2^r - 1). The
 *            triangle T is defined by T(0,0)=1; for r>=0 let
 *            S(j)=∑_{i=0}^{j}T(r,i) (partial sums). Then T(r+1,j)=S(j) for
 *            0≤j<2^r, and T(r+1,j+2^r)=S(j)
 * Range:     u8 n <= 69, u16 n <= 182, u32 n <= 1126, u64 n <= 8071,
 *            u128 n <= 8071, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>


seq_t a(idx_t n)
{
    /* Determine the row index r = floor(log2(n+1)) */
    idx_t r;
#if defined(__GNUC__) || defined(__clang__)
    r = 63 - __builtin_clzll(n + 1);
#else
    {
        idx_t v = n + 1;
        r = 0;
        while (v >>= 1) { ++r; }
    }
#endif

    /* Static storage for rows */
    static seq_t **rows = NULL;
    static idx_t rows_alloc = 0;

    if (rows_alloc <= r) {
        /* Resize the array of row pointers */
        rows = (seq_t**)realloc(rows, (r + 1) * sizeof(seq_t*));
        for (idx_t cur = rows_alloc; cur <= r; ++cur) {
            idx_t len = 1ULL << cur;          /* length of current row */
            rows[cur] = (seq_t*)malloc(len * sizeof(seq_t));
            if (cur == 0) {
                rows[0][0] = 1;                /* base case */
            } else {
                idx_t prev_len = len >> 1;      /* 2^(cur-1) */
                seq_t sum = 0;
                /* First half: partial sums of previous row */
                for (idx_t i = 0; i < prev_len; ++i) {
                    sum += rows[cur - 1][i];
                    rows[cur][i] = sum;
                }
                /* Second half: duplicate the partial sums */
                for (idx_t i = 0; i < prev_len; ++i) {
                    rows[cur][prev_len + i] = rows[cur][i];
                }
            }
        }
        rows_alloc = r + 1;
    }

    /* Position within the row */
    idx_t start_idx = (1ULL << r) - 1;
    idx_t pos = n - start_idx;
    return rows[r][pos];
}
